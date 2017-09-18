// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
#include <cassert>

#include "Constraint.h"
#include "Util.h"

//-----------------------------------------------------------------------------
// Constructor
Constraint::Constraint(vector<string> ifvalue, vector<string> thenvalue)
{

    assert(ifvalue.size() == 3);
    this->if_target   = trimParam(ifvalue[0]) ;
    this->if_operator = ifvalue[1] ;
    this->if_value    = trimParam(ifvalue[2]) ;

    assert(thenvalue.size() == 3);
    this->then_target   = trimParam(thenvalue[0]) ;
    this->then_operator = thenvalue[1] ;
    this->then_value    = trimParam(thenvalue[2]) ;
}
//-----------------------------------------------------------------------------
// public method
//

//-----------------------------------------------------------------------------
// private method
//
string Constraint::trimParam(string in_str)
{
    string out_str;
    
    out_str = Util::ReplaceString(in_str, "[" , "");
    out_str = Util::ReplaceString(out_str, "]" , "");

    out_str = Util::ReplaceString(out_str, "\"" , "");
    
    return out_str;
}

