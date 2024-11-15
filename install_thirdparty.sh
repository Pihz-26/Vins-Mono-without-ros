#!/bin/bash


cd /workspace/thirdparty/Pangolin || { echo "Failed to change directory (Pangolin)"; exit 1; }
mkdir -p build || { echo "Failed to create build directory (Pangolin)"; exit 1; }
cd build || { echo "Failed to change to build directory (Pangolin)"; exit 1; }
cmake .. || { echo "CMake configuration failed (Pangolin)"; exit 1; }
make -j4 || { echo "Make failed (Pangolin)"; exit 1; }
make install|| { echo "Make install failed (Pangolin)"; exit 1; }