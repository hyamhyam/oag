// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef ALGORITHMGF_H
#define ALGORITHMGF_H

#include "ProgInfo.h"
#include "ModelFile.h"
#include "BaseOrthogonalArray.h"
#include "LinearGraph.h"

using namespace std;


class AlgorithmGF
{
  //---------------------------------------------------------------------------
  public:
    //Contructor
    AlgorithmGF(ProgInfo  *);

    // member
    ProgInfo      *progInfo;
    Matrix        *matrix; // Orthogonal Array data
    int           Size;    // baseLevel

    BaseOrthogonalArray      *baseArray;
    LinearGraph              *linearGraph;

    vector<string>            elementList;

    // method
    Matrix *Generate(ModelFile *);


  //---------------------------------------------------------------------------
  private:
    // member

    // method
    bool       Try2MakeGF(ModelFile *, int);

    vector< vector<int> >  GetColsVector(vector<int>);

};


#endif
