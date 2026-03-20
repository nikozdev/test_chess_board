#include "main.hpp"

constexpr int ROOK_COUNT = 5;
constexpr int MOVE_LIMIT = 10; // 50 for production

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

    board.run_rooks(rooks, MOVE_LIMIT);

    std::cout << "\nfinal board:\n";
    board.print();

    return 0;
}
