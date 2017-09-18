// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include <cassert>
#include <math.h>

#include "LinearGraph.h"
#include "Util.h"




//-----------------------------------------------------------------------------
// Constructor

LinearGraph::LinearGraph(BaseOrthogonalArray *boa)
{
//    cout << "## LinearGraph Constructor start" << endl;
//    cout << "  baselevel=" << boa->baseLevel   << endl ;
//    cout << "  Size="      << boa->Size        << endl  ;

    if(boa == 0 || boa->Size <= 0) {
        return;
    }
    
    this->Size        = boa->Size;
    this->baseLevel   = boa->baseLevel;

//    if(boa->elementList.size() == 0 ) {
//        return;
//    }
    
    this->elementList = boa->elementList;
    
    
    // ex.
    //   L8  ==> usedColumn.size() is 7 
    

//    cout << "## LinearGraph Constructor end" << endl;
}


//-----------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------
void  LinearGraph::ClearUsedColumn()
{
    // initialize usedColumn by all false
    this->usedColumn.resize(0);
    for(int i=0; i<(this->Size - 1); i++){
        this->usedColumn.push_back(false);
    }
}
//-----------------------------------------------------------------------------
// return the enable column number set
//
vector<int>  LinearGraph::GetCols(int paramValuesCount)
{
//    cout << "## LinearGraph::GetCols start" << endl;

    vector<int> cols;
    
    if(this->Size <= 0) { return cols; }
    
    
    // level=2 and valueCount=2, then return the one column
    if(paramValuesCount == this->baseLevel){
        
        // simply, return the unused column set.
        for(int i=0; i<this->usedColumn.size(); i++){
            if(!this->usedColumn[i]){
                cols.push_back(i);
                this->usedColumn[i] = true;
                return cols;
            }
        } // end of for
        
        // cannot get cols ( all columns are already used. )
        cols.resize(0); 
        
        return cols;
    } // end of if

    
    cols = GetColsByLogic(paramValuesCount);
    
    for(int i=0; i<cols.size(); i++){
        this->usedColumn[cols[i]] = true;
    }   
     
    return cols;
}

//-----------------------------------------------------------------------------
// private method
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// adhoc logic.     reconsideration later
//  
vector<int>  LinearGraph::GetColsByLogic(int paramValuesCount)
{

    
    // ex.
    // baseLevel=2
    //    paramValuesCount    2   4   8  16 ...
    //    returnSize          1   3   7  15 ...
    //
    // baseLevel=3
    //    paramValuesCount    3   9  27  81 ...
    //    returnSize          1   4  10  22 ...
    //

    // ex2.
    // baseLevel=2
    //    returnSize = level * (2**n) - (level - 1)
    //         n = (log y X) - 1
    //         y=level, X=paramValuesCount
    
    
    int n = ( Util::Log(paramValuesCount, this->baseLevel) ) - 1 ;
              // log(x,y) ===> Log y X
              
    int returnSize = (this->baseLevel * (int)pow(2, (double)n)) - (this->baseLevel - 1);
    
    
    vector<int> cols;
    cols.resize(returnSize, -1);
    
    // element list corresponding cols
    //   ex.
    //       cols = {0,1,2}
    //       returnElementList = {"a", "b", "ab"};
    //
    vector<string> returnElementList;
    returnElementList.resize(returnSize, "");
    
    bool answerExist ;
    
    for(int col_pos=0; col_pos<returnSize; col_pos++){  // (1)
        
        answerExist = false;
        for(int i=0; i<this->usedColumn.size(); i++){   // (2)
        
            if(this->usedColumn[i]){
                // if true then it is used already
                continue; // return to (2)
            }
            
            if(!isValidElement(returnElementList, this->elementList[i])){
                // cannot use
                continue; // return to (2)
            }
            
            // can use column [i]
            returnElementList[col_pos] = this->elementList[i];
            cols[col_pos] = i;
            answerExist=true;
            
            break; // next (1)
        
        } // end of for(2)

        if(!answerExist){
            // cannot find answer...  X-(
            cols.resize(0);
            return  cols;
        }
        
    } // end of for(1)
    
    
    return cols;
}
//-----------------------------------------------------------------------------
//  Is the element valid with current returnList
bool  LinearGraph::isValidElement(vector<string> returnList, string element)
{
//    cout << "## LinearGraph::isValidElement start" << endl;

    for(int i=0; i<this->usedColumn.size(); i++){
        if(this->usedColumn[i]){
            // this->usedColumn[i] is already used.
            if(this->elementList[i] == element){
                return false;
            }
        }
    } // end of for(i)
    
    
    for(int j=0; j<returnList.size(); j++){
        string mergedName = MergeElementName(returnList[j], element);
        
        for(int k=0; k<this->usedColumn.size(); k++){
            if(this->usedColumn[k]){
                // already used
                if(this->elementList[k] == mergedName){
                    // mergedName is already used
                    return false;
                }
            }
        } // end of for(k)
        
        if(returnList[j] == ""){
            continue;
        }
        
        if(returnList[j] == element){
            return false;
        }
        
    } // end of for(j)
    
//    cout << "## LinearGraph::isValidElement end" << endl;

    return true;
}
//-----------------------------------------------------------------------------
// ex.
//  name1      name2           newname          mergedname
//   cd    +    c      ===>     cdc      ===>     d
//
string  LinearGraph::MergeElementName(string name1, string name2)
{
    //      cdc = cd + c
    string mergedName = name1 + name2;
    
    // ccd
    mergedName =  Util::SortStringChar(mergedName);
    
    // ex.
    //  level=2  ccd  ===> d
    //  level=3  cccd ===> d
    string alphaList = "abcdefghjiklmnopqrstuvwxyz";
    for(int i=0; i<alphaList.size(); i++){
        
        string target = "";
        // level=2  if alphaList[i] is 'c' then target is "cc"
        // level=3  if alphaList[i] is 'c' then target is "ccc"
        for(int j=0; j<this->baseLevel; j++){
            target += alphaList[i];
        }
        while(true){
            int loc = mergedName.find(target, 0);
            if( loc == string::npos){
                // the target is not found
                break;
            }
            
            // erase "ccc"  in mergedName (level=3)
            mergedName = mergedName.erase(loc, target.size());  
        }
    }
    
    return mergedName;
}
//-----------------------------------------------------------------------------
