#include "main.hpp"

constexpr int ROOK_COUNT = 5;
constexpr int MOVE_LIMIT = 10; // 50 for production

static char col_to_char(int col) { return 'a' + col; }

static void print_cell(Cell cell) {
    std::cout << col_to_char(cell.col) << (cell.row + 1);
}

int main() {
    std::vector<Rook> rooks(ROOK_COUNT);
    for (int i = 0; i < ROOK_COUNT; i++) {
        rooks[i].id = i + 1;
    }

    Board board;
    board.place_rooks_random(rooks);

    std::cout << "=== Chess Board ===" << "\n";
    std::cout << ROOK_COUNT << " rooks placed:" << "\n\n";
    board.print();
    std::cout << "\n";

    bool all_done = false;
    while (!all_done) {
        all_done = true;
        for (auto& rook : rooks) {
            if (rook.move_count >= MOVE_LIMIT) continue;
            all_done = false;

            auto dest = board.get_random_cell(rook);
            if (!board.has_free_path(rook.cell, dest)) {
                std::cout << "rook " << rook.id << " blocked: ";
                print_cell(rook.cell);
                std::cout << " -> ";
                print_cell(dest);
                std::cout << "\n";
                continue;
            }

            auto from = rook.cell;
            board.move_rook_to_cell(rook, dest);
            std::cout
                << "rook " << rook.id
                << " move " << rook.move_count << ": ";
            print_cell(from);
            std::cout << " -> ";
            print_cell(rook.cell);
            std::cout << "\n";
        }
    }

    std::cout << "\nfinal board:\n";
    board.print();

    return 0;
}
