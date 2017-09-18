// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include "AlgorithmAll.h"

#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }

//-----------------------------------------------------------------------------
// Constructor
AlgorithmAll::AlgorithmAll(ProgInfo *progInfo, ModelFile *modelFile)
    : Algorithm(progInfo, modelFile)
{
    // NOP.  use super class constructor
}

//-----------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------
Matrix *AlgorithmAll::GetMatrix()
{
    this->matrix = new Matrix();

    if(this->modelFile->maxCombinationCnt >= 1000000   ){
        // temporary restriction size (hyam)
        cout << "# [X] too large array size(>=1,000,000)(all)" <<endl; 
        cout << "# [X] maxCombinationCnt = "
             << this->modelFile->maxCombinationCnt <<endl; 
        return 0;
    }
    
    if(this->modelFile->maxCombinationCnt <= 0      ){
        // temporary restriction size (hyam)
        cout << "# [X] too large array size(AlgorithmAll)" <<endl; 
        return 0;
    }
    
    
    while(this->recordCount < this->modelFile->maxCombinationCnt){
        
        vector<string> vec_col = GetTestRecord();
        
        this->matrix->AddRow(vec_col);
    }
    
    return this->matrix;

}
//-----------------------------------------------------------------------------
void AlgorithmAll::PrintStreamData()
{

    if(this->modelFile->maxCombinationCnt <= 0      ){
        // temporary restriction size (hyam)
        cout << "# [X] too large array size(AlgorithmAll)" <<endl; 
        return ;
    }
    
    this->modelFile->PrintAllParamName();
    
    
    while(this->recordCount<this->modelFile->maxCombinationCnt){
        
        vector<string> vec_col = GetTestRecord();
        
        string delim = this->modelFile->progInfo->outDelimiter ;
        vector<string> result = this->modelFile->Element2Value(vec_col);
        Util::PrintVector(result, delim);
    }
    
    return ;

}

//-----------------------------------------------------------------------------
// recCnt = 0,1,2, ...  maxCombinationCnt - 1
vector<string> AlgorithmAll::GetTestRecord(){
    
    
    long val=0;
    vector<string> vec_col;

    long  divider=1;

    list<int>::iterator ite = this->modelFile->paramSizeList.begin(); 

    while( ite != this->modelFile->paramSizeList.end() ) {
    
        int paramSize = *ite;
        
        long cnt = this->recordCount  / divider;

        val = cnt % paramSize  ;
        
        divider *= paramSize ;
        
        vec_col.push_back(Util::Int2String(val));

        ++ite; 
    }
    
    // count up Test Run Record Count
    this->recordCount++;


    return  vec_col;
}


//-----------------------------------------------------------------------------
// private
//-----------------------------------------------------------------------------
