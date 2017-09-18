// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.



#include <string> 
#include <iostream> 
#include <list> 
#include <vector> 
#include <map> 


#include "Matrix.h"
#include "Util.h"


using namespace std;



//-----------------------------------------------------------------------------
// Constructor
//
//
//    +--   +-----------+
//    |     |           |  vector< vector<int> > 
// rowSize  |  data     |
//    |     |           |
//    +--   +-----------+
//          <--colSize-->
//
//
//
//

Matrix::Matrix()
{
    this->rowSize  = 0;
    this->colSize  = 0;
    this->data.resize(0);

    this->J2Optiomality = 0;
    this->LnValue = 0;
}
Matrix::Matrix(int rowNum, int colNum)
{

    this->rowSize  = rowNum;
    this->colSize  = colNum;
    
    this->data.resize(rowNum); //initial element
    for(int i=0; i<rowNum; i++){
        this->data[i].resize(colNum); 
    }

    this->J2Optiomality = 0;
    this->LnValue = 0;
}
Matrix::Matrix(vector< vector<string> > array)
{
    this->rowSize  = array.size();
    this->colSize  = array[0].size();
    this->data = array;

    this->J2Optiomality = 0;
    this->LnValue = 0;
}


//-----------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------
void  Matrix::Set(int row, int col, string elementData)
{
    this->data[row][col] = elementData; 
}
//-----------------------------------------------------------------------------
void  Matrix::AddRow(vector<string> vec_data)
{
    if(this->colSize != 0 && vec_data.size() != this->colSize){
        cout << "# [X] input data size must be equal to matrix column size." 
             << endl;
        return;
    }
    this->data.push_back(vec_data);
    
    this->rowSize++ ;
    if(this->colSize == 0){
        this->colSize = vec_data.size() ;
    }
}

void  Matrix::AddRow(vector<int> vec_int)
{
    if(this->colSize != 0 && vec_int.size() != this->colSize){
        cout << "# [X] input data size must be equal to matrix column size(vec int)." 
             << endl;
        return;
    }
    vector<string> vec_str;
    for(int i=0; i< vec_int.size(); i++){
        vec_str.push_back( Util::Int2String(vec_int[i]) );
    }
    this->data.push_back(vec_str);
    
    this->rowSize++ ;
    if(this->colSize == 0){
        this->colSize = vec_int.size() ;
    }
}
//-----------------------------------------------------------------------------
void  Matrix::AddCol(vector<string> vec_data)
{
    
    if(this->colSize != 0 && vec_data.size() != this->rowSize){
        cout << "# [X] input data size must be equal to matrix row size." 
             << endl;
       // cout << "#     this->colSize = " << this->colSize
       //      << ", vec_data.size() = " << vec_data.size() 
       //      << endl;
        return;
    }
    if(this->colSize == 0 ){
        this->data.resize(vec_data.size());
        this->rowSize = vec_data.size();
    }
    
    for(int i=0; i<vec_data.size(); i++){
        this->data[i].push_back(vec_data[i]); 
    }
    
    this->colSize++;
        
}

//-----------------------------------------------------------------------------
// replace column c data
void  Matrix::ReplaceCol(int c, vector<string> col_vec)
{
    
    if(col_vec.size() != this->rowSize){
        cout << "# [X] input data size must be equal to matrix row size." 
             << endl;
        return;
    }
    if(c >= this->colSize  ){
        cout << "# [X] input column position(c) is invalid." 
             << endl;
        cout << "c=" << c << endl;
        return;
    }
    
    for(int i=0; i<this->rowSize; i++){
        this->Set(i, c, col_vec[i]); 
    }
    
        
}

//-----------------------------------------------------------------------------
string  Matrix::Get(int row, int col)
{
    if(row >= this->rowSize || row < 0){
        cout << "[Matrix::Get] invalid row size:" << row << endl;
        return "";
    }
    if(col >= this->colSize || col < 0){
        cout << "[Matrix::Get] invalid col size:" << col << endl;
        return "";
    }
    return this->data[row][col] ;
}

int  Matrix::GetInt(int row, int col)
{
    return atoi( (this->data[row][col]).c_str() ) ;
}
//-----------------------------------------------------------------------------
vector<string>  Matrix::GetRow(int row)
{
    return this->data[row] ;
}

//-----------------------------------------------------------------------------
list<string>  Matrix::GetCol(int col)
{
    list<string> tmp_list;
    
    for(int i=0; i<this->rowSize; i++){
        tmp_list.push_back( this->data[i][col] ); 
    }

    return tmp_list ;
}

//-----------------------------------------------------------------------------
//
//           colData
//            +---+
//            | 0 |
//            | 1 |
//            | 0 |
//            | 1 |
//            +---+
//              |
//              |
//  data        V         colPos=2
//    +---+---+---+---+
//    |   |   | 0 |   |
//    |   |   | 1 |   |
//    |   |   | 1 |   |
//    |   |   | 0 |   |
//    +---+---+---+---+
//
//
//
//

void  Matrix::SetColumnData(int colPos, vector<string> colData)
{
    for(int rowCnt=0; rowCnt < this->rowSize; rowCnt++){
        this->data[rowCnt][colPos] = colData[rowCnt];
    }
}

//
// matrix data to paramSizeVector 
//
void  Matrix::MakeParamSizeVector()
{
    
    list<string>  tmp_list;
    
    this->paramSizeVector.resize(0);
    
    for(int i=0; i<this->colSize; i++){
        
        tmp_list = this->GetCol(i);

        tmp_list.sort();
        tmp_list.unique();
        
        this->paramSizeVector.push_back( tmp_list.size() );
                
        vector<string> tmp_vec ;
        
        list<string>::iterator ite = tmp_list.begin(); 
        while( ite != tmp_list.end() ) {
            tmp_vec.push_back( *ite ); ;
            ++ite; 
        }
        // tmp_vec.assign(tmp_list.begin(), tmp_list.end());
        //   is error on Linux, so I use iterator...
        
        this->paramVector.push_back( tmp_vec );
    }
    
}


//-----------------------------------------------------------------------------
// Model File                      paramSizeVector
//   AAA : a_1, a_2, a_3, a_4        4
//   BBB : b_1, b_2                  2
//   CCC : c_1, c_2                  2
//
//  rbTableSize = 4 + 2 + 2 = 8
//
//  round robin table
//       4   2  2
//     +----+--+--+
//    4|xxxx|..|..|  x : always zero because of same parameter
//     |xxxx|..|..|
//     |xxxx|..|..|  . : check point.
//     |xxxx|..|..|       left-under triangle is not checked
//     +----+--+--+       because it is same data with right upper area.
//    2|    |xx|..|
//     |    |xx|..|
//     +----+--+--+
//    2|    |  |xx|
//     |    |  |xx|
//     +----+--+--+
//
void  Matrix::MakeRoundRobin()
{
    int rbTableSize = 0;
    
    for(int i=0; i<this->paramSizeVector.size(); i++){
        rbTableSize += this->paramSizeVector[i];
    }
    
   // assert(rbTableSize != 0);
    if(rbTableSize == 0 ){ return; }
    
    // make round robin table
    this->RoundRobinTable.resize(rbTableSize); 
    for(int i=0; i<rbTableSize; i++){
        this->RoundRobinTable[i].resize(rbTableSize); 
    }

    int RowOrg = 0;
    int vectorSize;
    if(this->paramVector.size() == 0 ) {
        this->MakeParamSizeVector();
    }
    vectorSize = this->paramVector.size() ;

    for(int i=0; i<vectorSize;  i++){
        
        int ColOrg = 0;
        for(int j=0; j<vectorSize; j++){
            
            for(int p1=0; p1<this->paramSizeVector[i]; p1++){
            for(int p2=0; p2<this->paramSizeVector[j]; p2++){
    
                vector< vector<string> > param;
                param.resize(2);
                param[0].resize(2);
                param[1].resize(2);
                
                param[0][0] = Util::Int2String(i);
                param[1][0] = Util::Int2String(j);
                param[0][1] = this->paramVector[i][p1];
                param[1][1] = this->paramVector[j][p2];
                
                string key = MakeHashKey(param);
                this->RoundRobinTable[RowOrg + p1][ColOrg + p2] =
                                      this->coverageVector[1][key]  ;
                              //        this->coverageHash2[key]  ;
            }
            }
        
            ColOrg += this->paramSizeVector[j];
        
        } // end of for(j)

        RowOrg += this->paramSizeVector[i];
        
    } // end of for(i)
    
    
}



//-----------------------------------------------------------------------------
//
// Model File                      paramSizeVector
//   AAA : a_1, a_2, a_3, a_4       4
//   BBB : b_1, b_2                 2
//   CCC : c_1, c_2                 2
//
//     
//  round robin table
//   ( paramCnt=3 )
//
//    i= 0   1  2          i=2,j=0
//     +----+--+--+         +--+
//  j=0|xxxx|  |..|         |..| check target small table
//     |xxxx|  |..|         |..|
//     |xxxx|  |..|         |..|   all value are same
//     |xxxx|  |..|         |..|   and not zero.
//     +----+--+--+         +--+
//    1|xxxx|xx|  |
//     |xxxx|xx|  |
//     +----+--+--+ 
//    2|xxxx|xx|xx|  x : not check area
//     |xxxx|xx|xx|
//     +----+--+--+
void  Matrix::CheckRoundRobin()
{
    
    int paramCnt = this->paramSizeVector.size();
    
    for(int i=1; i<paramCnt; i++){
        for(int j=0; j<i; j++){
            if(!CheckSmallTable(j,i)){
                return;  // error
            }
        }
    }
}
//-----------------------------------------------------------------------------
// for debug
void  Matrix::Information()
{
    cout << "## Matrix" << endl;
    for(int i=0; i<this->rowSize; i++){
        Util::PrintVector( this->data[i] );
    }
}
//-----------------------------------------------------------------------------
void  Matrix::ConfirmOA(ProgInfo *progInfo)
{
    if(this->paramSizeVector.size() == 0){
        MakeParamSizeVector();
    }
    
    if(progInfo->isTableCheck){ 
        Util::PrintHeader("");
        cout << "# test size = " << this->rowSize << endl ;
    }
    
    if(progInfo->isDebug || progInfo->isTableCheck || progInfo->ShowInfo){ 
        Util::PrintHeader("");
        for(int way=2; way <= progInfo->coverageWay; way++){
            if(this->paramSizeVector.size() >= way ){
                this->GetCoverage(way);
                this->PrintCoverage(way);
            }
        }
    }
    
    if(progInfo->isDebug ){ 
        // -debug only
        Util::PrintHeader("round-robin matrix between 2 parameters");
        this->MakeRoundRobin();
        this->CheckRoundRobin();
        Util::PrintMatrix(this->RoundRobinTable);
    }

    

    long Ln = this->GetLn();
    long J2 = this->GetJ2Optiomality();
    float ratio = ((float) Ln) / ((float) J2) ;

    // -d or -check or -show or (*)
    if(progInfo->isDebug  || progInfo->isTableCheck || 
       progInfo->ShowInfo || 
       (ratio != 1 && Util::StrIcmp(progInfo->algorithm, "ca"))
         // (*) ratio is not 1 , and algorithm is not CA 
      ){ 

        Util::PrintHeader("Confirm Array");
        cout << "# J2(d) : "  << J2     << endl;
        cout << "# L(n)  : "  << Ln     << endl;
        cout << "# ratio : "  << ratio  << endl; 
        if(ratio != 1) {
            cout << "# [X] this is not Orthogonal Array." << endl;
        }
    }

}



//-----------------------------------------------------------------------------
// Xu's article2002      J2(d)
long  Matrix::GetJ2Optiomality()
{
    long j2d=0;
    
    for(int i=0; i<this->rowSize; i++){
        for(int j=(i+1); j<this->rowSize; j++){
            
            long delta = GetKroneckerDelta(i,j);
            
            j2d +=  ( delta * delta ) ;
        }
    }
    
    return j2d;
}
//-----------------------------------------------------------------------------
// Xu's article2002  (1)
long  Matrix::GetKroneckerDelta(int i, int j)
{
    long delta=0;
    
    for(int k=0; k<this->colSize; k++){
         if(this->data[i][k] == this->data[j][k]){
            delta++;
        }
    }
    
    return delta;
}

//-----------------------------------------------------------------------------
// Xu's article2002  (2)    L(n)
// n is 1 origin
long  Matrix::GetLn()
{
    return Util::GetLn(this->paramSizeVector, this->rowSize, this->colSize);
}

//-----------------------------------------------------------------------------
// Xu's article2002  (4)
//
//  Sk : column C has Sk levels
//
//  caution :
//    column C is not added yet.
//
long  Matrix::GetJ2Plus(long J2, int Sk)
{
    long j2plus = J2;

    int N = this->rowSize;
    for(int i=0; i<N; i++){
        for(int j=(i+1); j<N; j++){
            
            // KroneckerDeltaC(i,j);
            long deltaC = (this->vecC[i] == this->vecC[j]) ? 1 : 0 ;
            
            // deltaC is either 0 or 1
            if(deltaC == 1){
                
                long delta  = GetKroneckerDelta(i,j);
                
                j2plus += (delta * 2)  ;
            }else{
                // zero 
            }
        }
    }
    
    j2plus = j2plus + ( N * ( (N / Sk) - 1 ) ) / 2;
    
    return j2plus;
}

//-----------------------------------------------------------------------------
// Xu's article2002  (5)
long  Matrix::GetDeltaAB(int a, int b)
{

    long delta=0;
    
    for(int j=0; j<this->rowSize; j++){
        if(j!=a && j!=b) {
            
            //  KroneckerDelta C(a, j) - KroneckerDelta C(b, j) ;
            int cVal = ((this->vecC[a] == this->vecC[j]) ? 1 : 0) -
                       ((this->vecC[b] == this->vecC[j]) ? 1 : 0)   ;
                         
            // cVal = 0, or 1 or -1
            
            long tmp=0 ;
            
            //  KroneckerDelta d(a, j) - KroneckerDelta d(b, j) ;
            
            
            if(cVal == 1) { 
                tmp = (GetKroneckerDelta(a,j) - GetKroneckerDelta(b,j));
            }else if(cVal == -1) { 
                tmp = (GetKroneckerDelta(b,j) - GetKroneckerDelta(a,j));
            }
            
            delta = delta + tmp;
        }
    }
    
    return delta;
}




//-----------------------------------------------------------------------------
// private method
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  check a small able in the RoundRobinTable
//
//  boxNum  = 0    1  2       
//  colPos  = 0    4  6 
//           +----+--+--+     if input boxNum==2
//           |    |  |x |          then colPos  = 6
//           |    |  |  |               colSize = 2
//           :    :  :  :       
//
bool  Matrix::CheckSmallTable(int boxNum1, int boxNum2)
{
    int rowSize = this->paramSizeVector[boxNum1];
    int colSize = this->paramSizeVector[boxNum2];
    
    int rowPos = BoxNum2Pos(boxNum1);
    int colPos = BoxNum2Pos(boxNum2);
    

    // left-top value of small table
    int base_value = this->RoundRobinTable[rowPos][colPos];
    
    for(int i=0; i<rowSize; i++){
        for(int j=0; j<colSize; j++){
            int value = this->RoundRobinTable[rowPos+i][colPos+j];
            if(value==0){
                //cout << "# [X] value must not be zero." << endl;
                return false;
            }    
            if(value!=base_value){
                //cout << "# [X] all value must be same in small matrix." << endl;
                return false;
            }
        }
    }
    
    return true;
}
//-----------------------------------------------------------------------------
//
//  boxNum = 0    1  2       
//  pos =    0123 45 67
//          +----+--+--+     if input boxNum==2
//          |    |  |x |       then return 6
//          |    |  |  |       
//          |    |  |  |       
//          |    |  |  |       
//          +----+--+--+       
//          |    |  |  |
//          |    |  |  |
//          +----+--+--+
//          |    |  |  |
//          |    |  |  |
//          +----+--+--+
//
int  Matrix::BoxNum2Pos(int boxNum)
{
    int pos=0;
    
    for(int i=0; i<boxNum; i++){
        pos += this->paramSizeVector[i];
    }
    
    return pos;
}

//-----------------------------------------------------------------------------
// example.
//   level=2
//     Model File                      
//       AAA : a_1, a_2, a_3, a_4        (0,0) (0,1) (0,2) (0,3) 
//       BBB : b_1, b_2                  (1,0) (1,1)
//       CCC : c_1, c_2                  (2,0) (2,1)
//
//   key of "a_3" and "b_2"                                    
//     input param   (0,2)  (1,1)    
//     return key     "0->2,1->1"
//
//
// example.
//   level=3
//     inputparam    (1,2)  (3,4)  (5,6)   
//     return key    "1->2,3->4,5->6"
//
string  Matrix::MakeHashKey(vector< vector<string> > param)
{
    
    string key="";
    
    for(int i=0; i<param.size(); i++){
        if(key!=""){
            key += ",";
        }
        key = key + param[i][0] + "->" + param[i][1] ;
    }
    
    return key;
}


//-----------------------------------------------------------------------------
void  Matrix::PrintCoverage(int strength)
{
    int pos = strength - 1; // element position in coverageVector

    cout << "## " << strength << "-way coverage"  << endl;
    
    cout << "# coverage rate        : "
         << this->coverageRate[pos] << " %" << endl;
         
    cout << "# " << strength << "-way combinations   : " 
         << this->coverageAllCount[pos]     << endl;
         
    cout << "# covered combinations : " 
         << this->coverageCovered[pos]      << endl;

    if(strength==2 && this->coverageRate[pos] != 100){
        cout << "# [X] " << strength << "-way coverage is not 100%." << endl;
    }
}
//-----------------------------------------------------------------------------
//
//  strength=2 ---> get 2-way coverage
//           3 ---> get 3-way coverage
//
void  Matrix::GetCoverage(int strength)
{
    this->coverageVector.resize(strength);
    
    int pos = strength - 1; // element position in coverageVector
    
    this->FactorAllCount = 0;
    vector<int> CA;
    
    this->SetCoverageVector(CA, 0, 1, strength);

    
    float rate = (float) this->coverageVector[pos].size() / (float) FactorAllCount ;
    
    this->coverageRate.resize(strength);
    this->coverageAllCount.resize(strength);
    this->coverageCovered.resize(strength);
    
    this->coverageRate[pos]     =  (int) (rate * 100);
    this->coverageAllCount[pos] =  FactorAllCount ;
    this->coverageCovered[pos]  =  this->coverageVector[pos].size() ;
    
    
}
//-----------------------------------------------------------------------------
//
// coverageVector[0] <-- no data 
// coverageVector[1] <-- 2-way coverage data (map<string, long>)
// coverageVector[2] <-- 3-way coverage data
//      :                         :
//
//
// 2-way example. 
//   coverageVector[1] <-- 2-way coverage data (map<string, long>)
//
//    A:a1,a2
//    B:b1,b2
//    C:c1,c2
//    
//    parameter (A,B,C) = (0,1,2)
//    
//    paramCombi (0,1) (0,2) (1,2) <--- (A,B)(A,C)(B,C) parameter combinations
//    
//    matrix example
//      A B C
//      0,0,0         
//      0,1,1
//      1,0,1
//      1,1,0
//    
//    (A,B)coverage
//      key=0->0,1->0    A=0,B=0
//          0->0,1->1    A=0,B=1
//          0->1,1->0    A=1,B=0
//          0->1,1->1    A=1,B=1
//           :
//
//      coverageVector[1]['0->0,1->0'] = 1
//      coverageVector[1]['0->0,1->1'] = 1 <--existed combination counts
//           :
//
void  Matrix::SetCoverageVector(vector<int> paramCombi, int start, int nest, int strength)
{
    // Util::PrintVector(paramCombi);
            
    int pos = strength - 1; // element position in coverageVector
    string key="";  // hash key

    // if reach strength max, then set coverage
    if(nest>strength){ 
        int count = 1;
        for(int j=0; j<paramCombi.size(); j++){
            count = count *  this->paramSizeVector[ paramCombi[j] ];
        }
        this->FactorAllCount += count;
        
        for(int row=0; row<this->rowSize; row++){
            vector< vector<string> > param;

            param.resize(strength);
            for(int j=0; j<paramCombi.size(); j++){
                param[j].push_back( Util::Int2String(paramCombi[j]) ) ;
                param[j].push_back(this->Get(row, paramCombi[j])) ;
            }
            
            key = MakeHashKey(param);
            this->coverageVector[pos][key]++ ;  // set coverage count
            
        } // end of for(row)

        return; 
    }
    nest++;

    // check parameters each columns
    for(int col1=start; col1 < (this->colSize); col1++){
        paramCombi.push_back(col1);

        // recursive call until strength max
        SetCoverageVector(paramCombi, col1+1, nest, strength); 

        paramCombi.pop_back();
    }

}

//-----------------------------------------------------------------------------
