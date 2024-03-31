#include <iostream>
#include "TicTacToe/bot.h"
#include "ConnectFour/bot.h"
#include <chrono>
#include <emmintrin.h>
#include <immintrin.h>

__m128i test(const __m128i board){
    __m128i shift1 = _mm_srli_epi64(board, 1);
    __m128i shift2 = _mm_srli_epi64(board, 2);

    __m128i end = _mm_and_si128(shift1, shift2);

    return end;
}

int main() {
    /*
    auto start = std::chrono::steady_clock::now();
    std::cout << int(TTT::Bot::search(0, 0)) << std::endl;
    auto end = std::chrono::steady_clock::now();
    std::cout << "Nodes: " << CF::nodes << std::endl;

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "t: " << time << " microseconds" << std::endl;
     */

    __m128i a = {12, 12};
    __m128i r = test (a);
    std::cout << r[0] << std::endl;
    std::cout << r[1] << std::endl;
    return 0;
}
