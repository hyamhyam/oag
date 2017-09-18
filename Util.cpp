// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include <time.h>
#include <math.h>

#include "Util.h"



//-----------------------------------------------------------------------------
// Constructor
Util::Util()
{
}


//-----------------------------------------------------------------------------
// static  public method
//-----------------------------------------------------------------------------
//  a : new base
//  X : target data
//   ===> Log a X = log e A / log e x
//      ===> log(a,x) = log(a)/log(x)
//
int  Util::Log(int x, int a)
{
    
    double ansTemp = log((double)x) / log((double)a) ;
    int    ansInt  = (int)ansTemp;
    
    return ( ansInt ) ;
}


//-----------------------------------------------------------------------------
//  return the minimum value(power of "level") more than "number" 
//
//    input  (level=2, number=7)
//
//          2**1  2**2  2**3  2**4 ...
//            2     4     8    16  <--- power of "level"
//                     7           <--- "number"
//    output 8 
//
int  Util::PowerCeil(int level, int number)
{
    int i = level;
//    cout << "level=" << level << endl;
    
    if(level <= 0) { return 0 ; }
    
    while (true) {
        if(i>=number) {
            return i; 
        }
        
        i = i * level;
    }
    
}
//-----------------------------------------------------------------------------
// 
//  if level == 2  then XOR.
//
int  Util::GFadd(int level, int pos, int A, int B){
    
    return ( ( A*pos) + B ) % level ;
}

//-----------------------------------------------------------------------------
// (6,9) ===> 3
int  Util::GetGreatestCommonDiviser(int a, int b)
{
    // a must be greater than b.
    
    if(b>a){
        int tmp = a;
        a = b;
        b = tmp;
    }
    
    int  m  = a % b ;
    
    if(m==1){ return 1; }
    
    if(m==0){
        return b;
    }else{
        return GetGreatestCommonDiviser(b, m);
    }
    
}
//-----------------------------------------------------------------------------
//  (3,4) ===> 12
int  Util::GetLeastCommonMultiple(int a, int b)
{
    
    int    ansInt  = (a * b) / GetGreatestCommonDiviser(a,b) ;
    
    return ( ansInt ) ;
}

// (3,4,5) ==> 60
int  Util::GetLeastCommonMultiple(list<int> paramList)
{
    
    int ansInt = paramList.front();

    
    list<int>::iterator ite = paramList.begin(); 
    while( ite != paramList.end() ) {
        int value = *ite  ;
        ansInt = Util::GetLeastCommonMultiple(ansInt , value);
        ite++;
    }
    
    return ( ansInt ) ;
}

//-----------------------------------------------------------------------------
int  Util::Max(list<int> paramList)
{
    
    int max = paramList.front();
    
    list<int>::iterator ite = paramList.begin(); 
    while( ite != paramList.end() ) {
        int value = *ite  ;
        if(max < value){
            max = value;
        }
       
        ite++;
    }
    
    return ( max ) ;
}
//-----------------------------------------------------------------------------
int  Util::Min(list<int> paramList)
{
    
    int min = paramList.front();
    
    list<int>::iterator ite = paramList.begin(); 
    while( ite != paramList.end() ) {
        int value = *ite  ;
        if(min > value){
            min = value;
        }
       
        ite++;
    }
    
    return ( min ) ;
}

//-----------------------------------------------------------------------------
// if all values is not equal
//   (5,4,3) ===> 60  GetLeastCommonMultiple
// all equal
//   (5,5,5) ===> 25  ( not 5 because of strength=2)
//
//   (5,5,5,5,5,5,5,5,5,5) ===> 4*10 + 1 = 41 ==> 50
//
int  Util::GetMixedArraySize(list<int> paramList)
{
    if(paramList.size()==0){
        cout << "# [X] paramSizeList is null" << endl;
    }
    
    
    int GLCM = 1;
    bool isAllEqual = true;
    
    int first = paramList.front();
    if(paramList.size()==1){
        return first ;
    }
    
    int second=1;
    int cnt=0;
    
    int addAll = 0;
    
    list<int>::iterator ite = paramList.begin(); 
    while( ite != paramList.end() ) {
        int value = *ite  ;
        GLCM = Util::GetLeastCommonMultiple(GLCM , value);
        
        addAll = addAll + (value - 1) ;
        
        ite++;
        
        if(cnt==1){
            second = value;
        }
        
        cnt++; 
    }
    
    addAll = addAll - 1;
    
    int multiTopTwo = first * second ;
    
//    cout << "GLCM=" << GLCM << ",addAll=" << addAll 
//         << ",multiTopTwo=" << multiTopTwo << endl;
    
    int answer = GLCM;
    while(answer < addAll || answer < multiTopTwo ){
        answer =  answer + GLCM  ;
    }
    
    return ( answer ) ;
    
}

//-----------------------------------------------------------------------------
string  Util::Num2Alpha(int pos){
    string alphaList = "abcdefghjiklmnopqrstuvwxyz";
    
    string rtnStr( 1,  alphaList[pos] );

    return rtnStr ;
}

//-----------------------------------------------------------------------------
string  Util::Int2String(int num){
    char temp[5];
    assert(num < 99999);  // signed int <= 32767
    int rtn = sprintf(temp, "%d", num);
    
    string rtn_string(temp);
    
    return rtn_string;
    
//    ostringstream ostm ;
//    ostm << num ;
//    return ostm.str() ;
}

//-----------------------------------------------------------------------------
string  Util::TrimComment(string in_str){
    string trimedRec = Util::TrimString(in_str);
    

    if(trimedRec.size() ==0){
        return trimedRec;   // all space record
    }
    
    unsigned int loc = trimedRec.find( "#", 0 );
    if(loc != -1){ // cannot use npos with gcc 4.4.6 on CentOS ??
//    if(loc != string::npos){
        trimedRec.erase(loc);
    }
    
    return trimedRec;
}
//-----------------------------------------------------------------------------
//
//    "   abcd  fg   "  ===> "abcd  fg" 
//
string  Util::TrimString(string in_str){
    
    char *charArray = (char*)in_str.c_str();
    
    int   len       = strlen(charArray);
    int   startPos  = 0;
    int   endPos    = len - 1;
    bool  isAllSpace = true;
    
    // check head space
    for( int i = 0; i < len; i++ ){
        if( charArray[i] != ' ' ){
            startPos = i;
            isAllSpace = false;
            break;
        }
    }

    // check tail space
    for( int j = len - 1; j >= 0; j-- ){
        if( charArray[j] != ' ' ){
            endPos = j;
            isAllSpace = false;
            break;
        }
    }
    
    if(isAllSpace){
        return "";    //  all space or no data
    }

    assert(startPos <= endPos);
    
    return(in_str.substr( startPos, endPos - startPos + 1 ));
}

//-----------------------------------------------------------------------------
void  Util::PrintHeader(string  msg){

    cout << "#--------------------------------------------------------" << endl;
    if(msg != ""){
        cout << "## " << msg  << endl;    
    }
}

//-----------------------------------------------------------------------------
void  Util::PrintMatrix(Matrix *matrix)
{
    Util::PrintMatrix(matrix->data);
}

//-----------------------------------------------------------------------------
void  Util::PrintMatrix(vector< vector<int> >  data)
{
    for(int i=0; i < data.size(); i++){
       Util::PrintVector(data[i]);
    }
}
void  Util::PrintMatrix(vector< vector<string> >  data)
{
    for(int i=0; i < data.size(); i++){
       Util::PrintVector(data[i]);
    }
}
//-----------------------------------------------------------------------------
void  Util::PrintMatrix(vector< vector<int> >  data, string delimiter)
{
    for(int i=0; i < data.size(); i++){
       Util::PrintVector(data[i], delimiter);
    }
}
void  Util::PrintMatrix(vector< vector<string> >  data, string delimiter)
{
    for(int i=0; i < data.size(); i++){
       Util::PrintVector(data[i], delimiter);
    }
}
//-----------------------------------------------------------------------------
void  Util::PrintVector(vector<int>  int_vec)
{
    Util::PrintVector(int_vec, ",");
}
void  Util::PrintVector(vector<string>  str_vec)
{
    Util::PrintVector(str_vec, ",");
}
//-----------------------------------------------------------------------------
void  Util::PrintVector(vector<int>  str_vec, string delimiter)
{
    for(int i=0; i < str_vec.size(); i++){
        cout << str_vec[i]  ;
        if(i != (str_vec.size() - 1) ) { 
            cout << delimiter ;
        }
    }
    cout << endl;
}
void  Util::PrintVector(vector<string>  str_vec, string delimiter)
{
    for(int i=0; i < str_vec.size(); i++){
        cout << str_vec[i]  ;
        if(i != (str_vec.size() - 1) ) { 
            cout << delimiter ;
        }
    }
    cout << endl;
}
//-----------------------------------------------------------------------------
void  Util::PrintList(list<int>  int_list)
{
    list<int>::iterator ite = int_list.begin(); 

    while( ite != int_list.end() ) {
        cout << *ite  ;

        ++ite; 
        if( ite != int_list.end() ) { 
            cout << ",";
        }
    }
    cout << endl;
    
}
void  Util::PrintList(list<string>  str_list)
{
    list<string>::iterator ite = str_list.begin(); 

    while( ite != str_list.end() ) {
        cout << *ite  ;

        ++ite; 
        if( ite != str_list.end() ) { 
            cout << ",";
        }
    }
    cout << endl;
    
}
//-----------------------------------------------------------------------------
string  Util::SortStringChar(string inStr)
{
//    cout << "## Util::SortStringChar start" << endl;

    list<char> workList;
    
    for(int i=0; i<inStr.size(); i++){
        workList.push_back( inStr[i] );
    }
    
    workList.sort();
    
    string outStr = "";
    
    list<char>::iterator ite = workList.begin(); 
    while( ite != workList.end() ) {
        char workChar = *ite;
        outStr += workChar ;
        ++ite; 
    }

//    cout << "## Util::SortStringChar end" << endl;
    
    return outStr;
}
//-----------------------------------------------------------------------------
// it is not fixed (no debug)
// do not use yet...
string  Util::ReplaceString(string source, string before, string after)
{
   
    string result = source;
    
    for(string::size_type pos = 0 ;
        string::npos != (pos = result.find(before, pos));
        pos += after.size()
    ){
        result.replace( pos, before.size(), after );
    }
    
    
    return result;
}
//-----------------------------------------------------------------------------
vector<string>  Util::Split(string source, string delimiter)
{
   
    ExString str_ex;
    str_ex.assign( source )  ; 
    vector<string>   vec_str = str_ex.split( delimiter ); 
  
    // null value is not included in new vector
    //  split(aaa, " ");
    //  aaa = "XX YY   ZZ"
    //              ~~~
    //  vec_str = (XX,YY,,,ZZ)
    //  new_vec_str = (XX,YY,ZZ)
    
    vector<string>   new_vec_str;
    for(int i=0; i<vec_str.size(); i++){
        if(vec_str[i].size() > 0) {
            new_vec_str.push_back(vec_str[i]);
        }
    }    
    return new_vec_str;
}
//-----------------------------------------------------------------------------
int Util::StrIcmp(const char *s1, const char *s2)
{
#ifdef  PC
    return stricmp(s1, s2);
#else
    return strcasecmp(s1, s2);
#endif
}
int Util::StrIcmp(string s1, string s2)
{
#ifdef  PC
    return stricmp(s1.c_str() , s2.c_str());
#else
    return strcasecmp(s1.c_str(), s2.c_str());
#endif
}



//-----------------------------------------------------------------------------
// (4,2,2,2) ---> "4_2x3"
// (4,4,2,2,2) ---> "4x2_2x3"
// (3,3,3,3,3,3,3,2) ---> "3x7_2"
//
//   intList must be sorted.
//
string  Util::GetListName(list<int> intList)
{
   
    string result ;
    
    int preInt=0;
    int numCnt=0;
    int Cnt=0;
    
    list<int>::iterator ite = intList.begin(); 
    while( ite != intList.end() ) {
        int workInt = *ite;
        
        if(preInt == workInt){
            numCnt++;
        }else{
            if(numCnt != 0 ){
                result += ( "x" +  Int2String( numCnt + 1)  ) ;
                numCnt=0;
            } 
            if(Cnt==0){
                result +=  Int2String(workInt)  ;
            }else{
                result +=  ( "_" + Int2String(workInt) )  ;
            }
        }
        ++ite; 
        ++Cnt;
        preInt = workInt;
    }
    
    if(numCnt != 0 ){
        result += ( "x" +  Int2String( numCnt + 1)  ) ;
    }
        
    return result;
}
//-----------------------------------------------------------------------------
long  Util::GetLn(vector<int> sizeVec,int N, int n)
{
    if(n > sizeVec.size()){
        cout << "# [X] GetLn sizeVec invalid n=" << n << endl;
    }

    double Ln1=0;
    double Ln2=0;
    double Ln3=0;
    
    long LnValue;
    
 //   int N = this->rowSize ;

    for(int k=0; k<n; k++){
        int Sk =  sizeVec[k];
        Ln1 = Ln1 + (N / Sk);
    }
    Ln1 = Ln1 * Ln1 ;
    
    for(int k=0; k<n; k++){
        int Sk =  sizeVec[k];
        Ln2 = Ln2 +
            ((Sk - 1)*(N * N )) / (Sk * Sk) ;
    }
    
    Ln3 = N*n*n;
    
    LnValue = (int) ((Ln1 + Ln2 - Ln3) / 2 );
    

    return LnValue ;
}
