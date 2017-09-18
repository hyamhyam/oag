// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
#include <time.h>

#include "OrthogonalArray.h"

#include "AlgorithmAll.h"
#include "AlgorithmBigCA.h"
#include "AlgorithmCA.h"
#include "AlgorithmGF.h"
#include "AlgorithmXu.h"
#include "AlgorithmRandom.h"

#include "ArrayFile.h"
#include "ModelFile.h"
#include "Util.h"

#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }


//-----------------------------------------------------------------------------
// Constructor
OrthogonalArray::OrthogonalArray(ProgInfo *progInfo)
{
    
    this->progInfo   = progInfo;
    
    this->baseLevel     = progInfo->baseLevel;
    this->isDebug       = progInfo->isDebug;
    this->isTableCheck  = progInfo->isTableCheck;
    this->ShowInfo      = progInfo->ShowInfo;
    
    this->baseArray    = 0;
//    this->linearGraph  = 0;
    
    this->Size  = 0;
    this->isValid = true;
}

//-----------------------------------------------------------------------------
// public method
void  OrthogonalArray::PrintResult()
{
    if(this->progInfo->isStream){
        return; // not exist ResultArray in stream mode
    }

    if(this->isDebug || this->ShowInfo) {
        Util::PrintHeader("result array");
    }
    
    string delim = this->modelFile->progInfo->outDelimiter ;
    
    this->modelFile->PrintAllParamName();
    
    if(this->resultArrayWithConstraints.size() == 0){
        // no constraints
        Util::PrintMatrix(this->resultArray, delim);
    }else{
        if(this->modelFile->progInfo->enableConstraint){
            Util::PrintMatrix(this->resultArrayWithConstraints, delim);
        }else{
            Util::PrintMatrix(this->resultArray, delim);
        }
    }
}
//-----------------------------------------------------------------------------
void  OrthogonalArray::Information()
{

    if(this->progInfo->isStream){
        return; // do not display information in stream mode
    }


    // -debug only
    if(this->baseArray != 0 && this->isDebug){
        if(this->baseArray->elementList.size() != 0 &&
           this->baseArray->matrix->rowSize != 0 ){
            Util::PrintHeader("Base Array");
            cout << "## base array elementList" << endl ;
            Util::PrintVector(this->baseArray->elementList);
            this->baseArray->matrix->Information();
        }
    }
    
    
    // -debug and -show 
    if(this->isDebug || this->ShowInfo){ 
        if(!Util::StrIcmp(this->progInfo->algorithm, "ca")){
            Util::PrintHeader("Covering Array");
        }else if(!Util::StrIcmp(this->progInfo->algorithm, "all")){
            Util::PrintHeader("All Combinations");
        }else if(!Util::StrIcmp(this->progInfo->algorithm, "random")){
            Util::PrintHeader("Random Array");
        }else{
            Util::PrintHeader("Orthogonal Array");
        }
        cout << "## array data" << endl ;
        cout << "# number of parameters = " << this->paramCount << endl ;
        
        if(this->baseLevel > 0 &&
           (this->progInfo->algorithm=="" ||
            !Util::StrIcmp(this->progInfo->algorithm, "gf")
           )
          ) {
            cout << "# level                = " << this->baseLevel << endl ;
        }
        
        cout << "# test size            = " << this->Size << endl ;
        
        if(this->elementList.size() != 0 && this->isDebug){
            cout << "## orthogonal array elementList" << endl ;
            Util::PrintVector(this->elementList);
        }
        

        if(this->isDebug && !this->progInfo->isStream){
            this->matrix->Information();  
        }
    }
}

//-----------------------------------------------------------------------------
void  OrthogonalArray::Confirm()
{
    TRACER("OrthogonalArray::Confirm")
    
    if(!this->progInfo->isStream){
        this->matrix->ConfirmOA(this->progInfo);
    }
}
//-----------------------------------------------------------------------------
//  Caution:
//    if model file parameters number is not descending,
//    sort modelfile, make orthogonal array, and sort model file again.
//  
//  
//  original Model File
//   +----------------------------+ orignal_pos
//   | AAA : a_1, a_2             |   0
//   | BBB : b_1, b_2, b_3, b_4   |   1
//   | CCC : c_1, c_2             |   2
//   +----------------------------+  
//       |
//  SortParameterRecords();                               
//       |
//       V
//   +----------------------------+ 
//   | BBB : b_1, b_2, b_3, b_4   |   1
//   | AAA : a_1, a_2             |   0
//   | CCC : c_1, c_2             |   2
//   +----------------------------+  
//       |
//       +-------------------------------------------------+
//       |                                                 |
//       |                                            orthogonal array
//       |                                            (BBB,AAA,CCC):order
//       |                                              0, 0, 0   
//       |                                              0, 1, 1
//  SortParameterRecordsByOriginalOrder();              1, 0, 0               
//       |                                              1, 1, 1
//       |                                              2, 0, 1
//       |                                              2, 1, 0
//       V                                              3, 0, 1
//   +----------------------------+                     3, 1, 0
//   | AAA : a_1, a_2             |   0                   |
//   | BBB : b_1, b_2, b_3, b_4   |   1                   |
//   | CCC : c_1, c_2             |   2                   |
//   +----------------------------+                   ChangeColumn();
//       |                                                |
//       |                                                V
//       |                                          (AAA,BBB,CCC):order
//       |                                             0, 0, 0
//       |                                             1, 0, 1
//       |                                             0, 1, 0
//       |                                             1, 1, 1
//       |                                             0, 2, 1
//       |                                             1, 2, 0
//       |                                             0, 3, 1
//       |                                             1, 3, 0
//       |                                                |
//       |                                                |
//       +-----------------------------------------> SetItemData();
//                                                        |
//                                                        |
//                                                        V
//                                                  [ result array ]
//                                                    a_1, b_1, c_1
//                                                    a_2, b_1, c_2
//                                                    a_1, b_2, c_1
//                                                    a_2, b_2, c_2
//                                                    a_1, b_3, c_2
//                                                    a_2, b_3, c_1
//                                                    a_1, b_4, c_2
//                                                    a_2, b_4, c_1
//
//
//
//-----------------------------------------------------------------------------
// for normal generate 
bool  OrthogonalArray::Generate(ModelFile *model)
{

    this->modelFile  = model;
    if(this->modelFile->MakeMaxCombinationCnt() == false){
        return false;
    }
    
    // 
    if(!this->modelFile->isDescending && !this->progInfo->isStream ){
        // sort SortParameterRecords by array size
        this->modelFile->SortParameterRecords();
    }
    
    this->paramCount = this->modelFile->paramCount;
    
    if(!Algorithm()){
        // cannot generate the array
        return false;
    }

    
    
    if(!this->modelFile->isDescending && !this->progInfo->isStream){
        // sort orthogonal array coloumns by original model order
        ChangeColumn();             
        // sort SortParameterRecords by original order
        this->modelFile->SortParameterRecordsByOriginalOrder();
    }


    // Set ItemData of TestParameter to the resultArray.
    if(!this->progInfo->isStream){
        SetItemData();
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// for check option
bool  OrthogonalArray::Generate(ArrayFile *array)
{
    this->matrix     = array->matrix;
    this->Size       = array->matrix->rowSize;   // nn for Lnn
    this->paramCount = array->matrix->colSize;   
    
    return true;
}


//-----------------------------------------------------------------------------
// private method
//-----------------------------------------------------------------------------

bool  OrthogonalArray::Algorithm()
{
    if(this->paramCount == 2){
        // if there are only 2 parameter, then generate all combination array 
        return  Make2ParamArray() ;
    }
    
    if(!Util::StrIcmp(this->progInfo->algorithm, "xu") ){
        // Xu 2002 algorithm
        if(this->MakeXu()) {
            return true;
        }else{
            cout << "# [X] MakeXu error (1)" << endl;
            return false;
        }
    }

    if(!Util::StrIcmp(this->progInfo->algorithm, "gf") ){
        // Xu 2002 algorithm
        if(this->MakeGF()) {
            return true;
        }else{
            cout << "# [X] MakeGF error (1)" << endl;
            return false;
        }
    }
    
    if(!Util::StrIcmp(this->progInfo->algorithm, "ca") ){
        if(this->MakeCA()) {
            return true;
        }else{
            cout << "# [X] MakeCA error (1)" << endl;
            return false;
        }
    }
    
    if(!Util::StrIcmp(this->progInfo->algorithm, "bigca") ){
        if(this->MakeBigCA()) {
            return true;
        }else{
            cout << "# [X] MakeBigCA error (1)" << endl;
            return false;
        }
    }

    if(!Util::StrIcmp(this->progInfo->algorithm, "all") ){
        // all combination
        if(this->MakeAll()) {
            return true;
        }else{
            cout << "# [X] MakeAll error (1)" << endl;
            return false;
        }
    }

    if(!Util::StrIcmp(this->progInfo->algorithm, "random") ){
        if(this->MakeRandom()) {
            return true;
        }else{
            cout << "# [X] MakeRandom error (1)" << endl;
            return false;
        }
    }
    
        
    int max  = Util::Max(this->modelFile->paramSizeList);
    int GLCM = Util::GetLeastCommonMultiple(this->modelFile->paramSizeList);


    if(GLCM == this->modelFile->maxCombinationCnt){
        TRACER("> if GLCM==Max Combination Count, then algorithm All Combination")
        if(GLCM > 10000){
            this->progInfo->isStream  = true;  // force a stream mode
        }
        if(this->MakeAll()) {
            return true;
        }else{
            cout << "# [X] MakeAll error (2)" << endl;
            return false;
        }
    }
        
    
    if(max < GLCM){
        // Xu 2002 algorithm
        if(this->MakeXu()) {
            return  true;
        }else{
            cout << "# [X] MakeXu error (2)" << endl;
            return false;
        }
    }else{    
        // default algorithm
        if(this->MakeGF()) {
            return  true;
        }else{
            TRACER("> [X] MakeOA error (2)");
            
            if(this->MakeXu()) {
                return true;
            }
            TRACER("> [X] MakeXu error (3)");
            return false;
        }
          
          
    }  // end of if GLCM
        
}
//-----------------------------------------------------------------------------
// if there are parameters only, then generate all combination array 
//
//  A:a1,a2,a3 ----> a1,b1
//  B:b1,b2          a1,b2
//                   a2,b1
//                   a2,b2
//                   a3,b1
//                   a3,b2

bool  OrthogonalArray::Make2ParamArray()
{
    TRACER("> OrthogonalArray::Make2ParamArray start")

    int size1 = this->modelFile->testParamRecords[0]->paramValues.size();
    int size2 = this->modelFile->testParamRecords[1]->paramValues.size();

    int N = size1 * size2 ;

    this->matrix = new Matrix();

    // all combination for two parameters
    int cnt1 = N / size1;
    vector<string> str_vec1;
    for(int i=0; i<size1; i++){
        for(int j=0; j<cnt1; j++){
            str_vec1.push_back(Util::Int2String(i));
        }
    }
    this->matrix->AddCol(str_vec1);

    int cnt2 = N / size2;
    vector<string> str_vec2;
    for(int j=0; j<cnt2; j++){
        for(int i=0; i<size2; i++){
            str_vec2.push_back(Util::Int2String(i));
        }
    }
    this->matrix->AddCol(str_vec2);

    this->matrix->paramSizeVector.resize(2);
    this->matrix->paramSizeVector[0] = size1;
    this->matrix->paramSizeVector[1] = size2;
    this->matrix->MakeParamSizeVector();

    this->Size = this->matrix->rowSize;
    
    return true;
}
//-----------------------------------------------------------------------------
bool  OrthogonalArray::MakeGF()
{
    TRACER("> OrthogonalArray::MakeGF start")
    
    AlgorithmGF *gf = new AlgorithmGF(this->progInfo);
    
    this->matrix = gf->Generate(this->modelFile);
    if(this->matrix == 0){
        return false;
    }
    
    this->baseArray   = gf->baseArray;
    this->elementList = gf->elementList;
    
    this->Size = this->matrix->rowSize;

    // cannot generate into array
    if(this->matrix->rowSize == 0 || this->matrix->colSize == 0){
        return false;
    }

    return true;
    
}

//-----------------------------------------------------------------------------
bool  OrthogonalArray::MakeXu()
{
    TRACER("> OrthogonalArray::MakeXu start")

    AlgorithmXu *xu = new AlgorithmXu(this->progInfo);
    
    this->matrix = xu->Generate(this->modelFile);
    if(this->matrix == 0){
        return false;
    }
    
    // cannot generate into array
    if(this->matrix->rowSize == 0 || this->matrix->colSize == 0){
        return false;
    }

    this->Size = this->matrix->rowSize;
    
    return true;
}

//-----------------------------------------------------------------------------
bool  OrthogonalArray::MakeBigCA()
{
    TRACER("> OrthogonalArray::MakeBigCA start")
    
    AlgorithmBigCA *bigca = new AlgorithmBigCA(this->progInfo, this->modelFile);

    if(this->progInfo->isStream){
        bigca->PrintStreamData();
        return true;
    }else{
        this->matrix = bigca->GetMatrix();
        if(this->matrix == 0){
            return false;
        }
    }

    // cannot generate into array
    if(this->matrix->rowSize == 0 || this->matrix->colSize == 0){
        return false;
    }

    this->Size = this->matrix->rowSize;
    return true;
}

//-----------------------------------------------------------------------------
bool  OrthogonalArray::MakeCA()
{
    TRACER("> OrthogonalArray::MakeCA start")
    
    AlgorithmCA *ca = new AlgorithmCA(this->progInfo, this->modelFile);

    if(this->progInfo->isStream){
        ca->PrintStreamData();
        return true;
    }else{
        this->matrix = ca->GetMatrix();
        if(this->matrix == 0){
            return false;
        }
    }
    
    // cannot generate into array
    if(this->matrix->rowSize == 0 || this->matrix->colSize == 0){
        return false;
    }

    this->Size = this->matrix->rowSize;
    return true;
}

//-----------------------------------------------------------------------------
bool  OrthogonalArray::MakeAll()
{
    TRACER("> OrthogonalArray::MakeAll start")
    
    AlgorithmAll *all = new AlgorithmAll(this->progInfo, this->modelFile);

    if(this->progInfo->isStream){
        all->PrintStreamData();
        return true;
    }else{
        this->matrix = all->GetMatrix();
        if(this->matrix == 0){
            return false;
        }
    }

    // cannot generate into array
    if(this->matrix->rowSize == 0 || this->matrix->colSize == 0){
        return false;
    }

    this->Size = this->matrix->rowSize;
    
    return true;
}
//-----------------------------------------------------------------------------
bool  OrthogonalArray::MakeRandom()
{
    TRACER("> OrthogonalArray::MakeRandom start")

    // for dll 
//    if(this->progInfo->isCombinatorialTesting){
//        return true;  // NOP
//    }
    
    AlgorithmRandom *random = new AlgorithmRandom(this->progInfo);


    this->matrix = random->Generate(this->modelFile);
    if(this->matrix == 0){
        if(this->progInfo->isStream){
            return true;
        }else{
            return false;
        }
    }

    // cannot generate into array
    if(this->matrix->rowSize == 0 || this->matrix->colSize == 0){
        return false;
    }

    this->Size = this->matrix->rowSize;
    
    return true;
}


//-----------------------------------------------------------------------------
void  OrthogonalArray::ChangeColumn()
{
    Matrix *newMatrix = new Matrix(this->matrix->rowSize, 
                                   this->matrix->colSize); 
    
    
    vector<string> newElementList;
    
    // there is not elementList when Xu argorithm
    if(this->elementList.size() != 0) {
        newElementList.resize(this->elementList.size());
    }
    
    assert( this->paramCount==this->matrix->colSize );
    
    for(int col=0; col < this->matrix->colSize; col++){
        int orgPos = this->modelFile->testParamRecords[col]->orignal_pos;
        
        if(this->elementList.size() != 0) {
            newElementList[orgPos] = this->elementList[col];
        }
        
        for(int j=0; j<this->matrix->rowSize; j++){
            newMatrix->Set(j, orgPos, this->matrix->Get(j, col) ) ;
        }
        
    } // end of for (col)
    
    
    this->matrix      = newMatrix ;

    if(this->elementList.size() != 0) {
        this->elementList = newElementList;
    }
     
}
//-----------------------------------------------------------------------------
void  OrthogonalArray::SetItemData()
{
    
    this->resultArray.resize( this->matrix->rowSize );
    for(int i=0; i<this->matrix->rowSize; i++){
        this->resultArray[i].resize(this->matrix->colSize); 
        
        for(int j=0; j<this->matrix->colSize; j++){
            int val = this->matrix->GetInt(i,j);
            TestParameter *testParam = this->modelFile->testParamRecords[j];
            this->resultArray[i][j] = testParam->paramValues[val];
        }
    }
    
    if(this->modelFile->constraintRecords.size() == 0){
        // no Constraints
        this->resultArrayWithConstraints.resize(0);
    }
    
    for(int i=0; i<this->resultArray.size(); i++){
        if(isValitRecord(this->resultArray[i])){
            this->resultArrayWithConstraints.push_back(
                               this->resultArray[i]);
        }
    }
}

//-----------------------------------------------------------------------------
bool  OrthogonalArray::isValitRecord(vector<string> resultRec)
{
    ModelFile *modelFile = this->modelFile;
    
    for(int i=0; i<modelFile->constraintRecords.size(); i++){
        Constraint  *constraint = modelFile->constraintRecords[i];
        
        if(// for if
           CheckConstraint(resultRec, 
                           constraint->if_target,
                           constraint->if_operator,
                           constraint->if_value)       && 
           // for then
           !CheckConstraint(resultRec, 
                            constraint->then_target,
                            constraint->then_operator,
                            constraint->then_value)    ) {   
            return false;
        }
    }
    
    return true;
}
//-----------------------------------------------------------------------------
bool OrthogonalArray::CheckConstraint(vector<string> resultRec, 
                                      string target, string ope, string value)
{
    int paramNum   = Name2Num(target);
    
    if(ope == "=" || ope == "=="){
        if(resultRec[paramNum] == value){
            return true;
        }
    }
    if(ope == "<>" || ope == "!="){
        if(resultRec[paramNum] != value){
            return true;
        }
    }
    if(ope == ">=" || ope == "=>"){
        if(resultRec[paramNum] >= value){
            return true;
        }
    }
    if(ope == ">"){
        if(resultRec[paramNum] > value){
            return true;
        }
    }
    if(ope == "<=" || ope == "=<"){
        if(resultRec[paramNum] <= value){
            return true;
        }
    }
    if(ope == "<"){
        if(resultRec[paramNum] < value){
            return true;
        }
    }
    
    return false;
}

//-----------------------------------------------------------------------------
int  OrthogonalArray::Name2Num(string paramName)
{
    vector<TestParameter *> testParamRecords = 
                  this->modelFile->testParamRecords;

    for(int i=0; i<testParamRecords.size(); i++){
        if(testParamRecords[i]->paramName == paramName){
            return i;
        }
    }
    
    cout << "[X] invalid paramName." << endl;
    cout << "    " << paramName << endl;
    
    return -1;
}
    

//-----------------------------------------------------------------------------
