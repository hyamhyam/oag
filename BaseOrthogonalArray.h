// Copyright 2011, 2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef BASEORTHOGONALARRAY_H
#define BASEORTHOGONALARRAY_H


#include <string> 
//#include <sstream> 
#include <list> 
#include <cassert>
 
#include "ModelFile.h"
#include "Matrix.h"

using namespace std;

class BaseOrthogonalArray
{
  
  public:
  
    //Contructor
    BaseOrthogonalArray();
    BaseOrthogonalArray(int, int);
    
    // member
    int baseLevel;
    int Size;
    
    Matrix  *matrix;
    
    // ex.
    //   level=2   a, b, ab, c, ab, bc, abc ...
    //   level=3    ... aabbc ...
    vector<string>  elementList;
    
    // method
    vector<string>  GetJoinedColumns(vector<int>);

  private:
   
    
    // member
    
    // if final matrix is L16,
    // L2,L4,L8 are saved in baseMatrixList. {L2,L4,L8}
    vector< Matrix *>  baseMatrixList;
    
    // method
    Matrix            *Increase(Matrix *);
    vector<string>     MakeElementList(int, bool);
    string             TrimData(vector<string>);
};


#endif
