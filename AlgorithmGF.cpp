// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.


#include "AlgorithmGF.h"

#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }

//-----------------------------------------------------------------------------
// Constructor
AlgorithmGF::AlgorithmGF(ProgInfo *progInfo)
{
    
    this->progInfo   = progInfo;

}

//-----------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------
Matrix *AlgorithmGF::Generate(ModelFile *modelFile)
{
    
    if(!modelFile->Complement()){
        TRACER("> OrthogonalArray::MakeGF,Complement false")
        return 0;
    }
    
    this->Size  = modelFile->GetOASize();
    if(this->Size <= 0){
        TRACER("> OrthogonalArray::MakeGF GetOASize false")
        return 0;
    }
    
    while(true) {
        if(modelFile->maxCombinationCnt > 0 &&
            this->Size > modelFile->maxCombinationCnt){
            cout << "# [X] array size is larger than max combination("
                 << modelFile->maxCombinationCnt << ")." <<endl; 
            return 0;
        }
        if(this->Size >= 10000){
            // temporary restriction size (hyam)
            cout << "# [X] too large array size(>=10,000)" <<endl; 
            return 0;
        }
        
        if(Try2MakeGF(modelFile, this->Size)){
            // Orthogonal Array is generated successfnlly
            break;
        }else{
            if(this->progInfo->isDebug){
                cout << "# [i] cannot generate. try to next upper level" <<endl;
                cout << "    " << this->Size << " ===> "
                     << (this->Size * this->baseArray->baseLevel) <<endl;
            }
        }
        
        // try to next size array
        this->Size = this->Size * this->baseArray->baseLevel ;
        
    }  // end of while for Try2MakeGF loop
    
    
    return this->matrix;

}

//-----------------------------------------------------------------------------
// size = 8 if L8,   27 if L27 ...
//
//  modelFile
//    A:1,2,3,4     size_vec=(4,2,2)
//    B:1,2
//    C:1,2
//
//    a,b,ab,c,ac,bc,abc  ===   a:b:ab,  c,   ac
//    0 1 2  3 4  5  6        ((0,1,2) , (3), (4) ) = cols_vec
//
bool  AlgorithmGF::Try2MakeGF(ModelFile *modelFile, int size)
{

    this->baseArray = new BaseOrthogonalArray
                               (this->progInfo->baseLevel, size);
    
    this->linearGraph = new LinearGraph(this->baseArray);
    
    this->matrix = new Matrix(size, modelFile->paramCount);
    
    this->elementList.resize(modelFile->paramCount);
    
    vector< vector<int> > cols_vec;
    vector<int> size_vec;
    for(int i=0; i<modelFile->testParamRecords.size(); i++){
        TestParameter *testParam = modelFile->testParamRecords[i] ;
        size_vec.push_back(testParam->paramValues.size());
    }
    
    cols_vec = GetColsVector(size_vec);
    if(cols_vec.size()==0){ 
        if(this->progInfo->isDebug){
            cout << "# [i] cannot get columns data "  
                 <<  "from current size LinearGraph" << endl;
        }
        return false; 
    }
    
    for(int i=0; i<cols_vec.size(); i++){
        vector<int> cols = cols_vec[i];
        
        vector<string> joinedCol = this->baseArray->GetJoinedColumns(cols);
       
        if(joinedCol.size()==0){ 
            if(this->progInfo->isDebug){
                cout << "# [i] cannot get joined column data" << endl;
            }
            return false; 
        }
        
        this->matrix->SetColumnData(i, joinedCol);
        
        
        string newElement = "";
        for(int j=0; j < cols.size(); j++){
            if(newElement != ""){
                newElement += ":" ;
            }
            newElement += this->baseArray->elementList[cols[j]] ;
        } // end of for(j)   
        
        this->elementList[i] = newElement ;
        
    }  // end of for(i)  
    
    
    return true;
}
//-----------------------------------------------------------------------------
//
//  modelFile
//    A:1,2,3,4   
//    B:1,2
//    C:1,2
//
//  input   size_vec = (4,2,2)
//  output  cols_vec = ((0,1,2) , (3), (4) 
//
vector< vector<int> >  
    AlgorithmGF::GetColsVector(vector<int> size_vec)
{
    // Util::PrintVector(size_vec);
    
    this->linearGraph->ClearUsedColumn();
    
    vector< vector<int> > cols_vec;
    vector<int> cols;
    
    for(vector<int>::iterator ite = size_vec.begin();
        ite != size_vec.end();  ++ite){
        
        int paramSize = *ite ;
        
        cols = this->linearGraph->GetCols(paramSize);
        if(cols.size()==0){ 
            cols_vec.resize(0);
            return cols_vec; 
        }
        
        cols_vec.push_back(cols);
    }
    
    return cols_vec;
}

//-----------------------------------------------------------------------------
