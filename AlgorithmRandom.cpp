// Copyright 2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include <time.h>
#include "AlgorithmRandom.h"

#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }

//-----------------------------------------------------------------------------
// Constructor
AlgorithmRandom::AlgorithmRandom(ProgInfo *progInfo)
{
    
    this->progInfo   = progInfo;

    if(progInfo->TimeSeed){
        srand((unsigned int)time(NULL));
    }else{
        srand(13);
    }    

}

//-----------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------
Matrix *AlgorithmRandom::Generate(ModelFile *modelFile)
{
    if(!modelFile->progInfo->isStream){
        this->matrix = new Matrix();
    }

    if(modelFile->maxCombinationCnt >= 10000000 &&
       !modelFile->progInfo->isStream             ){
        // temporary restriction size (hyam)
        cout << "# [X] too large array size(>=10,000,000)(random)" <<endl; 
        cout << "# [X] maxCombinationCnt = "
             << modelFile->maxCombinationCnt <<endl; 
        return 0;
    }
    if(modelFile->maxCombinationCnt <= 0      ){
        // temporary restriction size (hyam)
        cout << "# [X] too large array size(AlgorithmRandom)" <<endl; 
        return 0;
    }
    
    if(modelFile->progInfo->isStream){
        modelFile->PrintAllParamName();
    }
    
    int max = 0;
    if(modelFile->progInfo->rowSize == 0){
        max = Util::GetMixedArraySize(modelFile->paramSizeList);
    }else{
        max = modelFile->progInfo->rowSize;
    }
    
    for(long i=0; i<max; i++){
        vector<string> vec_col;
        vec_col = MakeRandomCombinationRecord(modelFile, i);
        
        
        if(!modelFile->progInfo->isStream){
            this->matrix->AddRow(vec_col);
        }else{
            string delim = modelFile->progInfo->outDelimiter ;
            vector<string> result;
            for(int i=0; i<vec_col.size(); i++){
                int val = atoi( vec_col[i].c_str() );
                TestParameter *testParam = modelFile->testParamRecords[i];
                result.push_back(testParam->paramValues[val]);
            }
            Util::PrintVector(result, delim);
        }
    }
    
    if(modelFile->progInfo->isStream){
        return 0;
    }else{
        return this->matrix;
    }

}

//-----------------------------------------------------------------------------
// recCnt : not use
vector<string> AlgorithmRandom::MakeRandomCombinationRecord
                                       (ModelFile *modelFile, long recCnt){
    
    
    long val=0;
    vector<string> vec_col;

    list<int>::iterator ite = modelFile->paramSizeList.begin(); 
    
    while( ite != modelFile->paramSizeList.end() ) {
        int paramSize = *ite;
        
        // get random value 0 -- (paramSize - 1)
        
        val = (int)(rand()*(paramSize)/(1.0+RAND_MAX));
        
        
        vec_col.push_back(Util::Int2String(val));

        ++ite; 
    }

//cout <<"vec_col="; Util::PrintVector(vec_col);

    return  vec_col;
}


//-----------------------------------------------------------------------------
// private
//-----------------------------------------------------------------------------
