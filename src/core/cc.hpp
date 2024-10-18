// Top container header for C++ modules
#pragma once

/*
    ***
        std libs first

        using method two

        src files (core) last
    ***
*/

// std libs
#include <iostream>
#include <string>
#include <math.h>
#include <memory>
#include <any>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <type_traits>
#include <functional>
#include <chrono>

using std::any;
using std::any_cast;
using std::string;

using std::map;
using std::set;
using std::vector;

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

using std::logic_error;
using std::runtime_error;


// src
#include "../macros/macros.cpp"
#include "../let/let.cpp"
#include "../threads/async.cpp"
#include "../threads/jthread.cpp"
#include "../var/var.cpp"


#ifdef NO_MAIN_FOUND
#error "Error: Your program does not contain a 'main' function!"
#endif