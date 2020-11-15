#!/bin/bash

./build/src/ProgParalelaTrabalho_run SEQ 100 200 400 800 1000 1200 1400 1600 1800 2000 2500 3000 3500 4000
./build/src/ProgParalelaTrabalho_run OMP 100 200 400 800 1000 1200 1400 1600 1800 2000 2500 3000 3500 4000 2
./build/src/ProgParalelaTrabalho_run OMP 100 200 400 800 1000 1200 1400 1600 1800 2000 2500 3000 3500 4000 4
./build/src/ProgParalelaTrabalho_run OMP 100 200 400 800 1000 1200 1400 1600 1800 2000 2500 3000 3500 4000 8
python3 src/graphs/graphs.py SEQ_Prim.csv OMP_Prim_2.csv OMP_Prim_4.csv OMP_Prim_8.csv
