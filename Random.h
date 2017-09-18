// Copyright 2011,2012 hyam <hyamhyam@gmail.com>
// and it is under GPL
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
#ifndef RANDOM_H
#define RANDOM_H

class Random
{
public:
    Random();

     unsigned int operator()( unsigned int );
};

#endif
