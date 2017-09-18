// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include <string> 
#include <vector> 

using namespace std;

class TestParameter
{
  public:
    //Contructor
    TestParameter();

    // member
    string          paramName;
    vector<string>  paramValues;
    
    int             orignal_pos;


// Model File
//   AAA:a_1,a_2,a_3,a_4   
//   BBB:b_1,b_2
//   CCC:c_1,c_2
//
//  one TestParameter
//    AAA:a_1,a_2,a_3,a_4
//     
//    AAA <== paramName 
//
//    a_1,a_2,a_3,a_4  <== paramValues 
//
//     a_1  <== paramValues[0]   
//     a_2  <== paramValues[1]   
//     a_3  <== paramValues[2]   
//     a_4  <== paramValues[3]   
//

};

