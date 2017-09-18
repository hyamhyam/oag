// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include <string> 
#include <vector> 

using namespace std;

class Constraint
{
  public:
    //Contructor
    Constraint(vector<string>, vector<string>);

    // member
    string    if_target;
    string    if_operator;
    string    if_value;

    string    then_target;
    string    then_operator;
    string    then_value;

  private:
    string    trimParam(string);
};

