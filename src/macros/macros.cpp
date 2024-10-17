
// From topc.hpp, To let.cpp
#pragma once

#include "../core/cc.hpp"

// class
#define ctyp(Typename) template <typename Typename>

// main
#define run int main()   // for int main()
#define run_end return 0 // end main (no error)
#define run_err return 1 // force quit, for end main (return error)

// function type
#define fn any            // fn: for normal use and normal function, any type!
#define fnto any_cast     // fnto: set type for fn
#define fun inline fn     // fun: inline function for normal use
#define func constexpr fn // func: const function, fast and compiled time

// print
#define echo(x) cout << x << endl // for printing to console

// error
#define panic(msg) throw runtime_error(msg) // for make throwing exceptions
#define assert(condition) \
    if (!(condition))     \
    throw logic_error("Assertion failed") // for assertions

// lambda
#define lama(func_body) [&](auto &&...args) { func_body }
#define lm []()

// other integer types
#define sint signed int   // for signed int
#define uint unsigned int // for unsigned int

#define schar signed char   // for signed char
#define uchar unsigned char // for unsigned char
#define wch wchar_t         // for wchar_t

#define sll long long          // for signed long long
#define ull unsigned long long // for unsigned long long

// Fixed-width integer types
#define i8 int8_t     // for 8-bit signed integer
#define ui8 uint8_t   // for 8-bit unsigned integer
#define i16 int16_t   // for 16-bit signed integer
#define ui16 uint16_t // for 16-bit unsigned integer
#define i32 int32_t   // for 32-bit signed integer
#define ui32 uint32_t // for 32-bit unsigned integer
#define i64 int64_t   // for 64-bit signed integer
#define ui64 uint64_t // for 64-bit unsigned integer

// Minimum-width integer types
#define li8 int_least8_t     // for least 8-bit signed integer
#define lui8 uint_least8_t   // for least 8-bit unsigned integer
#define li16 int_least16_t   // for least 16-bit signed integer
#define lui16 uint_least16_t // for least 16-bit unsigned integer
#define li32 int_least32_t   // for least 32-bit signed integer
#define lui32 uint_least32_t // for least 32-bit unsigned integer
#define li64 int_least64_t   // for least 64-bit signed integer
#define lui64 uint_least64_t // for least 64-bit unsigned integer

// Fastest integer types
#define fi8 int_fast8_t     // for fast 8-bit signed integer
#define fui8 uint_fast8_t   // for fast 8-bit unsigned integer
#define fi16 int_fast16_t   // for fast 16-bit signed integer
#define fui16 uint_fast16_t // for fast 16-bit unsigned integer
#define fi32 int_fast32_t   // for fast 32-bit signed integer
#define fui32 uint_fast32_t // for fast 32-bit unsigned integer
#define fi64 int_fast64_t   // for fast 64-bit signed integer
#define fui64 uint_fast64_t // for fast 64-bit unsigned integer

// Other special types
#define c16 char16_t // for 16-bit character
#define c32 char32_t // for 32-bit character

// Null pointer type
#define npt nullptr_t // for nullptr_t
