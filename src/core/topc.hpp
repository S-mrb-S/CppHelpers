// Top container header for C++ modules
#pragma once

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

// using std
using std::any;
using std::any_cast;
using std::string;

using std::vector;
using std::map;
using std::set;

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

using std::runtime_error;
using std::logic_error;

#ifdef NO_MAIN_FOUND
#error "Error: Your program does not contain a 'main' function!"
#endif