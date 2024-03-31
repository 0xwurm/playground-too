#pragma once

#define FORCEINLINE __attribute__((always_inline))

typedef unsigned long long Map;
using Bit = Map;
enum Result: signed char{
    loss = -1,
    draw = 0,
    win = 1,
    undecided = 2
};