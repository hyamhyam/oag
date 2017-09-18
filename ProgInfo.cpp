// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include "ProgInfo.h"
#include "Util.h"

//-----------------------------------------------------------------------------
// Constructor
ProgInfo::ProgInfo(int argc, char **argv)
{
    // adhoc GetOpt.  Do improvement later
    
    // default setting
    this->programName = "OAG (Orthogonal Array Generator)";
    this->version = "0.6.3";

    this->isValid       = false;
    this->inputDataFile = "";
    
    this->enableConstraint      = true;
    this->enableComplementation = false; 
    this->enableMeasuring       = false;
    
    this->noD = false;
    
    this->isDebug          = false;
    this->isTableCheck     = false;
    this->isTrace          = false;
    this->isQuiet          = false;
    this->isStream         = false;
    this->isNoParamName    = false;
    this->ShowInfo         = false;
    this->TimeSeed         = false;
    this->tryCount         = 10;
    
    this->baseLevel        = 0;
    this->rowSize          = 0;
    
    this->inDelimiter      = ",";
    this->outDelimiter     = "\t";
                        
    this->algorithm   = "";
     

    this->coverageWay  = 3; 
    this->strength     = 2; 
    
    if(argc <= 0){
        this->isValid = false;
        return;
    }
    
    
    // cnt=0 is exename, not option
    for(int cnt=1; cnt<argc; cnt++){    // (1)
    

        if(!Util::StrIcmp(argv[cnt],"-help") ||
           !Util::StrIcmp(argv[cnt],"-h") ){
            this->isValid = false;
            return;
        }
        if(!Util::StrIcmp(argv[cnt],"-version") ||
           !Util::StrIcmp(argv[cnt],"-v") ){
            cout << this->programName << "  " ;
            cout << this->version << endl;
            this->isValid = false;
            exit(0);
        }
        
        if(!Util::StrIcmp(argv[cnt],"-debug") || 
           !Util::StrIcmp(argv[cnt],"-d") ){
            this->isDebug = true;
            continue;                              // return to (1)
        }
        if(!Util::StrIcmp(argv[cnt],"-show") ||
           !Util::StrIcmp(argv[cnt],"-s") ){
            this->ShowInfo = true;
            continue;                              // return to (1)
        }
        if(!Util::StrIcmp(argv[cnt],"-quiet") || 
           !Util::StrIcmp(argv[cnt],"-q") ){
            this->isQuiet = true;
            continue;                              // return to (1)
        }
        
        // not open facility
        if(!Util::StrIcmp(argv[cnt],"-trace") ||
           !Util::StrIcmp(argv[cnt],"-tr") ){
            this->isTrace = true;
            continue;                              // return to (1)
        }

        if(!Util::StrIcmp(argv[cnt],"-noConstraint") || 
           !Util::StrIcmp(argv[cnt],"-x1") ){
            this->enableConstraint = false;
            continue;                              // return to (1)
        }

        if(!Util::StrIcmp(argv[cnt],"-Elapsed") || 
           !Util::StrIcmp(argv[cnt],"-ep") ){
            this->enableMeasuring = true;
            continue;                              // return to (1)
        }

        if(!Util::StrIcmp(argv[cnt],"-NoParamName") || 
           !Util::StrIcmp(argv[cnt],"-np") ){
            this->isNoParamName = true;
            continue;                              // return to (1)
        }
   
        if(!Util::StrIcmp(argv[cnt],"-noD") || !Util::StrIcmp(argv[cnt],"-xD") ){
            this->noD = true;
            continue;                              // return to (1)
        }
        
        
        if(!Util::StrIcmp(argv[cnt],"-noStream") || !Util::StrIcmp(argv[cnt],"-xS") ){
            this->isStream = false;
            continue;                              // return to (1)
        }
        if(!Util::StrIcmp(argv[cnt],"-Stream") || !Util::StrIcmp(argv[cnt],"-sm") ){
            this->isStream = true;
            continue;                              // return to (1)
        }


        if(!Util::StrIcmp(argv[cnt],"-check") || !Util::StrIcmp(argv[cnt],"-c") ){
            this->isTableCheck = true;
            if( this->inDelimiter == ","){
                this->inDelimiter      = "\t";
            }
            continue;                              // return to (1)
        }
 
        if(!Util::StrIcmp(argv[cnt],"-level") || !Util::StrIcmp(argv[cnt],"-l") ){
            cnt++;
            if(cnt==argc){
                this->isValid = false;
                return;
            }
            if(argv[cnt][0] == '-'){
                this->isValid = false;
                return;
            }
            this->baseLevel = atoi(argv[cnt]);
            continue;                              // return to (1)
        }
        
        if(!Util::StrIcmp(argv[cnt],"-TimeSeed") ||
           !Util::StrIcmp(argv[cnt],"-ts") ){
            this->TimeSeed = true;
            continue;                              // return to (1)
        }
        
        if(!Util::StrIcmp(argv[cnt],"-way") || !Util::StrIcmp(argv[cnt],"-w") ){
            cnt++;
            if(cnt==argc){
                this->isValid = false;
                return;
            }
            if(argv[cnt][0] == '-'){
                this->isValid = false;
                return;
            }
            this->coverageWay = atoi(argv[cnt]);
            if(this->coverageWay <= 1 ){
                this->isValid = false;
                return;
            }
                
            continue;                              // return to (1)
        }

        if(!Util::StrIcmp(argv[cnt],"-strength") || 
           !Util::StrIcmp(argv[cnt],"-st") ){
            cnt++;
            if(cnt==argc){
                this->isValid = false;
                return;
            }
            if(argv[cnt][0] == '-'){
                this->isValid = false;
                return;
            }
            this->strength = atoi(argv[cnt]);
            if(this->strength <= 1 ){
                this->isValid = false;
                return;
            }
                
            continue;                              // return to (1)
        }


        if(!Util::StrIcmp(argv[cnt],"-algorithm") ||
           !Util::StrIcmp(argv[cnt],"-a") ){
            cnt++;
            if(cnt==argc){
                this->isValid = false;
                return;
            }
            if(argv[cnt][0] == '-'){
                this->isValid = false;
                return;
            }
            this->algorithm = argv[cnt] ;
            if(!Util::StrIcmp(argv[cnt],"GF")){
                this->enableComplementation = true;
            }
            if(!Util::StrIcmp(argv[cnt],"all")    ||
               !Util::StrIcmp(argv[cnt],"Random") ||
               !Util::StrIcmp(argv[cnt],"BigCA")     ){ // temporary
                this->isStream  = true;
            }
            continue;                              // return to (1)
        }

        if(!Util::StrIcmp(argv[cnt],"-t") ){
            cnt++;
            if(cnt==argc){
                this->isValid = false;
                return;
            }
            if(argv[cnt][0] == '-'){
                this->isValid = false;
                return;
            }
            this->tryCount = atoi(argv[cnt]) ;
            continue;                              // return to (1)
        }

        if(!Util::StrIcmp(argv[cnt],"-size") || 
           !Util::StrIcmp(argv[cnt],"-sz")){
            cnt++;
            if(cnt==argc){
                this->isValid = false;
                return;
            }
            if(argv[cnt][0] == '-'){
                this->isValid = false;
                return;
            }
            this->rowSize = atoi(argv[cnt]) ;
            continue;                              // return to (1)
        }


        if(!Util::StrIcmp(argv[cnt],"-indelimit") || 
           !Util::StrIcmp(argv[cnt],"-id") ){
            cnt++;
            if(cnt==argc){
                this->isValid = false;
                return;
            }
            if(argv[cnt][0] == '-'){
                this->isValid = false;
                return;
            }
            this->inDelimiter = argv[cnt];
            if(this->inDelimiter == "\" \"" || 
               Util::StrIcmp(argv[cnt],"space")==0 ){
                this->inDelimiter = " ";
            }
            if(this->inDelimiter == "\\t" ||
               Util::StrIcmp(argv[cnt],"tab")==0){
                this->inDelimiter = "\t";
            }
            continue;                              // return to (1)
        }
        
        if(!Util::StrIcmp(argv[cnt],"-outdelimit") ||
           !Util::StrIcmp(argv[cnt],"-od") ){
            cnt++;
            if(cnt==argc){
                this->isValid = false;
                return;
            }
            if(argv[cnt][0] == '-'){
                this->isValid = false;
                return;
            }
            this->outDelimiter = argv[cnt];
            continue;                              // return to (1)
        }

        
        // unknown option
        if(argv[cnt][0] == '-'){
            this->isValid = false;
            return;
        }

        // no option
        this->inputDataFile = argv[cnt];   // mandatory
        this->isValid = true;             
        
    } // end of for
    
    
    // check relation of options 
    if(this->algorithm == "" && this->isStream == true){
        this->isStream = false;
    }            
    
}
//-----------------------------------------------------------------------------
// Constructor #2      for dll
ProgInfo::ProgInfo()
{
    
    // default setting
    this->programName = "OAG (Orthogonal Array Generator)";

    this->isValid       = false;
    this->inputDataFile = "";
    
    this->enableConstraint      = true;
    this->enableComplementation = false; 
    
    this->noD = false;
    
    this->isDebug          = false;
    this->isTableCheck     = false;
    this->isTrace          = false;
    this->isQuiet          = false;
    this->ShowInfo         = false;
    this->TimeSeed    = false;
    this->tryCount         = 10;
    
    this->baseLevel        = 0;
    this->rowSize          = 0;
    
    this->inDelimiter      = ",";
    this->outDelimiter     = "\t";
                        
    this->algorithm   = "";
}



//-----------------------------------------------------------------------------
// public method
void ProgInfo::usage()
{
    cout << "USAGE : \n";
    cout << "  <generate>\n";
    cout << "      OAG  model-file-name\n";
    cout << "\n";
    cout << "  <check>\n";
    cout << "      OAG  -check  array-file \n";
    cout << "\n";
    cout << "  <options for generation>\n";
    cout << "      -algorithm (GF|Xu|all|random|CA|BigCA) | -a (GF|Xu|all|random|CA|BigCA)  \n";
    cout << "            : select an algorithm\n";
    cout << "               <Orthogonal Array>\n";
    cout << "                 no -algorithm : try Template -> GF -> Xu\n";
    cout << "                 GF : EX-OR extension\n";
    cout << "                 Xu : Hongquan Xu\n";
    cout << "               <other Arrays>\n";
    cout << "                 all : All Combination\n";
    cout << "                 random : random combination\n";
    cout << "                 CA : Covering Array (Pairwise:strength=2 only)\n";
    cout << "                 BigCA : Covering Array for very large model (experimental)\n";
    
    cout << "      -indelimit  char | -id char : input-data delimiter  (defalut=,)\n";
    cout << "      -outdelimit char | -od char : output-data delimiter (defalut=tab)\n";

    cout << "      -show | -s  : show array information\n";
    cout << "      -way num | -w num : caliculate num-way coverage in -show mode\n";

    cout << "      -strength | -st : strength of test array for BigCA\n";

    cout << "      -noConstraint | -x1  : disable constraint records\n";

    cout << "      -T num : specify repeat count for algorithm Xu\n";
    cout << "      -size num | -sz num  : specify row(run) size\n";
    
    cout << "      -TimeSeed | -ts : use time for random seed\n";

    cout << "      -noD | -xD  : do not add (D) for complementation\n";
    cout << "      -Stream   | -sm  : stream mode\n";
    cout << "      -noStream | -xS  : not stream mode\n";
    cout << "      -level num | -l num  : level for test parameters\n";
    cout << "      -quiet | -q  : no verbose message\n";
    cout << "      -elapsed | -ep : display elapsed time\n";
    


    cout << "\n";
    cout << "  <options for checking>\n";
    cout << "      -check | -c  : check array data\n";
    cout << "      -indelimit char | -id char : input-data delimiter (defalut=tab)\n";
    cout << "      -NoParamName | -np : no parameter name record\n";
    cout << "      -way num | -w num : caliculate num-way coverage in -show mode\n";
    cout << "      -elapsed | -ep : display elapsed time\n";


    cout << "\n";
    cout << "  <other options>\n";
    cout << "      -version | -v : print program version\n";
    cout << "      -help    | -h : help\n";


    cout << "\n";
    cout << " [valid option for each algorithm]\n";
    cout << "                  GF Xu all random CA BigCA\n";
    cout << "   -level         o  x  x    x     x   x \n";
    cout << "   -noD           o  x  x    x     x   x \n";
    cout << "   -T             x  o  x    x     x   x \n";
    cout << "   -TimeSeed      x  o  x    o     x   x \n";
    cout << "   -size          x  o  x    o     x   x \n";
    cout << "   -stream        x  x  d    d     o   d \n";
    cout << "   -noStream      d  d  o    o     d   o \n";
    cout << "   -show          o  o  o    o     o   x -noStream required\n";
    cout << "   -way           o  o  o    o     o   x -noStream required\n";
    cout << "   -strength      x  x  x    x     x   o \n";
    cout << "   -indelimit     o  o  o    o     o   o \n";
    cout << "   -outdelimit    o  o  o    o     o   o \n";
    cout << "   -noConstraint  o  o  o    o     o   x -noStream required\n";
    cout << "   -quiet         x  o  x    x     x   x \n";
    cout << "\n";
    cout << "      o:valid, x:invalid, d:valid and default\n";
    cout << "\n";


}


