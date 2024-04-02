#pragma once

#define FORCEINLINE __attribute__((always_inline))

typedef unsigned long long u64;
typedef unsigned int u32;
typedef signed char i8;

using Map = u64;
using Bit = u64;
enum Result: i8{
    loss = -1,
    draw = 0,
    win = 1,
    undecided = 2
};