// Copyright 2011, 2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef ARRAYFILE_H
#define ARRAYFILE_H


#include <string> 
#include <list> 
#include <iostream> 
#include <fstream> 
#include <algorithm> 

#include "ExString.h"
#include "Util.h"
#include "ProgInfo.h"

using namespace std;

//
//  AAA:a1,a2,a3,a4 
//  BBB:b1,b2
//  CCC:c1,c2
//
//    paramCount=3
//

class ArrayFile
{
  public:
    //Contructor
    ArrayFile(ProgInfo *);

    // member
    ProgInfo  *progInfo;

    bool    isValid;
    
    Matrix  *matrix;  // Checking Orthogonal Array data

    // method
    
    

  //--------------------------------------------------------------------
  private:
    // method
    bool   ReadOrthogonalArray();
    
//    string  TrimComment(string);
};



#endif
