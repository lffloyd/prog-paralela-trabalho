#include "gtest/gtest.h"
#include "../src/utils/test_utils.c"
#include "../src/stats/stats.c"
#include "../src/utils/matrix_generator.c"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
