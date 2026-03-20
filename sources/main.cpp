#include "main.hpp"

int main(int argc, char* argv[]) {
    std::cout << "number of arguments:" << argc << std::endl;
    for (auto i = 0; i < argc; i++) {
        std::cout << i << ": " << argv[i] << std::endl;
    }

    return 0;
}
