// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef ORTHOGONALARRAY_H
#define ORTHOGONALARRAY_H


#include <string> 
//#include <sstream>  // gcc v3.0 later
#include <list> 
#include <map> 

#include "LinearGraph.h"
#include "ArrayFile.h"
#include "ModelFile.h"
#include "BaseOrthogonalArray.h"
#include "Algorithm.h"

using namespace std;


class OrthogonalArray
{
  //---------------------------------------------------------------------------
  public:
    //Contructor
    OrthogonalArray(ProgInfo  *);

    // member
    ProgInfo      *progInfo;
    ArrayFile     *arrayFile;
    ModelFile     *modelFile;
    
    
    int          paramCount; 
    int          baseLevel; // default=2  
    
    bool         isValid;
     
    int        Size; // nn for Lnn (L8, L16, L32 ... ) (baseLevel=2)
                     //            (L9, L27, L81 ... ) (baseLevel=3)
                     //             :                         :
    
    
    BaseOrthogonalArray      *baseArray;
//    LinearGraph              *linearGraph;

    vector<string>            elementList;

    vector< vector<string> >  resultArray ;
    vector< vector<string> >  resultArrayWithConstraints ;

    Matrix   *matrix; // Orthogonal Array data

    // method
    bool  Generate(ModelFile *);
    bool  Generate(ArrayFile *);

    void  Information(); 
    void  Confirm();
    void  PrintResult();  
    
    

  //---------------------------------------------------------------------------
  private:
    // member
    bool         isDebug;
    bool         isTableCheck;     
    bool         ShowInfo;

    
    // method
    bool       Algorithm();
    bool       MakeAll();
    bool       MakeBigCA();
    bool       MakeCA();
    bool       MakeGF();
    bool       MakeRandom();
    bool       MakeXu();

    bool       Make2ParamArray();

    
    

    void       SetItemData();

    bool       isValitRecord(vector<string>);
    bool       CheckConstraint(vector<string>, string, string, string);

    int        Name2Num(string);
    void       ChangeColumn();
    

};


#endif
