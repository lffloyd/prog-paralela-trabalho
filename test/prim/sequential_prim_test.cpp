#include "gtest/gtest.h"
#include "stdio.h"
#include "limits.h"
#include "../../src/prim/sequential_prim.c"
#include "../../src/utils/test_utils.h"
#include "../../src/stats/stats.h"

TEST(SequentialPrimTest, GetExpectedCostForMST)
{
    int size = 5;
    int mtx[TEST_MTX_NROWS][TEST_MTX_NCOLS] = {
        {0, 2, 1, 6, 7},
        {2, 0, 3, 8, 5},
        {6, 3, 0, 2, 7},
        {6, 8, 5, 0, 9},
        {4, 5, 7, 9, 2}};

    int expected = 9;

    int **dynamic_mtx = create_dynamic_matrix_from_static(mtx);

    int cost = prim_minimum_spanning_tree(
        dynamic_mtx,
        TEST_MTX_NROWS,
        TEST_MTX_NCOLS,
        TEST_NTRIALS,
        create_simple_line(-1, 1.0)
    );
    EXPECT_EQ(cost, expected);
}
