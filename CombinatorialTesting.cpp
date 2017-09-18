// Copyright 2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.


//----------------------------------------------------------------------------
#include "CombinatorialTesting.h"
using namespace std;

// this macro must be used after new ProgInfo().
#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }

//-----------------------------------------------------------------------------
// Constructor
CombinatorialTesting::CombinatorialTesting(int argc, char **argv) {
    
    
    this->testCount = 0;
    
    this->progInfo   = new ProgInfo();

    this->progInfo->inputDataFile = argv[1];
    if(argc == 4){
        this->progInfo->algorithm = argv[3] ;    
    }else{
        this->progInfo->algorithm = "GF"; // default
        
        // set default option
        this->progInfo->baseLevel              = 2;
        this->progInfo->noD                    = true;
        this->progInfo->enableComplementation  = true;
    }
    
    
    // for debug
    //    this->progInfo->isTrace = true;
    TRACER("CombinatorialTesting::Constructor start");

    
    
    this->modelFile = new ModelFile(this->progInfo);
    if(!modelFile->isValid){ 
        TRACER("return false after new ModelFile()");
        return ;           
    }                        
    this->modelFile->MakeMaxCombinationCnt() ;
    
    
    if(!Util::StrIcmp(this->progInfo->algorithm, "all") ){
        this->progInfo->isStream  = true;
        this->all = new AlgorithmAll(this->progInfo, this->modelFile) ;
        this->all->modelFile   = this->modelFile;
        
        return;
    }

    if(!Util::StrIcmp(this->progInfo->algorithm, "CA") ){
        this->progInfo->isStream  = true;
        this->ca = new AlgorithmCA(this->progInfo, this->modelFile) ;
        this->ca->modelFile   = this->modelFile;
        
        return;
    }

    if(!Util::StrIcmp(this->progInfo->algorithm, "BigCA") ){
        this->progInfo->isStream  = false;
        this->bigca = new AlgorithmBigCA(this->progInfo, this->modelFile) ;
        this->bigca->modelFile   = this->modelFile;
        
        return;
    }

    if(!Util::StrIcmp(this->progInfo->algorithm, "GF") ){
        this->oa = new OrthogonalArray(this->progInfo);
        
        if(!this->oa->Generate(this->modelFile)){
            TRACER("return false after oa->Generate")
            return ;
        }
    }
}

//-----------------------------------------------------------------------------
//
// public method
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
bool  __declspec(dllexport)  CombinatorialTesting::GetTestRecord()
{
    

    // if counter end, then exit.
    if(!Util::StrIcmp(this->progInfo->algorithm, "GF")){
        if(this->testCount >=  this->oa->resultArray.size() ){
            return false;
        }
    }

    if(!Util::StrIcmp(this->progInfo->algorithm, "all") ){
        if(this->modelFile->maxCombinationCnt < 0 ||
           this->testCount >=  this->modelFile->maxCombinationCnt ){
            return false;
        }
    }

    if(!Util::StrIcmp(this->progInfo->algorithm, "CA")    ){
        if(this->modelFile->maxCombinationCnt < 0 ||
           this->testCount >=  this->ca->testSize ){
            return false;
        }
    }
    if(!Util::StrIcmp(this->progInfo->algorithm, "BigCA")     ){
        if(this->bigca->recordCount == 0 ){
            this->bigca->MakeBigCA();
        }
        if(this->modelFile->maxCombinationCnt < 0 ||
           this->testCount >=  this->bigca->testSize ){
            return false;
        }
    }


    // set Test data to testRecord
    if(!Util::StrIcmp(this->progInfo->algorithm, "GF")){
        this->testRecord =  this->oa->resultArray[this->testCount];
    }
    
    if(!Util::StrIcmp(this->progInfo->algorithm, "all")){
        vector<string> temp;
        temp = this->all->GetTestRecord();
        this->testRecord.resize(0);
        for(int i=0; i<temp.size(); i++){
            int val = atoi( temp[i].c_str() );
            TestParameter *testParam = this->modelFile->testParamRecords[i];
            this->testRecord.push_back(testParam->paramValues[val]);
        }
        
    }

    if(!Util::StrIcmp(this->progInfo->algorithm, "CA")){
        vector<int> temp;
        temp = this->ca->GetTestRecord();
        this->testRecord = this->modelFile->Element2Value(temp);
    }

    if(!Util::StrIcmp(this->progInfo->algorithm, "BigCA")){
        vector<int> temp;
        temp = this->bigca->GetTestRecord();
        this->testRecord = this->modelFile->Element2Value(temp);
    }
    
    
    
    this->testCount++;

    return true;
}

//-----------------------------------------------------------------------------
//
// private method
//
//-----------------------------------------------------------------------------


//
//----------------------------------------------------------------------------
