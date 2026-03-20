#pragma once

#include <array>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <latch>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

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
    Board();

    // checkers

    bool has_free_cell(Cell cell) const;
    bool has_free_path(Cell from, Cell dest) const;

    // getters

    Rook* get_cell_rook(Cell cell) const;
    Cell get_random_cell(const Rook& rook);
    int get_random_pause();

    // setters

    void set_cell_rook(Cell cell, Rook* rook);
    void place_rooks_random(std::vector<Rook>& rooks);
    void place_rook_at_cell(Rook& rook);
    void move_rook_to_cell(Rook& rook, Cell cell);

    // logic

    void run_rooks(std::vector<Rook>& rooks, int move_limit);
    void run_rook(Rook& rook, int move_limit, std::latch& start_latch);

    // printing

    void print() const;

    void log_move(const Rook& rook, Cell from);
    void log_blocked(const Rook& rook, Cell dest);
    void log_timeout(const Rook& rook, Cell dest);

    static char col_to_char(int col);
    static void print_cell(Cell cell);

private: // members

    std::array<std::array<Rook*, BOARD_SIZE>, BOARD_SIZE> grid;

    std::random_device randevice;
    std::mt19937 randengine;
    std::uniform_int_distribution<int> randist;

    std::mutex board_mutex;
    std::mutex print_mutex;
    std::condition_variable board_changed;
};
