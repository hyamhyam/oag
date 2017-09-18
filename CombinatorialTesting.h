// Copyright 2011, 2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef COMBINATORIALTESTING_H
#define COMBINATORIALTESTING_H

#include <iostream>
#include <string> 
#include <vector> 
#include <map> 

#include "ArrayFile.h"
#include "AlgorithmAll.h"
#include "AlgorithmCA.h"
#include "AlgorithmBigCA.h"
#include "ModelFile.h"
#include "ProgInfo.h"
#include "OrthogonalArray.h"

using namespace std;

class __declspec(dllexport) CombinatorialTesting
{
  public:
    //Contructor
    CombinatorialTesting(int, char **);
    
    // method
    bool   GetTestRecord();
    
    // member
    int             testCount;
    vector<string>  testRecord;


  //-----------------------------------------------------------------
  private:
    // member

    OrthogonalArray   *oa;

    ProgInfo          *progInfo;
    ModelFile         *modelFile;
    ArrayFile         *arrayFile;

    AlgorithmAll      *all;
    AlgorithmCA       *ca;
    AlgorithmBigCA    *bigca;
};



#endif
