#include <gtest/gtest.h>
#include "../include/GameOfLife.h"

// =========================================================
// Test if initializeGridRandom creates a grid with correct size
// =========================================================
TEST(GameOfLifeTest, RandomGridSize) {
    int width = 10;
    int height = 5;
    auto grid = initializeGridRandom(width, height);

    ASSERT_EQ(grid.size(), width);
    ASSERT_EQ(grid[0].size(), height);
}

// =========================================================
// Main for running all tests
// =========================================================
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}