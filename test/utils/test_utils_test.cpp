#include "gtest/gtest.h"
#include "stdio.h"
#include "limits.h"
#include "../../src/utils/test_utils.h"

TEST(CreateDynamicMatrixFromStatic, ChecksIfAllElementsAreEqualBetweenTheTwoMatrices)
{
    int mtx[TEST_MTX_NROWS][TEST_MTX_NCOLS] = {
        {0, 2, 0, 6, 0},
        {2, 0, 3, 8, 5},
        {0, 3, 0, 0, 7},
        {6, 8, 0, 0, 9},
        {0, 5, 7, 9, 0}};

    int **dynamic_mtx = create_dynamic_matrix_from_static(mtx);

    for (int i = 0; i < TEST_MTX_NROWS; i++)
        for (int j = 0; j < TEST_MTX_NCOLS; j++)
            EXPECT_EQ(mtx[i][j], dynamic_mtx[i][j]);

    for (int i = 0; i < TEST_MTX_NROWS; i++)
        free(dynamic_mtx[i]);
}
