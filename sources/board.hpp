#pragma once

#include "rook.hpp"

#include <array>
#include <vector>
#include <random>
#include <iostream>

constexpr int BOARD_SIZE = 8;

class Board {
public:
    Board() :
        grid{},
        randevice(),
        randengine(randevice()),
        randist(0, BOARD_SIZE - 1)
    {
        for (auto& row : grid) {
            row.fill(nullptr);
        }
    }

    bool has_free_cell(int col, int row) const {
        return grid[row][col] == nullptr;
    }

    Rook* get_cell(int col, int row) const {
        return grid[row][col];
    }

    void set_cell(int col, int row, Rook* rook) {
        grid[row][col] = rook;
    }

    void place_rooks_random(std::vector<Rook>& rooks) {
        for (auto& rook : rooks) {
            int col, row;
            do {
                col = randist(randengine);
                row = randist(randengine);
            } while (!has_free_cell(col, row));
            rook.col = col;
            rook.row = row;
            place_rook(rook);
        }
    }

    void place_rook(Rook& rook) {
        grid[rook.row][rook.col] = &rook;
    }

    void print() const {
        std::cout << "  ";
        for (int col = 0; col < BOARD_SIZE; col++) {
            std::cout << " " << static_cast<char>('a' + col);
        }
        std::cout << "\n";
        for (int row = BOARD_SIZE - 1; row >= 0; row--) {
            std::cout << (row + 1) << " ";
            for (int col = 0; col < BOARD_SIZE; col++) {
                Rook* rook = grid[row][col];
                if (rook) {
                    std::cout << " " << rook->id;
                } else {
                    std::cout << " .";
                }
            }
            std::cout << "\n";
        }
    }

private:
    std::array<std::array<Rook*, BOARD_SIZE>, BOARD_SIZE> grid;

    std::random_device randevice;
    std::mt19937 randengine;
    std::uniform_int_distribution<int> randist;
};
