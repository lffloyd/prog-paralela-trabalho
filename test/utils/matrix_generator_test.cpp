#include "gtest/gtest.h"
#include "stdio.h"
#include "limits.h"
#include "../../src/utils/matrix_generator.c"


TEST(GenerateMatrixTest, GenerateGraphMatrixWithRandomWeigths) {
    int n = 5;
    int **mtx = generate_matrix(n);

    for (int i = 0; i < n; i++)
        EXPECT_EQ(INT_MAX, mtx[i][i]);
    
    free_matrix(mtx, n);
}

TEST(FisherYatesShuffle, ShuffleArrayOfNumbers) {
    int n = 6;
    int numbers[n] = {1, 2, 3, 4, 5, 6};

    int *original_numbers = (int *)malloc(n * sizeof(int));
    memcpy(original_numbers, numbers, n * sizeof(int));

    int *result = fisher_yates_shuffle(numbers, 6);

    int count_equals = 0;
    for (int i = 0; i < n; i++)
      if (result[i] == original_numbers[i])
        count_equals++;
    
    free(original_numbers);
    EXPECT_NE(n, count_equals);
}
