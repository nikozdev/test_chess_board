#include "main.hpp"

#include <chrono>
#include <cstdlib>

constexpr int ROOK_COUNT_DEFAULT = 5;
constexpr int ROOK_COUNT_MIN = 4;
constexpr int ROOK_COUNT_MAX = 6;
#ifndef MOVE_LIMIT
constexpr int MOVE_LIMIT = 50;
#endif

int main(int argc, char* argv[]) {
    int rook_count = ROOK_COUNT_DEFAULT;
    if (argc > 1) {
        rook_count = std::atoi(argv[1]);
        if (rook_count < ROOK_COUNT_MIN || rook_count > ROOK_COUNT_MAX) {
            std::cerr << "error: rook count must be " << ROOK_COUNT_MIN << "-" << ROOK_COUNT_MAX << ", got " << rook_count << std::endl;
            return 1;
        }
    }

    std::vector<Rook> rooks(rook_count);
    for (int i = 0; i < rook_count; i++) {
        rooks[i].id = i + 1;
    }

    Board board;
    board.place_rooks_random(rooks);

    std::cout << "=== Chess Board ===" << std::endl;
    std::cout << rook_count << " rooks placed:" << std::endl << std::endl;
    board.print();
    std::cout << std::endl;

    auto since = std::chrono::steady_clock::now();
    board.run_rooks(rooks, MOVE_LIMIT);
    auto until = std::chrono::steady_clock::now();
    auto timing = std::chrono::duration_cast<std::chrono::milliseconds>(until - since);

    std::cout << std::endl << "final board:" << std::endl;
    board.print();

    std::cout << std::endl << "=== Summary ===" << std::endl;
    std::cout << "timing: " << timing.count() << "ms" << std::endl;
    for (const auto& rook : rooks) {
        std::cout << "rook " << rook.id << ": " << rook.move_count << " moves" << std::endl;
    }

    return 0;
}
