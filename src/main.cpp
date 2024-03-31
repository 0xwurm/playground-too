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
    auto start = std::chrono::steady_clock::now();
    __m128i board = {0, 0};
    std::cout << int(TTT::Bot::search(board)) << std::endl;
    auto end = std::chrono::steady_clock::now();
    std::cout << "Nodes: " << CF::nodes << std::endl;

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "t: " << time << " microseconds" << std::endl;
    return 0;
}
