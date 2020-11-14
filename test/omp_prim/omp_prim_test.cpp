#include "gtest/gtest.h"
#include "stdio.h"
#include "limits.h"
#include "../../src/omp_prim/omp_prim.c"
#include "../../src/utils/prim.c"
#include "../../src/utils/test_utils.h"
#include "../../src/utils/matrix_generator.h"

TEST(GetMinimumCostEdge, ObtainsEdgeWithMinimumCostFromArray)
{
    int n = 3;

    Edge *list = create_edge_node(-1, -1, 3);
    Edge *second = create_edge_node(-1, -1, 2);
    list = insert_node(second, list);
    Edge *third = create_edge_node(-1, -1, 1);
    list = insert_node(third, list);

    Edge *result = get_minimum_cost_edge(list, n);

    int expected = 1;
    EXPECT_EQ(result->cost, expected);
    free_edge_list(list);
}

TEST(OmpPrimTest, GetExpectedCostForMST)
{
    int nthreads = 4;
    int size = 5;
    int mtx[TEST_MTX_NROWS][TEST_MTX_NCOLS] = {
        {0, 2, 1, 6, 7},
        {2, 0, 3, 8, 5},
        {6, 3, 0, 2, 7},
        {6, 8, 5, 0, 9},
        {4, 5, 7, 9, 2}};

    int expected = 9;

    int **dynamic_mtx = create_dynamic_matrix_from_static(mtx);

    int cost = omp_prim_minimum_spanning_tree(
        dynamic_mtx,
        TEST_MTX_NROWS,
        TEST_MTX_NCOLS,
        nthreads,
        TEST_NTRIALS,
        create_simple_line(-1, 1.0)
    );

    EXPECT_EQ(cost, expected);
}
