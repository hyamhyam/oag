// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef UTIL_H
#define UTIL_H

#include <string> 
#include <list> 
#include <iostream> 
#include <cassert>
#include <vector> 
#include <string.h>
#include <stdio.h>

#include "Matrix.h"
#include "ExString.h"

using namespace std;

class Util
{
  public:
    //Contructor
    Util();

    // static method
    static int  Log(int , int);

    static int     PowerCeil(int, int);
    static int     GFadd(int, int, int, int);
    static int     GetGreatestCommonDiviser(int, int);
    
    static int     GetMixedArraySize(list<int>);
    static int     GetLeastCommonMultiple(int, int);
    static int     GetLeastCommonMultiple(list<int>);
    static int     Max(list<int>);
    static int     Min(list<int>);

    static string  Num2Alpha(int);
    
    static string  Int2String(int);
    
    static string  TrimString(string);
    static string  TrimComment(string);

    static string  SortStringChar(string);
    static string  ReplaceString(string, string, string);
    
    static vector<string>  Split(string, string);

    static int StrIcmp(const char *, const char *);
    static int StrIcmp(string, string);

     
    // for debug infomation
    static void    PrintHeader(string); 
    
    static void    PrintMatrix(Matrix *); 
    static void    PrintMatrix(vector< vector<int> >); 
    static void    PrintMatrix(vector< vector<string> >); 
    static void    PrintMatrix(vector< vector<int> >, string); 
    static void    PrintMatrix(vector< vector<string> >, string); 
    
    static void    PrintVector(vector<int>);    
    static void    PrintVector(vector<string>); 
    static void    PrintVector(vector<int>, string); 
    static void    PrintVector(vector<string>, string); 

    static void    PrintList(list<int>);    
    static void    PrintList(list<string>);    
    
    static string    GetListName(list<int>);    

    //  for J2-Optimality
    static long GetLn(vector<int>, int, int);


};

#endif
