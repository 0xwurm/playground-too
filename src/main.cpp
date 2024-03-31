#include <iostream>
#include "TicTacToe/bot.h"
#include <chrono>

int main() {
    auto start = std::chrono::steady_clock::now();
    std::cout << int(TTT::Bot::search(0, 0)) << std::endl;
    auto end = std::chrono::steady_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "t: " << time << " microseconds" << std::endl;
    return 0;
}
