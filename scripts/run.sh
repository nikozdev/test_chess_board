#!/bin/sh

cmake -B build
cmake --build build -j$(nproc) --target chess_board
./build/chess_board

