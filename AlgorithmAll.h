// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef ALGORITHMALL_H
#define ALGORITHMALL_H

#include "Algorithm.h"

#include "ProgInfo.h"
#include "ModelFile.h"
#include "BaseOrthogonalArray.h"
#include "LinearGraph.h"

using namespace std;


class AlgorithmAll : public Algorithm
{
  //---------------------------------------------------------------------------
  public:
    //Contructor
    AlgorithmAll(ProgInfo *, ModelFile *) ;

    // member


    // method
    Matrix          *GetMatrix();
    void             PrintStreamData();
    vector<string>   GetTestRecord();


  //---------------------------------------------------------------------------
  private:
    // member
    
    // method

};


#endif
