#include "main.hpp"

constexpr int ROOK_COUNT = 5;
constexpr int MOVE_LIMIT = 50;

static char col_to_char(int col) { return 'a' + col; }

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

    Rook& rook = rooks[0];
    while (rook.move_count < MOVE_LIMIT) {
        auto dest = board.get_random_cell(rook);
        if (!board.has_free_path(rook.cell, dest)) {
            std::cout
                << "rook " << rook.id
                << " blocked: "
                << col_to_char(rook.cell.col) << (rook.cell.row + 1)
                << " -> "
                << col_to_char(dest.col) << (dest.row + 1)
                << "\n";
            continue;
        }
        auto from = rook.cell;
        board.move_rook_to_cell(rook, dest);
        std::cout
            << "rook " << rook.id
            << " move " << rook.move_count << ": "
            << col_to_char(from.col) << (from.row + 1)
            << " -> "
            << col_to_char(rook.cell.col) << (rook.cell.row + 1)
            << "\n";
    }

    std::cout << "\nfinal board:\n";
    board.print();

    return 0;
}
