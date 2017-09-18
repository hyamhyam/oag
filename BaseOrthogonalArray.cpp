// Copyright 2011 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include "BaseOrthogonalArray.h"


//-----------------------------------------------------------------------------
// Constructor
//
//  level : 2 level (2**n) or 3 level (3**n) ...
//  size  : 32 if L32
//
BaseOrthogonalArray::BaseOrthogonalArray()
{
}

BaseOrthogonalArray::BaseOrthogonalArray(int level, int size)
{
    this->baseLevel = level;
    this->Size      = size;
    
    // make minimum base matrix
    //   L2 = {{0}, {1}}
    //   L3 = {{0}, {1}, {2}}
    //    :     :
    //   Ln = {{0}, {1}, {2} ... {n-1}}
    //
    Matrix  *baseMatrix = new Matrix(level, 1); 
    for(int i=0; i<level; i++){
        baseMatrix->Set(i, 0, Util::Int2String(i));
    }
    //baseMatrix->Information();
    
    this->elementList = MakeElementList(size, false);
    
    while(true){
        
        // backup 
        baseMatrixList.push_back(baseMatrix);
        
        if(baseMatrix->rowSize == size){ 
            break;
        }
        assert( baseMatrix->rowSize < size ) ;
        
        // L2 ==> L4 ==> L8 ...
        // L3 ==> L9 ==> L27 ...
        baseMatrix = this->Increase(baseMatrix);
    }

//    baseMatrix->Information();
    
    this->matrix = baseMatrix;
    
    return;
}


//-----------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------
// private method
//-----------------------------------------------------------------------------
//
// current     next
//    M1  ===>  M2
//
//  ex1.
//   M1(2,1)  ===>  M2(4,3) 
//
//          *1      *2      *3
//    0 ==> 000 ==> 000 ==> 000
//    1     000     010     011
//          100     100     101
//          100     110     110
//                  
//  ex2.
//   M1(4,3)  --->  M2(8,7) 
//
//            *1          *2          *3
//   000 ==> 0000000 ==> 0000000 ==> 0000000 
//   011     0000000     0001000     0001111
//   101     0110000     0110000     0110011
//   110     0110000     0111000     0111100
//           1010000     1010000     1010101
//           1010000     1011000     1011010
//           1100000     1100000     1100110
//           1100000     1101000     1101001
//
Matrix  *BaseOrthogonalArray::Increase(Matrix *M1)
{
    //cout << "baseMatrix::Increase  M1->rowSize=" << M1->rowSize << endl ;

    int M1row = M1->rowSize;
    int M1col = M1->colSize;
    
    int M2row = M1row * this->baseLevel;
    int M2col = (M1col * this->baseLevel) + 1 ;
    
    Matrix *M2 = new Matrix(M2row, M2col); 
    
    // move M1 data to M2
    int level = this->baseLevel;

    // *1
    for(int i=0; i<M1row; i++){
        for(int j=0; j<M1col; j++){
            for(int k=0; k<level; k++){
                M2->Set(i*level + k, j, M1->Get(i,j) );
            }
        }
    }

    // *2
    for(int i=0; i<M2row; i++){
        M2->Set(i, M1col, Util::Int2String(i%level) );
    }
    
    // *3
    for(int i=0; i<M2row; i++){
        for(int j=0; j<M1col; j++){
            int z = level - 1;
            for(int k=0; k<z; k++){
                int gf = Util::GFadd(level, k+1, 
                                 M2->GetInt(i,j), M2->GetInt(i,M1col));
                M2->Set(i, (j*z)+M1col+1+k, Util::Int2String(gf));
            }
        }
    }
    
//    M2->Information();
    
    return  M2;
}
//-----------------------------------------------------------------------------
//
//  input  : size    16 if L16
//  output : (a, b, ab, c, ac, bc ,abc, ...)     element name list. 
//            <----  elementListSize  ----> 
//
vector<string>  BaseOrthogonalArray::MakeElementList(int size, bool withBrackets)
{
//    cout << "## BaseOrthogonalArray::MakeElementList start" << endl;

    vector<string> str_vec;
    int level = this->baseLevel;
    
    if(size<=3){
        str_vec.resize(0);
        return str_vec;
    }
    
    // ex.
    // baseLevel=2
    //    size                 2   4   8  16 ...
    //    elementListSize      1   3   7  15 ...
    //
    // baseLevel=3
    //    size                 3   9  27  81 ...
    //    elementListSize      1   4  13  40 ...
    //
    int elementListSize = (size - 1) / (level - 1);
    
    str_vec.resize(elementListSize, "");
    
    str_vec[0] = "a";
    
    if(size == level){
        return str_vec;   // because elementListSize == 1
    }
    
    int  pos = 1 ;            // Util::Num2Alpha(1) ===> "b"
    int  currentSize = 1;
    
    while(true){
        str_vec[currentSize] = Util::Num2Alpha(pos);
        pos++;
        
        for(int i=0; i<currentSize; i++){
            int target = currentSize + 1 + (i* (level - 1));
            for(int k=0; k<(level - 1); k++){
                if(k==0){
                    str_vec[target + k] = str_vec[i] + str_vec[currentSize];
                }else{
                    if(withBrackets){
                        // 2*a + b ===> 2(a)b
                        str_vec[target + k] = Util::Int2String(k+1) 
                                                 + "(" + str_vec[i] + ")"
                                                 + str_vec[currentSize];
                    }else{
                        string workElement = "";
                        for(int m=0; m<(k+1); m++){
                            workElement += str_vec[i];
                        }
                        
                        // ex.
                        //  2*c + bc ===> ccbc
                        workElement += str_vec[currentSize];
                        
                        // sort workElement
                        //    ccbc ===> bccc
                        str_vec[target + k] = Util::SortStringChar(workElement);
                    }
                }
            } //end of for(k)
        } //end of for(i)
        
        // level=2  1==>3==>7==>15 ...
        // level=3  1==>4==>13==>40 ...
        currentSize = ( currentSize * level ) + 1 ;
        if(currentSize >= elementListSize) {
            break;
        }
    } // end of while
    
    
//    cout << "## BaseOrthogonalArray::MakeElementList end" << endl;
    return str_vec;
}
//-----------------------------------------------------------------------------
//
//  cols (2, 4, 5)
//
//  this->matrix
//     0   1   2   3   4   5   6
//   +---+---+---+---+---+---+---+
//   |   |   | 0 |   | 0 | 0 |   |
//   |   |   | 1 |   | 1 | 0 |   |
//   |   |   | 0 |   | 1 | 1 |   |
//   |   |   | 1 |   | 0 | 1 |   |
//   :   :   :   :   :   :   :   :
//   +---+---+---+---+---+---+---+
//             |       |   |               L4 base array
//             +-----+-+---+        +------> 0,0,0   i=0
//                   |              |        0,1,1   i=1 <---+
//                   V              |        1,0,1   i=2 <---|--+
//             +---+---+---+        |   +--> 1,1,0   i=3     |  |
//             | 0 | 0 | 0 | <------+   |                    |  |
//             | 1 | 1 | 0 | <----------+                    |  |
//             | 0 | 1 | 1 | <-------------------------------+  |
//             | 1 | 0 | 1 | <----------------------------------+
//             :   :   :   :  
//             +---+---+---+  
//                   |        
//                   V        
//                 rtnList
//                  +---+ 
//                  | 0 | 
//                  | 3 | 
//                  | 1 | 
//                  | 2 | 
//                  :   :
//                  +---+
//
//  input  : 2,3,5
//  output : 0,3,1,2, ...
//
vector<string>  BaseOrthogonalArray::GetJoinedColumns(vector<int> cols)
{

    vector<string> rtnList;
    rtnList.resize(this->matrix->rowSize);
    
    for(int i=0; i<this->matrix->rowSize; i++){
        vector<string> dummy;
        dummy.resize(cols.size());
        
        for(int j=0; j<cols.size(); j++){
            dummy[j] = this->matrix->Get(i, cols[j]) ;
        }
        
        // ex.
        //  dummy=(1,1,0)
        //     1,1,0 ===> "110" ===> 3
        rtnList[i] = TrimData(dummy);    
        
        if(rtnList[i] == "") {
            rtnList.resize(0);
            return rtnList;
        }
        
    } // end of for(i)
    
    return  rtnList;
}
//-----------------------------------------------------------------------------
//  ex.
//   dummy=(1,1,0)
//                                 return value
//            L4 base matrix          |
//             +---+---+---+          V
//             | 0 | 0 | 0 |        i=0
//             | 1 | 1 | 0 |          1 
//             | 0 | 1 | 1 |          2  
//             | 1 | 0 | 1 |          3    
//             +---+---+---+                  
//                             
//  baseLevel=2
//   matrixNum             dummy.size (=arraySize)
//       0          L2        1
//       1          L4        3
//       2          L8        7
//       3          L16      15
//       4          L32      31
//       5          L64      63
//       :           :        :
//
//  baseLevel=3
//   matrixNum             dummy.size (=arraySize)
//       0          L3        1
//       1          L9        4
//       2          L27      13 
//       3          L81      40
//       :           :        :
//
string  BaseOrthogonalArray::TrimData(vector<string> dummy)
{
//    cout << "dummy in TrimDat:" ;
//    Util::PrintVector(dummy);
    
    int arraySize = dummy.size();
    if(arraySize == 1){
        return dummy[0];
    }
    
    int tmp = (arraySize * (this->baseLevel - 1) ) + 1 ;

     int matrixNum = Util::Log(tmp, this->baseLevel);

    matrixNum = matrixNum - 1;  
    // level=2 : baseMatrixList[0] = L2 ,  [1]=L4  ...
    // matrixNum =              0 ,         1,     ...
    
    Matrix *smallMatrix = this->baseMatrixList[matrixNum];
    
    for(int recNum=0; recNum<smallMatrix->rowSize; recNum++){
        if(smallMatrix->GetRow(recNum) == dummy){
           // cout << "dummy is match" << endl;
            return Util::Int2String(recNum);
        }
    }

//cout << "dummy is NOT match" << endl;


    return  "";  // error
}


