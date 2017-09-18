// Copyright 2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef ALGORITHMRANDOM_H
#define ALGORITHMRANDOM_H

#include <stdlib.h>

#include "ProgInfo.h"
#include "ModelFile.h"
#include "BaseOrthogonalArray.h"
#include "LinearGraph.h"

using namespace std;


class AlgorithmRandom
{
  //---------------------------------------------------------------------------
  public:
    //Contructor
    AlgorithmRandom(ProgInfo  *);

    // member
    ProgInfo      *progInfo;
    Matrix        *matrix; //  Array data


    // method
    Matrix *Generate(ModelFile *);
    vector<string> MakeRandomCombinationRecord(ModelFile *, long);


  //---------------------------------------------------------------------------
  private:
    // member

    // method

};


#endif
