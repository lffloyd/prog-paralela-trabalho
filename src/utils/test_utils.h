#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

//Numero de linhas de matrizes nos testes unitarios
#define TEST_MTX_NROWS 5
//Numero de colunas de matrizes nos testes unitarios
#define TEST_MTX_NCOLS 5
//Numero de repeticoes de cenarios nos testes unitarios
#define TEST_NTRIALS 1

int **create_dynamic_matrix_from_static(int static_mtx[TEST_MTX_NROWS][TEST_MTX_NCOLS]);

#endif
