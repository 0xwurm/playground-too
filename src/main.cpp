#include <iostream>
#include "TicTacToe/bot.h"
#include "ConnectFour/bot.h"
#include <chrono>
#include "testing.h"

int main() {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 100000; i++) TTT::Bot::search<true>(0, 0);
    auto end = std::chrono::steady_clock::now();
    // std::cout << res << std::endl << std::endl;

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "t: " << time / 100000 << " microseconds" << std::endl;
    return 0;
}
