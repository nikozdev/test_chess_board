#include "board.hpp"

#include <random>

Board::Board() : grid{} {
    for (auto& row : grid) {
        row.fill(nullptr);
    }
}

static std::mt19937& get_rng() {
    thread_local std::mt19937 engine(std::random_device{}());
    return engine;
}

// checkers

bool Board::has_free_cell(Cell cell) const {
    return grid[cell.row][cell.col] == nullptr;
}

bool Board::has_free_path(Cell from, Cell dest) const {
    if (from.col == dest.col) {
        int step = (dest.row > from.row) ? 1 : -1;
        for (int r = from.row + step; r != dest.row; r += step) {
            Cell cell{.col = from.col, .row = r};
            if (!has_free_cell(cell)) {
                return false;
            }
        }
    } else {
        int step = (dest.col > from.col) ? 1 : -1;
        for (int c = from.col + step; c != dest.col; c += step) {
            Cell cell{.col = c, .row = from.row};
            if (!has_free_cell(cell)) {
                return false;
            }
        }
    }
    return has_free_cell(dest);
}

// getters

Rook* Board::get_cell_rook(Cell cell) const {
    return grid[cell.row][cell.col];
}

Cell Board::get_random_cell(const Rook& rook) {
    std::uniform_int_distribution<int> dist(0, BOARD_SIZE - 1);
    auto& rng = get_rng();

    bool hor = dist(rng) % 2 == 0;
    int pos = dist(rng);

    if (hor) {
        while (pos == rook.cell.col) pos = dist(rng);
        return {pos, rook.cell.row};
    } else {
        while (pos == rook.cell.row) pos = dist(rng);
        return {rook.cell.col, pos};
    }
}

int Board::get_random_pause() {
    std::uniform_int_distribution<int> dist(200, 300);
    return dist(get_rng());
}

// setters

void Board::set_cell_rook(Cell cell, Rook* rook) {
    grid[cell.row][cell.col] = rook;
}

void Board::place_rooks_random(std::vector<Rook>& rooks) {
    std::uniform_int_distribution<int> dist(0, BOARD_SIZE - 1);
    auto& rng = get_rng();

    for (auto& rook : rooks) {
        Cell cell;
        do {
            cell.col = dist(rng);
            cell.row = dist(rng);
        } while (!has_free_cell(cell));
        rook.cell = cell;
        place_rook_at_cell(rook);
    }
}

void Board::place_rook_at_cell(Rook& rook) {
    grid[rook.cell.row][rook.cell.col] = &rook;
}

void Board::move_rook_to_cell(Rook& rook, Cell dest) {
    Cell from = rook.cell;
    grid[from.row][from.col] = nullptr;
    rook.cell = dest;
    rook.move_count++;
    grid[dest.row][dest.col] = &rook;
    row_cvs[from.row].notify_all();
    col_cvs[from.col].notify_all();
}

// logic

void Board::run_rooks(std::vector<Rook>& rooks, int move_limit) {
    std::latch start_latch(rooks.size());
    std::vector<std::thread> threads;
    for (auto& rook : rooks) {
        threads.emplace_back(&Board::run_rook, this, std::ref(rook), move_limit, std::ref(start_latch));
    }
    for (auto& t : threads) {
        t.join();
    }
}

void Board::run_rook(Rook& rook, int move_limit, std::latch& start_latch) {
    start_latch.arrive_and_wait();

    while (rook.move_count < move_limit) {
        const auto dest = get_random_cell(rook);

        std::unique_lock lock(board_mutex);
        const auto from = rook.cell;

        if (!has_free_path(from, dest)) {
            log_blocked(rook, dest);
            bool horizontal = (dest.row == from.row);
            auto& cv = horizontal ? row_cvs[rook.cell.row] : col_cvs[rook.cell.col];
            bool freed = cv.wait_for(lock, std::chrono::seconds(5), [&] { return has_free_path(rook.cell, dest); });
            if (!freed) {
                log_timeout(rook, dest);
                continue;
            }
        }

        move_rook_to_cell(rook, dest);
        lock.unlock();

        log_move(rook, from);

        int pause = get_random_pause();
        std::this_thread::sleep_for(std::chrono::milliseconds(pause));
    }
}

// printing

void Board::print() const {
    std::cout << "  ";
    for (int col = 0; col < BOARD_SIZE; col++) {
        std::cout << " " << static_cast<char>('a' + col);
    }
    std::cout << std::endl;
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
        std::cout << std::endl;
    }
}

void Board::log_move(const Rook& rook, Cell from) {
    std::lock_guard lock(print_mutex);
    std::cout << "rook " << rook.id << " move " << rook.move_count << ": ";
    print_cell(from);
    std::cout << " -> ";
    print_cell(rook.cell);
    std::cout << std::endl;
}

void Board::log_blocked(const Rook& rook, Cell dest) {
    std::lock_guard lock(print_mutex);
    std::cout << "rook " << rook.id << " blocked: ";
    print_cell(rook.cell);
    std::cout << " -> ";
    print_cell(dest);
    std::cout << " (waiting...)" << std::endl;
}

void Board::log_timeout(const Rook& rook, Cell dest) {
    std::lock_guard lock(print_mutex);
    std::cout << "rook " << rook.id << " timeout: ";
    print_cell(rook.cell);
    std::cout << " -> ";
    print_cell(dest);
    std::cout << " (re-picking)" << std::endl;
    print();
}

char Board::col_to_char(int col) {
    return 'a' + col;
}

void Board::print_cell(Cell cell) {
    std::cout << col_to_char(cell.col) << (cell.row + 1);
}
