// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include "AlgorithmBigCA.h"

#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }


// this source is under construction. (hyam)


//-----------------------------------------------------------------------------
// Constructor
AlgorithmBigCA::AlgorithmBigCA(ProgInfo *progInfo, ModelFile *modelFile)
    : Algorithm(progInfo, modelFile)
{
    // NOP.  use super class constructor
}

//-----------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------
Matrix *AlgorithmBigCA::GetMatrix()
{
    this->matrix = new Matrix();

    if(this->modelFile->maxCombinationCnt >= 1000000   ){
        // temporary restriction size (hyam)
        cout << "# [X] too large array size(>=1,000,000)(BigCA)" <<endl; 
        cout << "# [X] maxCombinationCnt = "
             << this->modelFile->maxCombinationCnt <<endl; 
        return 0;
    }

    if(this->modelFile->totalParameterValuesCnt <= 0 ){
        // temporary restriction size (hyam)
        cout << "# [X] too large array size(AlgorithmCA)" <<endl; 
        return 0;
    }
    
    
    MakeBigCA(); 
    
    for(long k=0; k<CA.size(); k++){
        this->matrix->AddRow(CA[k]);
    }

    return this->matrix;

}
//-----------------------------------------------------------------------------
void  AlgorithmBigCA::PrintStreamData()
{

    if(this->modelFile->totalParameterValuesCnt <= 0 ){
        // temporary restriction size (hyam)
        cout << "# [X] too large array size(AlgorithmCA)" <<endl; 
        return ;
    }
    
    this->modelFile->PrintAllParamName();
    
    MakeBigCA(); // including stream output
    
 //   for(long k=0; k<CA.size(); k++){
 //       string delim = this->modelFile->progInfo->outDelimiter ;
 //       vector<string> result = this->modelFile->Element2Value(CA[k]);
 //       Util::PrintVector(result, delim);
 //   }

    return ;
}
//-----------------------------------------------------------------------------
vector<int> AlgorithmBigCA::GetTestRecord(){

    if(this->recordCount == 0 ){
        MakeBigCA();
    }

    vector<int> vec_col = CA[this->recordCount]   ;
    
    this->recordCount++;
    return vec_col ;
}
//-----------------------------------------------------------------------------
// recCnt = 0,1,2, ...  maxCombinationCnt - 1
void  AlgorithmBigCA::MakeBigCA(){
    
    
    MakeParamCombi(0, 1);
    
    //Util::PrintMatrix(paramCombiVector);
    
    for(int i=0; i< paramCombiVector.size(); i++){
        MakeSubCombination(paramCombiVector[i]);
    }
    
    this->testSize =  this->CA.size();
}


//-----------------------------------------------------------------------------
// private
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// make parameter combination with strength
//
// ex.
//  paramCount = 4
//  strength = 2
//
//  paramCombi=(0,1)(0,2)(1,2)...
//  paramCombiVector=((0,1)(0,2)(1,2)...)
// 
void AlgorithmBigCA::MakeParamCombi(int start, int nest)
{
    if(nest > this->modelFile->progInfo->strength){
        this->paramCombiVector.push_back(this->paramCombi);
        return ;
    }
    nest++;
    
    for(int i=start; i<this->modelFile->paramCount; i++) {
        this->paramCombi.push_back(i);
        MakeParamCombi(i+1, nest);
        this->paramCombi.pop_back();
    }
    
    return  ;
}
//-----------------------------------------------------------------------------
// ex.
//   A:a1,a2,a3    
//   B:b1,b2
//   C:c1,c2       
//              
//        paramCombi=(0, 1)
//                    |  |
//                    |  |
//                    V  V
//   paramSizeVector=(3, 2, 2)
//   allCombiCnt = 3*2
//
void AlgorithmBigCA::MakeSubCombination(vector<int>  paramCombi)
{
    int allCombiCnt=1;
    for(int i=0;i<paramCombi.size(); i++){
        allCombiCnt *= this->modelFile->paramSizeVector[paramCombi[i]];
    }

    //cout << "allCombiCnt=" << allCombiCnt << endl;

    for(int i=0; i<allCombiCnt; i++){
        long  val=0;
        long  divider=1;
        
        vector<int> tmpRec;
        tmpRec.resize(this->modelFile->paramCount);
        
        // set random value to all columns at first
        for(int j=0; j< this->modelFile->paramCount; j++){
            int paramSize = this->modelFile->paramSizeVector[j];
            tmpRec[j] = (int)(rand()*(paramSize)/(1.0+RAND_MAX));
        }
        
        // set combination into the position defined by paramCombi
        for(int k=0; k<paramCombi.size(); k++){
            int paramSize = this->modelFile->paramSizeVector[paramCombi[k]];
            long cnt = i / divider;
            val = cnt % paramSize  ;
            
            divider *= paramSize ;
            
            // cout << val << ", " ;
            
            tmpRec[paramCombi[k]] = val;
        }
        
        if(this->modelFile->progInfo->isStream){
            string delim = this->modelFile->progInfo->outDelimiter ;
            vector<string> result = this->modelFile->Element2Value(tmpRec);
            Util::PrintVector(result, delim);
        }else{
            this->CA.push_back(tmpRec);
        }
    }
   
}
//-----------------------------------------------------------------------------
