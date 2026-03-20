#include "main.hpp"

constexpr int ROOK_COUNT = 5;

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

    return 0;
}
