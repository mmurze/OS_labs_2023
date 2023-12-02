#!/bin/bash

mkdir build
cd build || exit
cmake ..
cmake --build .
cd ..
mv build/host* .
rm -rf build
