// Copyright 2011, 2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef  PROGINFO_H
#define  PROGINFO_H

#include <string> 
#include <iostream> 

using namespace std;

class ProgInfo
{
  protected:

  public:
    //Contructor
    ProgInfo(int, char**);
    ProgInfo();

    // member
    bool    isValid;
    string  inputDataFile;
    int     baseLevel;

    int     rowSize;
    
    bool    enableConstraint;
    bool    enableComplementation;
    bool    enableMeasuring ; // Measuring execution time
    bool    noD;
    
    int     tryCount; // try count in Xu's algorithm
    
    bool    isTrace;
    bool    isDebug;
    bool    isQuiet;
    bool    isTableCheck;
    bool    isStream;
    bool    isNoParamName;

    bool    ShowInfo;

//    bool    isCombinatorialTesting;

    string  inDelimiter;  // not use yet
    string  outDelimiter;

    string  algorithm;

    int     strength;  // strength of test array

    bool    TimeSeed; 
    
    int     coverageWay; 
    
    // method
    void usage();

  //--------------------------------------------------------------------------
  private:
    // member
    string programName;
    string version;
    
    // method
    //int STRICMP(const char *, const char *);
    
};

#endif
