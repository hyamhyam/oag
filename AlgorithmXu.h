// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef ALGORITHMXU_H
#define ALGORITHMXU_H


#include <string> 
//#include <sstream>  // gcc v3.0 later
#include <list> 
#include <map> 

#include "ArrayFile.h"
#include "ModelFile.h"

using namespace std;


class AlgorithmXu
{
  //---------------------------------------------------------------------------
  public:
    //Contructor
    AlgorithmXu(ProgInfo  *);

    // member
    ProgInfo      *progInfo;

    Matrix *D; // result matrix
    
//    vector<string> vecC; // temporary work
    
    // method
    Matrix *Generate(ModelFile *);
    

  //---------------------------------------------------------------------------
  private:
    // member
    
    long  base_J2;   // J2(d)
    
    long  best_deltaAB;  // temporary work

    vector<long>   LkVector;  // L(k) vector
    
    // method
    void ComputeLk(list<int>);

    void AddColumn(int, int, long);
    void CheckDeltaAB(int);


};


#endif
