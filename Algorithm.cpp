// Copyright 2011, 2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include "Algorithm.h"

#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }

//-----------------------------------------------------------------------------
// Constructor
Algorithm::Algorithm(ProgInfo *progInfo, ModelFile *modelFile)
{
    this->progInfo    = progInfo;
    this->modelFile   = modelFile;

    this->recordCount = 0;
}

//-----------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------
Matrix *Algorithm::Generate(ModelFile *modelFile)  
{
    return 0;

}


//-----------------------------------------------------------------------------
// private
//-----------------------------------------------------------------------------
