// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// this source is written by hyam.
// but original algorithm is here
//   http://www.stat.ucla.edu/~hqxu/
//   http://www.stat.ucla.edu/~hqxu/pub/Xu2002.pdf
//
#include <time.h>

#include "AlgorithmXu.h"
#include "Util.h"
#include "Random.h"

#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }


//-----------------------------------------------------------------------------
// Constructor
AlgorithmXu::AlgorithmXu(ProgInfo *progInfo)
{
    
    this->progInfo   = progInfo;
    
}

//-----------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------
Matrix *AlgorithmXu::Generate(ModelFile *modelFile)
{

    if(modelFile->paramSizeList.size() < 3){
        cout << "# [X] number of factors must be greater than 3." << endl;
        return 0;
    }
    
    int size1 = modelFile->testParamRecords[0]->paramValues.size();
    int size2 = modelFile->testParamRecords[1]->paramValues.size();

    // calculate the row size N
    int N;
    if(this->progInfo->rowSize == 0){
        N = Util::GetMixedArraySize(modelFile->paramSizeList);
    }else{
        N = this->progInfo->rowSize;
        if( N < (size1*size2) ){
            cout << "# [X] -size value is too small for Xu." << endl;
            return 0;
        }    
    }

    if(!this->progInfo->isQuiet){
        cout << "# initial array size is "
             << N << " x 2" << endl;
        cout << "# repeat T : " << this->progInfo->tryCount << endl;
    }

    if(N > 1000){
        cout << "# [X] too large array size( N >= 1,000 )" <<endl; 
        return 0;
    }
    
    
    this->D = new Matrix(N,0);
    
    
    //-----------------------------------------------------
    // 1.
    //  For k=1,...,n, compute the lower bound L(k) 
    //  according to (2).
    this->ComputeLk(modelFile->paramSizeList);
        //  lower bound L(k) list is D->LkVector

    //-----------------------------------------------------
    // 2.
    //  specify an initial design d with two columns
    int cnt1 = N / size1;
    vector<string> str_vec1;
    for(int i=0; i<size1; i++){
        for(int j=0; j<cnt1; j++){
            str_vec1.push_back(Util::Int2String(i));
        }
    }
    this->D->AddCol(str_vec1);  // column 1

    int cnt2 = N / size2;
    vector<string> str_vec2;
    for(int j=0; j<cnt2; j++){
        for(int i=0; i<size2; i++){
            str_vec2.push_back(Util::Int2String(i));
        }
    }
    this->D->AddCol(str_vec2); // column 2

    this->D->paramSizeVector.resize(2);
    this->D->paramSizeVector[0] = size1;
    this->D->paramSizeVector[1] = size2;
    
    // compute Sigma(d) and J2(d)
    this->base_J2 = this->D->GetJ2Optiomality();   // J2(d)
    
    if(!this->progInfo->isQuiet){
        cout << "## 1st J(2) = " << this->base_J2 << endl;
    }
    
    //-----------------------------------------
    // 3.
    //  For k= 3, ... n, do the following:
    //
    //  (col is 0 origin, so do 2,...,n-1)
    for(int col=2; col<modelFile->testParamRecords.size(); col++){
        
        int Sk = modelFile->testParamRecords[col]->paramValues.size();

        long Lk = this->LkVector[col];  // L(k)

        if(!this->progInfo->isQuiet){
            cout << "## add column : k=" << ( col + 1 ) << endl;
            cout << "## L(k) = " << Lk << endl;
        }
        
        this->AddColumn(col, Sk, Lk);
        if(this->D == 0){
            return 0; // null(error)
        }
    }
    
    
    return this->D;
}

//-----------------------------------------------------------------------------
// private method
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  For k=1,...,n, compute the lower bound L(k) 
//  according to (2).
void  AlgorithmXu::ComputeLk(list<int> paramSizeList)
{
    
    vector<int> sizeVec;
    
    // list --> vector
    list<int>::iterator ite = paramSizeList.begin(); 
    while( ite != paramSizeList.end() ) {
        int size = *ite;
        sizeVec.push_back(size);
        ++ite; 
    }
    
    for(int i=0; i<sizeVec.size(); i++){
        this->LkVector.push_back(
                    Util::GetLn(sizeVec, this->D->rowSize, i+1) );
    }
}


//-----------------------------------------------------------------------------
// col : 2,3,...n-1
// Sk  : level of column k
// Lk  : L(k)
void AlgorithmXu::AddColumn(int col, int Sk, long Lk)
{
    //--------------------------------------------
    // 3. a.
    //  Randomly generate a balanced column c

    this->D->vecC.resize(0); // renew
    
    for(int i=0; i< this->D->rowSize; i++){
        this->D->vecC.push_back(Util::Int2String(i % Sk));
    }

    
    long j2plus = -1;
    
    long best_j2_in_T = 2147483647 ; // max of signed long
    vector<string> best_vecC;
    
    // Repeat (a) to (b) T times and choose a column C that produces
    // the smallest J2(d+)
    for(int t=0; t< this->progInfo->tryCount ; t++){
        
        if(this->progInfo->TimeSeed){
            Random r2;
            random_shuffle( this->D->vecC.begin(), this->D->vecC.end(), r2 );
        }else{
            // fixed seed
            random_shuffle( this->D->vecC.begin(), this->D->vecC.end() );
        }
        
        j2plus = this->D->GetJ2Plus(this->base_J2, Sk);  // J2(d+)
            if(j2plus < best_j2_in_T){
                best_j2_in_T = j2plus;
                best_vecC = this->D->vecC;
            }

        if(j2plus==Lk){
            break;  // goto (d)
        }
        
        
        // repeat (b) until no further improvement
        while(1){
            
            // check all (a,b), and calculate best_deltaAB
            this->CheckDeltaAB(col);
            
            if(this->best_deltaAB == 0) {
                // no further improvement is made.
                break;
            }
            
            // reduce J2(d+) by 2*Delta(a,b)
            j2plus = j2plus - (2 * this->best_deltaAB );
            // update the best score
            if(j2plus < best_j2_in_T){
                best_j2_in_T = j2plus;
                best_vecC = this->D->vecC;
            }

            
            // if J2(d+)=L(k) then go to (d)
            if(j2plus==Lk){
                break;
            }
        
        } // end of while
        
        
        if(!this->progInfo->isQuiet){
            cout << "# [T=" << (t + 1) << "] J2(d+)=" << j2plus <<endl;
        }
        
        if(j2plus==Lk){ 
            // cout << "# it is OA" << endl;
            break;
        }

    } // end of for T times
    

    // (d)
    //   J2(d) = J2(d+)
    this->base_J2 = best_j2_in_T;
    if(best_j2_in_T==2147483647){
        cout << "# [X] J2(d+) is invalid." << endl;
        this->D = 0;
        return;
    }
    
    this->D->AddCol(best_vecC);  
    this->D->paramSizeVector.resize(col + 1);
    this->D->paramSizeVector[col] = Sk;
    
    if(!this->progInfo->isQuiet){
        cout << "# best J2(d+)=" << best_j2_in_T <<endl;
    }
}



//-----------------------------------------------------------------------------
// check Delta(a,b) and replace column c   (a,c)<-->(b,c)
void  AlgorithmXu::CheckDeltaAB(int col)
{
    
    long best_deltaAB=0;
    int best_a=0;
    int best_b=0;
    
    // choose a pair with largest Delta(a,b)
    for(int a=0; a < this->D->rowSize; a++){
        for(int b=a+1; b < this->D->rowSize; b++){
            
            if(this->D->vecC[a] != this->D->vecC[b]){
                long deltaAB = this->D->GetDeltaAB(a,b);
                
                if(best_deltaAB < deltaAB){
                    best_a = a;
                    best_b = b;
                    best_deltaAB = deltaAB;
                }
            }
        }
    }

    
     
    // exchange the symbols in rows a and b of column C
    string tmp   = this->D->vecC[best_a];
    this->D->vecC[best_a] = this->D->vecC[best_b] ;
    this->D->vecC[best_b] = tmp ;
    
    this->best_deltaAB = best_deltaAB;
    
}
