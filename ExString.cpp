// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include "ExString.h"


//-----------------------------------------------------------------------------
vector<string> ExString::split(string delimiter)
{
    vector<string> str_vec;
    
    size_type index = 0;
    size_type next  = 0;
//    basic_string <char>::size_type index = 0;
//    basic_string <char>::size_type next  = 0;

    while((index = this->find(delimiter, next)) != this->npos) {
        
        str_vec.push_back(string(this->begin() + next, this->begin() + index));
        
        next = index + delimiter.size();
        
       //  "aa bb   cc" ===> (aa,bb,cc)
       // 
       // if(delimiter == " "){
       //     while(this->at(next) == ' '){
       //         next++;
       //     }
       // }
    }
    
    str_vec.push_back(string(this->begin() + next, this->end()));

    return str_vec;
}

