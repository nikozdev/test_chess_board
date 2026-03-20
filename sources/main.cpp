#include "main.hpp"

#include <cstdlib>
#include <chrono>

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
            std::cerr
                << "error: rook count must be "
                << ROOK_COUNT_MIN << "-" << ROOK_COUNT_MAX
                << ", got " << rook_count << "\n";
            return 1;
        }
    }

    std::vector<Rook> rooks(rook_count);
    for (int i = 0; i < rook_count; i++) {
        rooks[i].id = i + 1;
    }

    Board board;
    board.place_rooks_random(rooks);

    std::cout << "=== Chess Board ===" << "\n";
    std::cout << rook_count << " rooks placed:" << "\n\n";
    board.print();
    std::cout << "\n";

    auto since = std::chrono::steady_clock::now();
    board.run_rooks(rooks, MOVE_LIMIT);
    auto until = std::chrono::steady_clock::now();
    auto timing = std::chrono::duration_cast<std::chrono::milliseconds>(
        until - since
    );

    std::cout << "\nfinal board:\n";
    board.print();

    std::cout << "\n=== Summary ===" << "\n";
    std::cout << "timing: " << timing.count() << "ms" << "\n";
    for (const auto& rook : rooks) {
        std::cout << "rook " << rook.id << ": " << rook.move_count << " moves\n";
    }

    return 0;
}
