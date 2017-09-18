// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef ALGORITHMBIGCA_H
#define ALGORITHMBIGCA_H

#include "ProgInfo.h"
#include "ModelFile.h"
#include "BaseOrthogonalArray.h"
#include "LinearGraph.h"
#include "Algorithm.h"

using namespace std;


class AlgorithmBigCA  : public Algorithm
{
  //---------------------------------------------------------------------------
  public:
    //Contructor
    AlgorithmBigCA(ProgInfo  *, ModelFile *);

    // member


    // method
    Matrix          *GetMatrix();
    void             PrintStreamData();
    vector<int>      GetTestRecord();
    void             MakeBigCA();


  //---------------------------------------------------------------------------
  private:
    // member
    vector< vector <int> > CA ;

    vector< vector <int> > paramCombiVector ;
    vector<int>            paramCombi ;
    
    // method
    void   MakeParamCombi(int, int);
    void   MakeSubCombination(vector<int>);

};


#endif
