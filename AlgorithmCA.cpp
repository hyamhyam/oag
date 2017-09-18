// hyam <hyamhyam@gmail.com>
//
// This source is based on the QICT
// I(hyam) rewrite QICT source in C++.
//
// original QICT source is wriiten by C#
// http://archive.msdn.microsoft.com/mag1209TestRun/Release/ProjectReleases.aspx?ReleaseId=3657
//
#include "AlgorithmCA.h"

#define TRACER(MSG)  \
    if(this->progInfo->isTrace){  \
        cout << MSG << endl ;  \
    }

//-----------------------------------------------------------------------------
// Constructor
AlgorithmCA::AlgorithmCA(ProgInfo *progInfo, ModelFile *modelFile)
    : Algorithm(progInfo, modelFile)
{
    // NOP.  use super class constructor
}

//-----------------------------------------------------------------------------
// public method
//-----------------------------------------------------------------------------
Matrix *AlgorithmCA::GetMatrix()
{
    this->matrix = new Matrix();

    if(this->modelFile->totalParameterValuesCnt <= 0 ){
        // temporary restriction size (hyam)
        cout << "# [X] too large array size(AlgorithmCA)" <<endl; 
        return 0;
    }
    
    
    MakeCA();
    
    for(long k=0; k<CA.size(); k++){
        this->matrix->AddRow(CA[k]);
    }

    return this->matrix;

}
//-----------------------------------------------------------------------------
void AlgorithmCA::PrintStreamData()
{

    if(this->modelFile->totalParameterValuesCnt <= 0 ){
        // temporary restriction size (hyam)
        cout << "# [X] too large array size(AlgorithmCA)" <<endl; 
        return ;
    }
    
    this->modelFile->PrintAllParamName();
    
    MakeCA();
    
    for(long k=0; k<CA.size(); k++){
        string delim = this->modelFile->progInfo->outDelimiter ;
        vector<string> result = 
            this->modelFile->Element2Value(this->CA[k]);
        Util::PrintVector(result, delim);
    }

    return ;

}

//-----------------------------------------------------------------------------
vector<int>  AlgorithmCA::GetTestRecord()
{
    
    if(this->recordCount == 0 ){
        MakeCA();
    }
    
    vector<int> vec_col = CA[this->recordCount]   ;
    
    

    this->recordCount++;
    return vec_col ;

}

//-----------------------------------------------------------------------------
// private
//-----------------------------------------------------------------------------
//  using qict algorithm (only strength 2)
//
void AlgorithmCA::MakeCA (){

    
    int numberParameters = this->modelFile->paramCount;
    int numberParameterValues = this->modelFile->totalParameterValuesCnt;
    int numberPairs = 0;
    
    // number of candidate testSet arrays to generate 
    // before picking one to add to testSets List
    int poolSize = 20; 
 
    // one-dimensional array of all parameter values
    vector<string> parameterValues ; 
    
    vector< int* > unusedPairs ; // changes
    vector< int* > testSets ;    // the main result data structure



    int **legalValues;
    legalValues = new int*[numberParameters];
    
    int *legalValuesEachSize;
    legalValuesEachSize = new int[numberParameters];
    
    parameterValues.resize(numberParameterValues);
    int *parameterValuePositon = new int[numberParameterValues];
    
    int currRow = 0;
    int kk = 0; // points into parameterValues
    
    for(int i=0; i < this->modelFile->testParamRecords.size(); i++ )
    {
      // create small row array for legalValues
      int strValues_size =
           this->modelFile->testParamRecords[i]->paramValues.size();
      int *values = new int[strValues_size];
      legalValuesEachSize[currRow] = strValues_size;
      
      for (int j = 0; j < strValues_size; ++j) 
      {
        values[j] = kk;
        parameterValues[kk] = this->modelFile->testParamRecords[i]->paramValues[j];
        parameterValuePositon[kk] = j;
        ++kk;
      }

      legalValues[currRow] = values;
      currRow++;
    } // while



    // determine the number of pairs for this input set
    for (int i = 0; i <= numberParameters - 2; ++i)
    {
      for (int j = i + 1; j <= numberParameters - 1; ++j)
      {
        numberPairs += (legalValuesEachSize[i] * legalValuesEachSize[j]);
      }
    }

    // rectangular array; does not change, used to generate unusedCounts array
    int **allPairsDisplay;
    // process the legalValues array to populate the allPairsDisplay & 
    // unusedPairs & unusedPairsSearch collections
    allPairsDisplay = new int*[numberPairs];
    for(int i=0; i<numberPairs; i++){
        allPairsDisplay[i] = new int[2];
    }
    
    // square array -- changes
    int **unusedPairsSearch = new int*[numberParameterValues];
    for(int i=0; i<numberParameterValues; i++){
        unusedPairsSearch[i] = new int[numberParameterValues]; 
    }

    int currPair = 0;
    for (int i = 0; i <= numberParameters - 2; ++i)
    {
      for (int j = i + 1; j <= numberParameters - 1; ++j)
      {
        int *firstRow  = legalValues[i];
        int *secondRow = legalValues[j];
        
        for (int x = 0; x < legalValuesEachSize[i]; ++x)
        {
          for (int y = 0; y < legalValuesEachSize[j]; ++y)
          {
            allPairsDisplay[currPair][0] = firstRow[x];  // pair first value
            allPairsDisplay[currPair][1] = secondRow[y]; // pair second value

            int *aPair = new int[2]; //// strength 2 only
            aPair[0] = firstRow[x];
            aPair[1] = secondRow[y];
            unusedPairs.push_back(aPair);

            unusedPairsSearch[firstRow[x]][secondRow[y]] = 1;

            ++currPair;
          } // y
        } // x
      } // j
    } // i


    // process legalValues to populate parameterPositions array
    
    // the indexes are parameter values, the cell values are positions within a testSet
    int *parameterPositions = new int [numberParameterValues] ;
    
    int k = 0;  // points into parameterPositions
    for (int i = 0; i < numberParameters; ++i)
    {
        int *curr = legalValues[i];
        for (int j = 0; j < legalValuesEachSize[i]; ++j)
        {
            parameterPositions[k++] = i;
        }
    }


    // process allPairsDisplay to determine unusedCounts array

    // inexes are parameter values, cell values are counts of how many times 
    // the parameter value apperas in the unusedPairs collection
    int *unusedCounts = new int [numberParameterValues];  
    
    for (int i = 0; i < numberPairs; ++i)
    {
      ++unusedCounts[allPairsDisplay[i][0]];
      ++unusedCounts[allPairsDisplay[i][1]];
    }

    //============================================================================
    
    while (unusedPairs.size() > 0) 
    // as long as ther are unused pairs to account for . . .
    {
      // holds candidate testSets
      int **candidateSets = new int*[poolSize];
      
      for (int candidate = 0; candidate < poolSize; ++candidate)
      {
        // make an empty candidate testSet
        int *testSet = new int[numberParameters];

        // pick "best" unusedPair -- the pair which has the sum of the most unused values
        int bestWeight = 0;
        int indexOfBestPair = 0;
        for (int i = 0; i < unusedPairs.size(); ++i)
        {
          int *curr = unusedPairs[i];
          int weight = unusedCounts[curr[0]] + unusedCounts[curr[1]];
          if (weight > bestWeight)
          {
            bestWeight = weight;
            indexOfBestPair = i;
          }
        }


        int *best;  // a copy is not strictly necesary here
        best = unusedPairs[indexOfBestPair];


        // position of first value from best unused pair
        int firstPos = parameterPositions[best[0]]; 
        int secondPos = parameterPositions[best[1]];


        // generate a random order to fill parameter positions
        vector<int> ordering ;
        ordering.resize(numberParameters);
        for (int i = 0; i < numberParameters; ++i) // initially all in order
          ordering[i] = i;

        // put firstPos at ordering[0] && secondPos at ordering[1]
        ordering[0] = firstPos;
        ordering[firstPos] = 0;

        int t = ordering[1];
        ordering[1] = secondPos;
        ordering[secondPos] = t;

        // shuffle ordering[2] thru ordering[last]
        //   Knuth shuffle. start at i=2 because want first two slots left alone
        for (int i = 2; i < ordering.size(); i++)  
        {
          //int j = r.Next(i, ordering.size());
          int j = i + rand() % (ordering.size() - i);
          int temp = ordering[j];
          ordering[j] = ordering[i];
          ordering[i] = temp;
        }


        // place two parameter values from best unused pair into candidate testSet
        testSet[firstPos]  = best[0];
        testSet[secondPos] = best[1];

        // for remaining parameter positions in candidate testSet, 
        // try each possible legal value, picking the one which captures the most unused pairs . . .
        for (int i = 2; i < numberParameters; ++i) // start at 2 because first two parameter have been placed
        {
          int currPos = ordering[i];
          int *possibleValues = legalValues[currPos];

          int currentCount = 0;  // count the unusedPairs grabbed by adding a possible value
          int highestCount = 0;  // highest of these counts
          int bestJ = 0;         // index of the possible value which yields the highestCount
          
          for (int j = 0; j < legalValuesEachSize[currPos]; ++j) 
          // examine pairs created by each possible value and each parameter value already there
          {
            currentCount = 0;
            for (int p = 0; p < i; ++p)  // parameters already placed
            {
              vector<int> candidatePair ;
              candidatePair.push_back(possibleValues[j]);
              candidatePair.push_back(testSet[ordering[p]]);


              if(unusedPairsSearch[candidatePair[0]][candidatePair[1]] == 1 ||
                 unusedPairsSearch[candidatePair[1]][candidatePair[0]] == 1)  
                // because of the random order of positions, must check both possibilities
              {
                ++currentCount;
              }
            } // p -- each previously placed paramter
            
            if (currentCount > highestCount)
            {
              highestCount = currentCount;
              bestJ = j;
            }

          } // j -- each possible value at currPos

          testSet[currPos] = possibleValues[bestJ]; // place the value which captured the most pairs
        } // i -- each testSet position 


        candidateSets[candidate] = testSet;  // add candidate testSet to candidateSets array
        
      } // for each candidate testSet


      // Iterate through candidateSets to determine the best candidate

      int indexOfBestCandidate =  rand() % poolSize;
      
      int mostPairsCaptured =
         NumberPairsCaptured(candidateSets[indexOfBestCandidate], 
                             numberParameters, unusedPairsSearch);

      int *bestTestSet;
      for (int i = 0; i < poolSize; ++i)
      {
        int pairsCaptured =
           NumberPairsCaptured(candidateSets[i], 
                               numberParameters, unusedPairsSearch);
        if (pairsCaptured > mostPairsCaptured)
        {
          mostPairsCaptured = pairsCaptured;
          indexOfBestCandidate = i;
        }
      }
      bestTestSet = candidateSets[indexOfBestCandidate];
      testSets.push_back(bestTestSet); // Add the best candidate to the main testSets List

      // now perform all updates

      for (int i = 0; i <= numberParameters - 2; ++i)
      {
        for (int j = i + 1; j <= numberParameters - 1; ++j)
        {
          int v1 = bestTestSet[i]; // value 1 of newly added pair
          int v2 = bestTestSet[j]; // value 2 of newly added pair

          --unusedCounts[v1];
          --unusedCounts[v2];

          unusedPairsSearch[v1][v2] = 0;

          for (int p = 0; p < unusedPairs.size(); ++p)
          {
            int *curr = unusedPairs[p];

            if (curr[0] == v1 && curr[1] == v2)
            {
              unusedPairs.erase(unusedPairs.begin() + p);
            }
          }
        } // j
      } // i

    } // primary while loop


    // set results to CA 
    for (int i = 0; i < testSets.size(); ++i)
    {
        //cout << i  <<  ": "  ;
        int *curr = testSets[i];
        vector<int> str_vec;
        str_vec.resize(0);
        for (int j = 0; j < numberParameters; ++j)
        {
            str_vec.push_back( parameterValuePositon[curr[j]] );
        }
        CA.push_back(str_vec);
    }
    
    
    this->testSize =  testSets.size();
}

//-----------------------------------------------------------------------------
// number of unused pairs captured by testSet ts 
int AlgorithmCA::NumberPairsCaptured(int  *ts, int ts_size, int **unusedPairsSearch)  
{
  int ans = 0;
  for(int i = 0; i <= ts_size - 2; ++i)
  {
      for(int j = i + 1; j <= ts_size - 1; ++j)
      {
          if(unusedPairsSearch[ts[i]][ts[j]] == 1){
              ++ans;
          }
      }
  }
  return ans;
} 
