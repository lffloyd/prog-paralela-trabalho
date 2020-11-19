#!/bin/bash

./build/src/ProgParalelaTrabalho_run SEQ 100 200 400
./build/src/ProgParalelaTrabalho_run OMP 100 200 400 2
mpiexec -np 2 ./build/src/ProgParalelaTrabalho_run MPI 100 200 400
