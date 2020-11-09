#include "gtest/gtest.h"
#include "stdio.h"
#include "limits.h"
#include "../../src/omp_prim/omp_prim.c"
#include "../../src/utils/prim.c"
#include "../../src/utils/test_utils.h"


TEST(GetMinimumCostEdge, ObtainsEdgeWithMinimumCostFromArray) {
    int n = 3;
    Edge *edges = (Edge *)malloc(n * sizeof(Edge));

    Edge *ptr = edges;
    for (int i = n-1; i >= 0 && ptr != NULL; i--) {
        ptr->cost = i+1;
        ptr++;
    }

    Edge *result = get_minimum_cost_edge(edges, n);

    int expected = 1;
    EXPECT_EQ(result->cost, expected);
    free(edges);
}
