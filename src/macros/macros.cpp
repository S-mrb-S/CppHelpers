// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

// function type
#define fn inline std::any  // inline function for normal use
#define endf return nullptr

// print
#define echo(x) std::cout << x << std::endl // printing to console

// lambda
#define lm []()
#define lma [&]()
