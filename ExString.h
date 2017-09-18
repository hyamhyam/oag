// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#ifndef STRING_EX_H
#define STRING_EX_H


#include <string> 
#include <vector> 

using namespace std;

class ExString: public basic_string<char>
{
  public:
    // method
    vector<string> split(string);
};


#endif
