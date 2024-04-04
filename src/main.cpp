#include <iostream>
#include "TicTacToe/bot.h"
#include "ConnectFour/bot.h"
#include <chrono>
#include "testing.h"

int main() {
    auto start1 = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000000; i++) {
        TTT::Bot::search<9>(0, 0);
        TTT::TT::clear();
    }
    auto end1 = std::chrono::steady_clock::now();

    auto time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1).count();
    std::cout << "t1: " << time1 / 1000000 << " nanoseconds" << std::endl;

    auto start2 = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000000; i++) TTT::TT::clear();
    auto end2 = std::chrono::steady_clock::now();

    auto time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count();
    std::cout << "t2: " << time2 / 1000000 << " nanoseconds" << std::endl;
    std::cout << "ts: " << (time1 - time2) / 1000000 << " nanoseconds" << std::endl;

    int res = TTT::Bot::search<9>(0, 0);
    std::cout << "Evaluation: " << res << std::endl;
    // std::cout << "Nodes: " << TTT::nodes << std::endl;

    return 0;
}
