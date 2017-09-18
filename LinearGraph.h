// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef LINEARGRAPH_H
#define LINEARGRAPH_H


#include <string> 
#include <iostream> 
//#include <sstream> 
#include <list> 
#include <vector> 

#include "BaseOrthogonalArray.h"


using namespace std;


class LinearGraph
{
  public:
  
    //Contructor
    LinearGraph(BaseOrthogonalArray *);

    // member
    int baseLevel;
    
    // method
    vector<int>   GetCols(int);
    void          ClearUsedColumn();
    
  private:
    // member
    int Size;
    vector<string>  elementList;
    vector<bool>    usedColumn;
    
    // method
    vector<int>  GetColsByLogic(int);
    bool         isValidElement(vector<string> , string );
    string       MergeElementName(string , string );
    
};


#endif
