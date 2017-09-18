// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.


//----------------------------------------------------------------------------
//
// main function of the tool
//
#include <time.h>

#include "ArrayFile.h"
#include "ModelFile.h"
#include "ProgInfo.h"
#include "OrthogonalArray.h"

int main(int argc, char **argv)
{
    ProgInfo          *progInfo;
    OrthogonalArray   *oa;

    ModelFile         *modelFile;
    ArrayFile         *arrayFile;

    //---------------------------------------------------
    // adhoc GetOpt()
    progInfo = new ProgInfo(argc, argv);
    if(!progInfo->isValid) {
        progInfo->usage();
        return -1;
    }

    clock_t start,end;
    if(progInfo->enableMeasuring){
        start = clock();
    }


    //---------------------------------------------------
    if(!progInfo->isTableCheck) {
        // normal construction of OA
        modelFile = new ModelFile(progInfo);
        if(!modelFile->isValid){
            return -1;
        }
        if(progInfo->baseLevel == 0) {
            progInfo->baseLevel = modelFile->GetBaseLevel();
        }
        
        oa =  new OrthogonalArray(progInfo);
        if(!oa->Generate(modelFile)){
            return -1;
        }
        
        // print Generated Array Information
        oa->Information();
        
    }else{
        // -check OA
        arrayFile = new ArrayFile(progInfo);
        if(!arrayFile->isValid){
            return -1;
        }
        
        oa =  new OrthogonalArray(progInfo);
        if(!oa->Generate(arrayFile)){
            return -1;
        }
        
    }

    
    // print 2 or 3-way coverage and J(2)
    oa->Confirm();

    if(!progInfo->isTableCheck) {
       oa->PrintResult();
    }
    
    if(progInfo->enableMeasuring){
        end = clock();
        cout << "## elapsed time : " 
             << ( (double)(end-start)/CLOCKS_PER_SEC )
             << " secs" << endl;
    }

    return 0;
}

//
//----------------------------------------------------------------------------
