// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef ALGORITHMCA_H
#define ALGORITHMCA_H

#include "ProgInfo.h"
#include "ModelFile.h"
#include "BaseOrthogonalArray.h"
#include "LinearGraph.h"
#include "Algorithm.h"

using namespace std;


class AlgorithmCA : public Algorithm
{
  //---------------------------------------------------------------------------
  public:
    //Contructor
    AlgorithmCA(ProgInfo *, ModelFile *);

    // member

    // method
    Matrix          *GetMatrix();
    void             PrintStreamData();
    vector<int>      GetTestRecord();


  //---------------------------------------------------------------------------
  private:
    // member
    vector< vector<int> >  CA;
    
    // method
    void   MakeCA();
    int    NumberPairsCaptured(int *, int, int **);
};


#endif
