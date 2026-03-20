#pragma once

#include <array>
#include <vector>
#include <random>
#include <iostream>
#include <cstdint>

constexpr int BOARD_SIZE = 8;

struct Cell {
    int col;
    int row;
};

struct Rook {
    int id;
    Cell cell;
    int move_count = 0;
};

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

    bool has_free_cell(Cell cell) const {
        return grid[cell.row][cell.col] == nullptr;
    }

    Rook* get_cell_rook(Cell cell) const {
        return grid[cell.row][cell.col];
    }

    void set_cell_rook(Cell cell, Rook* rook) {
        grid[cell.row][cell.col] = rook;
    }

    void place_rooks_random(std::vector<Rook>& rooks) {
        for (auto& rook : rooks) {
            Cell cell;
            do {
                cell.col = randist(randengine);
                cell.row = randist(randengine);
            } while (!has_free_cell(cell));
            rook.cell = cell;
            place_rook_at_cell(rook);
        }
    }

    void place_rook_at_cell(Rook& rook) {
        grid[rook.cell.row][rook.cell.col] = &rook;
    }

    bool has_free_path(Cell from, Cell dest) const {
        if (from.col == dest.col) {
            int step = (dest.row > from.row) ? 1 : -1;
            for (int r = from.row + step; r != dest.row; r += step) {
                Cell cell{.col = from.col, .row = r};
                if (!has_free_cell(cell)) return false;
            }
        } else {
            int step = (dest.col > from.col) ? 1 : -1;
            for (int c = from.col + step; c != dest.col; c += step) {
                Cell cell{.col = c, .row = from.row};
                if (!has_free_cell(cell)) return false;
            }
        }
        return has_free_cell(dest);
    }

    void move_rook_to_cell(Rook& rook, Cell cell) {
        grid[rook.cell.row][rook.cell.col] = nullptr;
        rook.cell = cell;
        rook.move_count++;
        grid[cell.row][cell.col] = &rook;
    }

    Cell get_random_cell(const Rook& rook) {
        bool hor = randist(randengine) % 2 == 0;
        int pos = randist(randengine);
        if (hor) {
            while (pos == rook.cell.col) pos = randist(randengine);
            return {pos, rook.cell.row};
        } else {
            while (pos == rook.cell.row) pos = randist(randengine);
            return {rook.cell.col, pos};
        }
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
