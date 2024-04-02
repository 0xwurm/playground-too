#include <iostream>
#include "TicTacToe/bot.h"
#include "ConnectFour/bot.h"
#include <chrono>
#include "testing.h"

int main() {
    auto start = std::chrono::steady_clock::now();
    auto res = TTT::Bot::search<true>(0, 0);
    auto end = std::chrono::steady_clock::now();
    std::cout << int(res) << std::endl << std::endl;

    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "t: " << time << " microseconds" << std::endl;
    return 0;
}
