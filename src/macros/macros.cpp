// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

// op class
#define safe >> // Go and wait (safe mod)
#define qq >> // for classes: unsafe mod (شیفت راست)

// function type
#define fn inline std::any  // inline function for normal use
#define endf return nullptr

// print
#define echo(x) std::cout << x << std::endl // printing to console

// lambda
#define lm []()
#define lma [&]()
