// $Id$

/*
 Copyright (c) 2007-2012, Trustees of The Leland Stanford Junior University
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this 
 list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice, this
 list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_
#include <string>
#include <vector>
#include <iostream>
#include "NetworkInterface.h"

namespace Booksim
{

    /*all declared in main.cpp*/
    long long GetSimTime();

    class Stats;
    Stats * GetStats(const std::string & name);

    extern bool gPrintActivity;

    extern int gK;
    extern int gN;
    extern int gC;



    extern int gHK; // Global K (number of chiplets per dimension)
    extern int gHN; // Global N (number of dimensions in global mesh)
    extern int lHK; // Local K (number of cores per dimension in chiplet)
    extern int lHN; // Local N (number of dimensions in local mesh)
    extern int gNumChiplets; // Total number of chiplets
    extern int gNumCoresPerChiplet; // Number of cores per chiplet
    extern int gTotalNodes; // Total number of nodes (cores)


    extern vector<int> gKvector;
    extern vector<int> gCvector;

    extern int gNodes;

    extern bool gTrace;

    extern std::ostream * gWatchOut;

    extern NetworkInterface * ni;
} // namespace Booksim

#endif
