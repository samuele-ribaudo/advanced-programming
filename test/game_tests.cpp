#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include "Grid.h"
#include "Game.h"
#include "Cell.h"

// =========================================================
// Test Grid: verify randomInit creates grid with correct dimensions
// =========================================================
TEST(GridTest, RandomInitSize) {
    Grid grid;
    grid.randomInit(80, 20);
    EXPECT_EQ(grid.getWidth(), 80);
    EXPECT_EQ(grid.getHeight(), 20);
}

// =========================================================
// Test Grid: verify loadFromFile creates grid with correct dimensions
// =========================================================
TEST(GridTest, LoadFromFileSize) {
    Grid grid;
    grid.loadFromFile("../input/wikipedia_test.txt");
    EXPECT_EQ(grid.getWidth(), 80);
    EXPECT_EQ(grid.getHeight(), 20);
}

// =========================================================
// Test Grid: verify loadFromFile loads cell states correctly
// =========================================================
TEST(GridTest, LoadFromFileContent) {
    Grid grid;
    grid.loadFromFile("../input/wikipedia_test.txt");
    
    // Verify some known cells from wikipedia pattern
    EXPECT_TRUE(grid.at(13, 3).isAliveNow());
    EXPECT_TRUE(grid.at(21, 11).isAliveNow());
    EXPECT_TRUE(grid.at(68, 9).isAliveNow());
    EXPECT_TRUE(grid.at(36, 11).isAliveNow());
    EXPECT_TRUE(grid.at(49, 3).isAliveNow());
    EXPECT_TRUE(grid.at(36, 1).isAliveNow());
    EXPECT_TRUE(grid.at(36, 2).isAliveNow());
    EXPECT_FALSE(grid.at(0, 0).isAliveNow());
    EXPECT_FALSE(grid.at(35, 11).isAliveNow());
}

// =========================================================
// Test Grid: verify loadFromFile handles non-existent files
// =========================================================
TEST(GridTest, LoadFromFileNonExistent) {
    Grid grid;
    EXPECT_FALSE(grid.loadFromFile("../input/non_existent_file.txt"));
}

// =========================================================
// Test Grid: verify stepClassicRules on 1x1 grid
// =========================================================
TEST(GridTest, StepClassicRulesSingleCell) {
    Grid grid;
    grid.randomInit(1, 1);
    
    // alive cell -> dies (0 neighbors)
    grid.at(0, 0).setAlive(true);
    grid.stepClassicRules(false);
    EXPECT_FALSE(grid.at(0, 0).isAliveNow());
    
    // dead cell -> stays dead
    grid.at(0, 0).setAlive(false);
    grid.stepClassicRules(false);
    EXPECT_FALSE(grid.at(0, 0).isAliveNow());
}

// =========================================================
// Test Grid: verify stepClassicRules on edge cells
// =========================================================
TEST(GridTest, StepClassicRulesEdgeCells) {
    Grid grid;
    grid.randomInit(2, 2);
    
    // configure 3 alive cells around corner
    grid.at(0, 0).setAlive(true);   // corner
    grid.at(0, 1).setAlive(true);
    grid.at(1, 0).setAlive(true);
    
    grid.stepClassicRules(false);
    
    // all should survive (2-3 neighbors)
    EXPECT_TRUE(grid.at(0, 0).isAliveNow());
    EXPECT_TRUE(grid.at(0, 1).isAliveNow());
    EXPECT_TRUE(grid.at(1, 0).isAliveNow());
    EXPECT_TRUE(grid.at(1, 1).isAliveNow());  // born with 3 neighbors
}

// =========================================================
// Test Grid: verify stepClassicRules with blinker pattern
// =========================================================
TEST(GridTest, StepClassicRulesBlinker) {
    Grid grid;
    grid.loadFromFile("../input/next_state_test.txt");
    Grid expected;
    expected.loadFromFile("../input/next_state_test_expected.txt");
    
    grid.stepClassicRules(false);
    
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            EXPECT_EQ(grid.at(x, y).isAliveNow(), expected.at(x, y).isAliveNow())
                << "Mismatch at (" << x << "," << y << ")";
        }
    }
}

// =========================================================
// Test Grid: verify infinity grid resizing
// =========================================================
TEST(GridTest, InfinityGridResizing) {
    Grid grid;
    grid.randomInit(3, 3);
    
    // Set up a pattern that will grow beyond current boundaries
    grid.at(0, 0).setAlive(true);
    grid.at(2, 2).setAlive(true);
    
    grid.stepClassicRules(true);  // enable resizing
    
    // After one step, the grid should have resized to at least 5x5
    EXPECT_GE(grid.getWidth(), 5);
    EXPECT_GE(grid.getHeight(), 5);
    
    // Check that new cells are correctly initialized (dead)
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            if (x < 1 || x > 3 || y < 0 || y > 2) {
                EXPECT_FALSE(grid.at(x, y).isAliveNow())
                    << "New cell at (" << x << "," << y << ") should be dead.";
            }
        }
    }
}


// =========================================================
// Test Game: verify Game::run applies correct number of steps
// =========================================================
TEST(GameTest, RunSimulationBlinker) {
    Grid initial;
    initial.loadFromFile("../input/next_state_test.txt");
    Game game(initial);
    game.setSteps(2);  // 2 steps return to original pattern
    game.setDelay(0);
    
    game.run(false);  // don't print
    
    // verify back to original after 2 steps
    for (int y = 0; y < initial.getHeight(); ++y) {
        for (int x = 0; x < initial.getWidth(); ++x) {
            EXPECT_EQ(game.getGrid().at(x, y).isAliveNow(), 
                     initial.at(x, y).isAliveNow())
                << "Mismatch at (" << x << "," << y << ")";
        }
    }
}

// =========================================================
// Test Game: verify wikipedia pattern cyclicity
// =========================================================
TEST(GameTest, RunSimulationWikipedia) {
    Grid initial;
    initial.loadFromFile("../input/wikipedia_test.txt");
    Game game(initial);
    game.setSteps(30);  // number of steps to return to initial state
    game.setDelay(0);
    
    game.run(false);
    
    // verify cyclicity
    for (int y = 0; y < initial.getHeight(); ++y) {
        for (int x = 0; x < initial.getWidth(); ++x) {
            EXPECT_EQ(game.getGrid().at(x, y).isAliveNow(), 
                     initial.at(x, y).isAliveNow())
                << "Mismatch at (" << x << "," << y << ")";
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
