// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

#include <ctime>
#include <algorithm>

#include "Random.h"

Random::Random()
{
    srand( static_cast<unsigned int>( time(0) ) );
}


unsigned int Random::operator()(unsigned int max)
{
    double tmp = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
    return static_cast<unsigned int>( tmp * max );
}

//-----------------------------------------------------------------------------
