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

#ifndef _BOOKSIM_HPP_
#define _BOOKSIM_HPP_

#include <string>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cassert>
#include <map>
#include <vector>
#include <list>
#include <iostream>
#include <set>
#ifdef _WIN32_
#pragma warning (disable: 4786)
#include <ostream>
#endif

namespace Booksim
{
    using namespace std;
    template<typename K, typename V>
    void printMap(const std::map<K, V>& m) {
        std::cout << "{ ";
        for (const auto& [key, value] : m) {
            std::cout << key << " : " << value << ", ";
        }
        std::cout << "\b\b }" << std::endl;
    }

    template<typename T>
    void printList(const T& list) {
        std::cout << "[ ";
        for (const auto& element : list) {
            std::cout << element << ", ";
        }
        if (!list.empty()) {
            std::cout << "\b\b";  // Remove the last comma and space
        }
        std::cout << " ]" << std::endl;
    }


    template<typename T>
    void printSet(const T& set) {
        std::cout << "{ ";
        bool first = true;
        for (const auto& element : set) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << element;  // Now uses the overloaded << operator
            first = false;
        }
        std::cout << " }" << std::endl;
    }


} // namespace Booksim

#endif
