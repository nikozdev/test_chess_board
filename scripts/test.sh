#/bin/sh

cmake -B build
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
