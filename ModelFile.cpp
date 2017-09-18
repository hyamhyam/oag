// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include "ModelFile.h"

#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }

//-----------------------------------------------------------------------------
// Constructor
ModelFile::ModelFile(ProgInfo  *progInfo)
{
    
    this->progInfo = progInfo;

    
    // set default
    this->isValid      = true;
    this->isDescending = true;
    this->isDummyAdded = false; // dummy value is not included for default

    
    // normal reading for model-file
    if( !this->ReadAllRecords() ) {
        this->isValid = false;
        return;
    }

    if( !this->Verify() ) {
        this->isValid = false;
        return;
    }
    
    return;
}


//-----------------------------------------------------------------------------
//
// public method
//
//-----------------------------------------------------------------------------
int  ModelFile::GetOASize()
{
    int size_by_free;   // (level1 - 1) + (level2 - 1) + (level3 - 1) ... + 1
    int size_by_multi;  // Max_level * Secont_level
    
    if(this->testParamRecords.size()==0 || 
       this->testParamRecords.size()==1    ) {
        return 0;
    }
    
    size_by_free  = Util::PowerCeil(this->progInfo->baseLevel, 
                                    this->AddAllFactorLevel());
    
    size_by_multi = Util::PowerCeil(this->progInfo->baseLevel, 
                                    this->MultiplicationTopTwoFactors());
    

    if(size_by_free > size_by_multi){
        return size_by_free;
    }else{
        return size_by_multi;
    }
}
//-----------------------------------------------------------------------------
int  ModelFile::AddAllFactorLevel()
{
    int degreeOfFreedom =0;
    
    for(int i=0; i<this->testParamRecords.size(); i++){
        TestParameter *testParam = this->testParamRecords[i];
        int level = testParam->paramValues.size();
        degreeOfFreedom = degreeOfFreedom + level - 1;
    }
    
    degreeOfFreedom = degreeOfFreedom + 1;
    
    return (degreeOfFreedom);
}
//-----------------------------------------------------------------------------
int  ModelFile::MultiplicationTopTwoFactors()
{
    int multiValue  = 0;
    
    int maxValue    = 0;
    int secondValue = 0;
    
    for(int i=0; i<this->testParamRecords.size(); i++){
        TestParameter *testParam = this->testParamRecords[i];
        int level = testParam->paramValues.size();
        
        if(maxValue < level){
            secondValue = maxValue;
            maxValue    = level;
        }else{
            if(secondValue < level){
                secondValue = level;
            }
        }
    }
    
    multiValue = maxValue * secondValue ;
    
    return (multiValue);
}
//-----------------------------------------------------------------------------
bool  ModelFile::Verify()
{
    if(this->testParamRecords.size() == 0){
        cout << "[X] no parameter record." << endl;
        cout << "    parameter record must be more than 2." << endl;
        return false;
    }

    if(this->testParamRecords.size() == 1){
        cout << "[X] only one parameter record." << endl;
        cout << "    parameter record must be more than 2." << endl;
        return false;
    }
    
    return true;
}
//-----------------------------------------------------------------------------
// if paramSizeList = (4,2,2)
//   then maxCombinationCnt is 16(=4*2*2)
//   then totalParameterValuesCnt is 8(=4+2+2)
bool  ModelFile::MakeMaxCombinationCnt()
{
    this->maxCombinationCnt = 1;
    this->totalParameterValuesCnt = 0;
    
    list<int>::iterator ite = this->paramSizeList.begin(); 
    while( ite != this->paramSizeList.end() ) {
        int paramSize = *ite;
        long tmp = this->totalParameterValuesCnt;
        this->totalParameterValuesCnt += paramSize;
        
        if(this->totalParameterValuesCnt < 0   ||
           this->totalParameterValuesCnt < tmp ||
           this->totalParameterValuesCnt > 2147483647  // 64bit
          ){  
            // overflow
            this->totalParameterValuesCnt = -9;
            cout << "[X] totalParameterValuesCnt overflow." << endl;
            return false;
        }
        ++ite; 
    }

    ite = this->paramSizeList.begin(); 
    while( ite != this->paramSizeList.end() ) {
        int paramSize = *ite;

        long tmp = this->maxCombinationCnt;

        this->maxCombinationCnt       *= paramSize;

        
        if(this->maxCombinationCnt < 0   ||
           this->maxCombinationCnt < tmp ||
           this->maxCombinationCnt > 2147483647   // 64bit
          ){
            // overflow
            this->maxCombinationCnt = -9;
            
            // do not error return here, because maxCombinationCnt
            // may not be used.
            return true;
        }
        ++ite; 
    }
    
    return true;
}
//-----------------------------------------------------------------------------
void  ModelFile::SortParameterRecords()
{
    
    sort(this->testParamRecords.begin(), 
         this->testParamRecords.end(),
         &CompareParamCount
        );

    this->paramSizeList.sort();
    this->paramSizeList.reverse();

    return ;
}
//-----------------------------------------------------------------------------
void  ModelFile::SortParameterRecordsByOriginalOrder()
{
    
    sort(this->testParamRecords.begin(), 
         this->testParamRecords.end(),
         &CompareParamCountByOriginalOrder
        );
        
    return ;
}
//-----------------------------------------------------------------------------
bool  ModelFile::CompareParamCount(const TestParameter *a, 
                                   const TestParameter *b)
{
//cout << "a size:" <<     a->paramValues.size() << endl;
//cout << "b size:" <<     b->paramValues.size() << endl;

    if(a->paramValues.size() > b->paramValues.size() ){
        return true;
    }
    if(a->paramValues.size() < b->paramValues.size() ){
        return false;
    }
    
    // if a->paramValues.size() == b->paramValues.size()
    // then check orignal_pos
    // orignal_pos does not have same value
    if(a->orignal_pos < b->orignal_pos ){
        return true;
    }
    
    return false;
}
//-----------------------------------------------------------------------------
bool  ModelFile::CompareParamCountByOriginalOrder
                                  (const TestParameter *a, 
                                   const TestParameter *b)
{
    
    // if a->paramValues.size() == b->paramValues.size()
    // then check orignal_pos
    // orignal_pos does not have same value
    
    if(a->orignal_pos < b->orignal_pos ){
        return true;
    }
    
    return false;
}
//-----------------------------------------------------------------------------
void  ModelFile::Information(string msg)
{
    Util::PrintHeader(msg);
    
    cout << "# input file name = " 
         << this->progInfo->inputDataFile << endl;
    
    
    for(int i=0; i<this->testParamRecords.size(); i++){
        TestParameter *testParam = this->testParamRecords[i];
        cout <<  testParam->paramName << " : " ;
        
        Util::PrintVector( testParam->paramValues );
        
    }
    
}
//-----------------------------------------------------------------------------
void  ModelFile::Information()
{
    Information("ModelFile Information");
}
//-----------------------------------------------------------------------------
void  ModelFile::PrintAllParamName()
{
    for(int i=0; i<this->testParamRecords.size(); i++){
        TestParameter *testParam = this->testParamRecords[i];
        cout <<  testParam->paramName ;
        
        if(i < ( this->testParamRecords.size() - 1 ) ){
            cout << this->progInfo->outDelimiter;
        }
    }
    cout << endl;
}
//-----------------------------------------------------------------------------
bool  ModelFile::Complement()
{
    for(int i=0; i<this->testParamRecords.size(); i++){

        TestParameter *testParam = this->testParamRecords[i];

        int valuesCount = testParam->paramValues.size();
        int ceil = Util::PowerCeil(this->progInfo->baseLevel, valuesCount);

        if(ceil != valuesCount && this->progInfo->baseLevel >= 2){
            // dummy value complementation
            // ex. level=2
            //         AAA : a_1,a_2,a_3
            //    ===> AAA : a_1,a_2,a_3, a_1(D)
            //                            ~~~~~~dummy
            
            if(!this->progInfo->enableComplementation &&
               !Util::StrIcmp(this->progInfo->algorithm, "gf")  ){
                cout << "# [X] number of values is not equal to level**n." << endl;
                cout << "    " << testParam->paramName << " : " ;
                Util::PrintVector(testParam->paramValues);
                cout << "    must be " << ceil  << endl;
                cout << "     (level=" << this->progInfo->baseLevel << ")" << endl;
                return  false; 
            }else{
                if(!this->progInfo->enableComplementation){
                    return false;
                }else{
                    // NOP
                    //   specify -Complementation then do complementation
                }
            }

            // do complementation
            for(int j=0; j<(ceil - valuesCount); j++){
                string value ="";
                if(this->progInfo->noD){
                     value = testParam->paramValues[j] ;
                }else{
                     value = testParam->paramValues[j] + "(D)";
                }
                testParam->paramValues.push_back(value);
            }
            this->isDummyAdded = true;
            
        } // end of complementation
        
    } // end of for

    if(this->isDummyAdded && this->progInfo->isDebug){
        this->Information("ModelFile complementation");
    }
    
    return true;
}
//-----------------------------------------------------------------------------
//
//  A:a1,a2,a3
//  B:b1,b2
//  C:c1,c2
//
//             input          output
//  vec_col = (1,0,1) --->  (a2, b1, c2)
//
vector<string>  ModelFile::Element2Value(vector<string> vec_col)
{
    vector<string> result;
    for(int i=0; i<vec_col.size(); i++){
        int val = atoi( vec_col[i].c_str() );
        TestParameter *testParam = this->testParamRecords[i];
        result.push_back(testParam->paramValues[val]);
    }
    
    return result;
}

vector<string>  ModelFile::Element2Value(vector<int> vec_col)
{
    vector<string> result;
    for(int i=0; i<vec_col.size(); i++){
        TestParameter *testParam = this->testParamRecords[i];
        result.push_back(testParam->paramValues[vec_col[i]]);
    }
    
    return result;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//
// private method
//
//-----------------------------------------------------------------------------
bool  ModelFile::ReadAllRecords()
{
    TRACER("> ModelFile::ReadAllRecords")
    
    ifstream   fin(this->progInfo->inputDataFile.c_str()); 
                                     // c_str (string ==> char)
    if(fin.fail()){
        cout << "[X] cannot open model file. \"" 
             << this->progInfo->inputDataFile << "\"" << endl;
        return  false;
    }
    
    string  workRec;
    
    int prev_rec_values_count = 0;
    
    int rec_cnt = 0;
    
    while( fin && getline(fin, workRec) ) {
        
        workRec = Util::TrimComment(workRec);
        if(workRec.size()==0){
            continue;
        }

        if(IsConstraints(workRec)){
            continue;
        }

        // process for one record
        //    AAA : a_1, a_2, a_3, a_4
        
        vector<string>   v_work = Util::Split(workRec , ":");  
        
        if(v_work.size() > 2) {
            cout << "[X] \":\" must be one in a record." << endl;
            cout << "    " << workRec << endl;
            return  false;  
        }
        

        if(v_work.size() != 2) {
            
            // no Parameter
            if(this->testParamRecords.size()==0){
                cout << "[X] no delimiter \":\"" << endl;
                cout << "    " << workRec << endl;
                return  false;  
            }

            // save values into previous record
            //   AAA : a_1, a_2
            //         a_3, a_4
            vector<string>   vec_values = GetValues(workRec);
            int last = this->testParamRecords.size() - 1 ;
            
            this->testParamRecords[last]->paramValues.insert(
                this->testParamRecords[last]->paramValues.end(),
                vec_values.begin(), vec_values.end() );
            
            continue;  // return to while
            
        }

        TestParameter  *testParam= new TestParameter();           

        testParam->paramName = Util::TrimString(v_work[0]); 
        testParam->paramValues = GetValues(v_work[1]);
        
        int valuesCount = testParam->paramValues.size();
        if(valuesCount == 0){
            // AAA : 
            //       no value
            cout << "# [X] no value in record." << endl;
            cout << "    " << workRec << endl;
            return  false; 
        }
        if(valuesCount == 1){
            // AAA : 111
            //       only one value
            cout << "# [X] value must be more than two." << endl;
            cout << "    " << workRec << endl;
            return  false; 
        }
        
        if(prev_rec_values_count != 0 &&
           prev_rec_values_count < valuesCount ){
            this->isDescending = false;
        }
        prev_rec_values_count = valuesCount;
        
        
        testParam->orignal_pos = rec_cnt ; // 0 origin
        rec_cnt++;
        
        this->testParamRecords.push_back(testParam);

    } // end of while  (records loop)
    
    
    // make this->paramSizeList
    vector<TestParameter *>::iterator ite = this->testParamRecords.begin(); 
    while( ite != this->testParamRecords.end() ) {
        TestParameter *testP = *ite;
        this->paramSizeList.push_back(testP->paramValues.size());
        this->paramSizeVector.push_back(testP->paramValues.size());
        ++ite; 
    }
    
    
    if(this->progInfo->baseLevel == 0){
        this->baseLevel = this->GetBaseLevel();
        this->progInfo->baseLevel = this->baseLevel ;
    }
    
    this->paramCount = this->testParamRecords.size();
 
    return true;
}
//-----------------------------------------------------------------------------
int  ModelFile::GetBaseLevel()
{
    TRACER("> ModelFile::GetBaseLevel")
    
    list<int> uniqList = this->paramSizeList;
    
                         // (3,27,9,3,9)
    uniqList.unique();   //   ==>(3,27,9)
    uniqList.sort();     //   ==>(3,9,27)
    uniqList.reverse();  //   ==>(27,9,3)

    int max = Util::Max(uniqList);
    int min = Util::Min(uniqList);
    
    //cout << "max=" << max << ",min=" << min << endl;
    
    // all level are same and prime number. ( and < 100 ) 
    if(max==min){
        if(max<20){
            if(max==3 || max==5 || max==7 || max==11 || max==13 || 
               max==17 || max==19 ){
                return max;
            }
        }else{
            if(max==23 || max==29 || max==31 || max==37 || max==41 ||
               max==43 || max==47 || max==53 || max==59 || max==61 || 
               max==67 || max==71 || max==79 || max==83 || max==89   ){
                return max;
            }
        }
    }

    string param = Util::GetListName(uniqList);
    
    
    if(param=="9_3" || param=="27_3" || param=="27_9_3"){
        return 3;
    }
    
    if(param=="25_5" || param=="125_5" || param=="125_25_5"){
        return 5;
    }
    
    
    
    return 2;
}
//-----------------------------------------------------------------------------
// "a_1,a_2,a_3,a_4"  ===>  vector<string> (a_1, a_2, a_3, a_4)
vector<string> ModelFile::GetValues(string dataString)
{
    
    string delim = this->progInfo->inDelimiter ;
    vector<string>   vec_items = Util::Split(dataString, delim); 

    vector<string>   rtnVec; 

    for(int i=0; i<vec_items.size(); i++){
        
        string value = Util::TrimString(vec_items[i]);
        
        if(value.size() == 0 ) {
            if( i != (vec_items.size() - 1) ) {
                //  a_1,a_2,,a_4  
                rtnVec.resize(0);
                return  rtnVec; 
            }else{
                // last null field
                //   AAA:a1,a2,a3,  <-- comma end
                break; // mo error
            }
        }
        rtnVec.push_back(value);
    }
    
    return rtnVec;
}


//-----------------------------------------------------------------------------
//
//  adhoc logic for simple test.  try to lex, later.
//
bool  ModelFile::IsConstraints(string record)
{
    // trim space
    string trimedRec = Util::TrimString(record);
    
    unsigned int loc = trimedRec.find( "IF ", 0 );
    if(loc != 0){
        loc = trimedRec.find( "if ", 0 );
    }
    
    if(loc == 0){
        // "IF" is exist.
     
        vector<string>   vec_str = Util::Split(trimedRec, "THEN"); 
        if(vec_str.size()==1){
            vec_str = Util::Split(trimedRec, "then"); 
        }
        
        if(vec_str.size()==2){
            string ifValue   = vec_str[0];
            string thenValue = vec_str[1];
            
            ifValue   = Util::ReplaceString(ifValue, "IF " , "");
            ifValue   = Util::ReplaceString(ifValue, "if " , "");
            
            thenValue = Util::ReplaceString(thenValue, ";" , "");
            ifValue   = Util::TrimString(ifValue);
            thenValue = Util::TrimString(thenValue);

            vector<string> if_values   = Util::Split(ifValue   , " ") ;

            vector<string> then_values = Util::Split(thenValue , " ") ;
            
            if(if_values.size() !=3 || then_values.size() !=3){
                cout << "[X] constraint record format is invalid." << endl;
                cout << "    " << record << endl;
                return false;
            }
            
            Constraint *constraint = new Constraint(if_values, then_values);
            this->constraintRecords.push_back(constraint);
        }
        
        return true;
    }
    
    return false;
    
}
//-----------------------------------------------------------------------------


