// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef MATRIX_H
#define MATRIX_H


#include <string> 
#include <iostream> 
#include <list> 
#include <vector> 
#include <map> 

#include <cstdlib>

#include "ProgInfo.h"


using namespace std;


class Matrix
{
  public:
  
    //Contructor
    Matrix();
    Matrix(int, int);
    Matrix(vector< vector<string> >);

    // member
    
    int rowSize;
    int colSize;
    
    // type is string, but element must be int
    //  ("1", "3", "0", ...)
    vector< vector<string> >    data;
    
    vector<int>                paramSizeVector;
    
    vector< vector<string> >   paramVector;

    vector< vector<int> >      RoundRobinTable;    

    long J2Optiomality;
    long LnValue;

    vector<string> vecC; // temporary work
       
    
    // method
    void    Set(int, int, string);
    void    AddRow(vector<string>);
    void    AddRow(vector<int>);
    void    AddCol(vector<string>);
    void    ReplaceCol(int, vector<string>);
    string  Get(int, int);
    int     GetInt(int, int);
    
    vector<string>   GetRow(int);
    list<string>     GetCol(int);
    
    void  Information();
    void  SetColumnData(int, vector<string>);
    
    
    void    MakeParamSizeVector();
    
    
    
    void    ConfirmOA(ProgInfo *);
    
    void       MakeRoundRobin();
    void       CheckRoundRobin();
    
    // for Algorithm Xu
    long GetJ2Optiomality();
    long GetLn();
    long GetKroneckerDelta(int, int);
    long GetJ2Plus(long, int);
    long GetDeltaAB(int, int);

  //--------------------------------------------------------------------------
  private:
    // member
    vector< map<string, long> > coverageVector;

    long FactorAllCount;
    vector<int> coverageRate;
    vector<int> coverageAllCount;
    vector<int> coverageCovered;
    
    
    // method
    string   MakeHashKey(vector< vector<string> > param);
    bool     CheckSmallTable(int,int);
    int      BoxNum2Pos(int);
    vector<string>  ConvKeyNum(int);

    void    PrintCoverage(int);
    void    GetCoverage(int);
    void    SetCoverageVector(vector<int>, int, int, int);

    
};

#endif
