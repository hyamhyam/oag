// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef MODELFILE_H
#define MODELFILE_H


#include <string> 
#include <list> 
#include <iostream> 
#include <fstream> 
#include <algorithm> 

#include "TestParameter.h"
#include "ExString.h"
#include "Constraint.h"
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

class ModelFile
{
  public:
    //Contructor
    ModelFile(ProgInfo *);

    // member
    ProgInfo  *progInfo;
    int        baseLevel;
    
    vector<TestParameter *>  testParamRecords;
    vector<Constraint    *>  constraintRecords;

    bool    isValid;
    bool    isDescending;
    bool    isDummyAdded; // dummy value complementation
    
    
    int           paramCount; 
    list<int>     paramSizeList;
    vector<int>   paramSizeVector;
        // if A : a1,a2,a3,a4
        //    B : b1,b2
        //    C : c1,c2
        // then paramCount=3 , paramSizeList   = (4,2,2) 
        //                     paramSizeVector = (4,2,2) 
    long maxCombinationCnt;        // 16 = 4*2*2
    long totalParameterValuesCnt;  // 8 = 4+2+2


    // method
    int         GetOASize();
    int         AddAllFactorLevel();
    int         MultiplicationTopTwoFactors();
    bool        Verify();
    bool        MakeMaxCombinationCnt();

    bool        Complement();
    
    void        SortParameterRecords();
    void        SortParameterRecordsByOriginalOrder();

    void        Information();
    void        Information(string);
    
    void        PrintAllParamName();
    
    
    static  bool CompareParamCount(const TestParameter *,
                                   const TestParameter * );
                                   
    static  bool CompareParamCountByOriginalOrder
                                  (const TestParameter *,
                                   const TestParameter * );

    int    GetBaseLevel();
    
    vector<string> Element2Value(vector<string>);
    vector<string> Element2Value(vector<int>);
    
  //--------------------------------------------------------------------
  private:
    // method
    bool   ReadAllRecords();
    bool   IsConstraints(string);
    
    vector<string> GetValues(string);
    
};



#endif
