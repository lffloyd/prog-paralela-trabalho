#!/bin/bash

mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"
make

# Run unit tests
./test/ProgParalelaTrabalho_test
