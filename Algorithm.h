// Copyright 2011, 2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "ProgInfo.h"
#include "ModelFile.h"
#include "BaseOrthogonalArray.h"
#include "LinearGraph.h"

using namespace std;


class Algorithm
{
  //---------------------------------------------------------------------------
  public:
    //Contructor
    Algorithm(ProgInfo *, ModelFile *);

    // member
    ProgInfo      *progInfo;
    Matrix        *matrix; //  Array data

    ModelFile     *modelFile;
    int            recordCount;
    int            testSize;


    // method
    Matrix *Generate(ModelFile *);


  //---------------------------------------------------------------------------
  private:
    // member
    
    // method

};


#endif
