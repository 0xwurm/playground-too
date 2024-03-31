cmake_minimum_required(VERSION 3.27)
project(playground)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-march=native)

add_executable(playground src/main.cpp
        src/TicTacToe/bot.h
        src/ConnectFour/bot.h
        src/types.h)
