// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include "ArrayFile.h"

#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }

//-----------------------------------------------------------------------------
// Constructor
ArrayFile::ArrayFile(ProgInfo  *progInfo)
{
    
    this->progInfo = progInfo;

    
    // set default
    this->isValid      = true;

    this->matrix = new Matrix();
    
    // orthogonal array check only (not getneration)
    if( !this->ReadOrthogonalArray() ) {
        this->isValid = false;
    }
    
    return;
}


//-----------------------------------------------------------------------------
//
// public method
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// private method
//
//-----------------------------------------------------------------------------
// read OrthogonalArray for -check option
//
//    reading file ===> this->matrix
//
bool  ArrayFile::ReadOrthogonalArray()
{
    
    ifstream   fin(this->progInfo->inputDataFile.c_str()); 
                                     // c_str (string ==> char)
    if(fin.fail()){
        cout << "[X] cannot open array file. \"" 
             << this->progInfo->inputDataFile << "\"" << endl;
        return  false;
    }
    
    string  workRec;
    
    int prev_rec_values_count = 0;
    
    int rec_cnt = 0;
    
    if(!this->progInfo->isNoParamName){
        // skip first record
        fin && getline(fin, workRec);
    }
    
    while( fin && getline(fin, workRec) ) {
        
        workRec = Util::TrimComment(workRec);
        if(workRec.size()==0){
            continue;
        }

        // process for one record
        //    a_1, a_2, a_3, a_4
        

        string delim = this->progInfo->inDelimiter ;
        vector<string>   vec_items = Util::Split(workRec, delim);  
        
        if(vec_items.size() <= 1) {
            cout << "[X] no value in orthogonal array" << endl;
                cout << "    " << workRec << endl;
            return  false; 
        }
        
        vector<string> vec_str;
        
        for(int i=0; i<vec_items.size(); i++){
            string value = Util::TrimString(vec_items[i]);
            if(value.size() == 0) {
                cout << "[X] no value in record." << endl;
                cout << "    " << workRec << endl;
                return  false; 
            }
            vec_str.push_back(value);
        }
                                   
        this->matrix->AddRow(vec_str);

    } // end of while  (records loop)
    
     
    return true;
}
//-----------------------------------------------------------------------------

