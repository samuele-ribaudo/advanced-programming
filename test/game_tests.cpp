#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "../include/GameOfLife.h"

// =========================================================
// Test if initializeGridRandom creates a grid with correct size
// =========================================================
TEST(InitializeGrid, RandomGridSize){
    int width = 80;
    int height = 20;
    auto grid = initializeGridRandom(width, height);

    ASSERT_EQ(grid.size(), width);
    ASSERT_EQ(grid[0].size(), height);
}

// =========================================================
// Test if initializeGridFromFile creates a grid with correct size
// =========================================================
TEST(InitializeGrid, LoadFromFileSize){
    int width, height;
    auto grid = initializeGridFromFile(width, height, "../input/wikipedia_test.txt");

    ASSERT_EQ(width, 80);
    ASSERT_EQ(height, 20);
}

// =========================================================
// Test if initializeGridFromFile correctly loads cell states
// =========================================================
TEST(InitializeGrid, LoadFromFileContent){
    int width, height;
    auto grid = initializeGridFromFile(width, height, "../input/wikipedia_test.txt");

    // Check some known alive and dead cells from the wikipedia_test.txt pattern
    EXPECT_TRUE(grid[13][3].isAlive);
    EXPECT_TRUE(grid[21][11].isAlive);
    EXPECT_TRUE(grid[68][9].isAlive);
    EXPECT_TRUE(grid[36][11].isAlive);
    EXPECT_TRUE(grid[49][3].isAlive);
    EXPECT_TRUE(grid[36][1].isAlive);
    EXPECT_TRUE(grid[36][2].isAlive);
    EXPECT_FALSE(grid[0][0].isAlive);
    EXPECT_FALSE(grid[35][11].isAlive);
}

// =========================================================
// Test if initializeGridFromFile returns empty grid for non-existent file
// =========================================================
TEST(InitializeGrid, LoadFromFileNonExistent){
    int width, height;
    auto grid = initializeGridFromFile(width, height, "../input/non_existent_file.txt");

    ASSERT_TRUE(grid.empty());
}


// =========================================================
// Test calculateNextState on a single cell grid
// =========================================================
TEST(CalculateNextState, SingleCell) {
    std::vector<std::vector<Cell>> grid(1, std::vector<Cell>(1));
    
    // alive
    grid[0][0].isAlive = true;
    calculateNextState(grid);
    EXPECT_FALSE(grid[0][0].isAlive);

    // dead
    grid[0][0].isAlive = false;
    calculateNextState(grid);
    EXPECT_FALSE(grid[0][0].isAlive);
}


// =========================================================
// Test calculateNextState on edge cells
// =========================================================
TEST(CalculateNextState, EdgeCells) {
    std::vector<std::vector<Cell>> grid(2, std::vector<Cell>(2));
    grid[0][0].isAlive = true; // corner cell
    grid[0][1].isAlive = true;
    grid[1][0].isAlive = true;

    calculateNextState(grid);
    
    EXPECT_TRUE(grid[0][0].isAlive); 
    EXPECT_TRUE(grid[0][1].isAlive);
    EXPECT_TRUE(grid[1][0].isAlive);
    EXPECT_TRUE(grid[1][1].isAlive);
}


// =========================================================
// Test calculateNextState on all alive grid
// =========================================================
TEST(CalculateNextState, AllAlive) {
    std::vector<std::vector<Cell>> grid(3, std::vector<Cell>(3, {true, false}));
    calculateNextState(grid);
    
    // corner cells with 3 neighbors survive, center dies
    EXPECT_FALSE(grid[1][1].isAlive);
}


// =========================================================
// Test calculateNextState on all dead grid
// =========================================================
TEST(CalculateNextState, AllDead) {
    std::vector<std::vector<Cell>> grid(4, std::vector<Cell>(4, {false, false}));
    calculateNextState(grid);
    
    for (auto &row : grid)
        for (auto &cell : row)
            EXPECT_FALSE(cell.isAlive);
}


// =========================================================
// Test if calculateNextState produces expected next state with blinker pattern
// =========================================================
TEST(CalculateNextState, BlinkerPattern){
    int width, height;
    auto grid = initializeGridFromFile(width, height, "../input/next_state_test.txt");
    auto expected_grid = initializeGridFromFile(width, height, "../input/next_state_test_expected.txt");
    calculateNextState(grid);

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            EXPECT_EQ(grid[x][y].isAlive, expected_grid[x][y].isAlive) << "Mismatch at (" << x << "," << y << ")";
        }
    }
}


// =========================================================
// Test if runSimulation produces expected final state after random steps
// =========================================================
TEST(RunSimulation, BlinkerPattern){
    int width, height;
    auto grid = initializeGridFromFile(width, height, "../input/next_state_test.txt");
    auto expected_grid = initializeGridFromFile(width, height, "../input/next_state_test.txt");
    int steps = (random() % 50) * 2; // even number of steps to return to original state
    runSimulation(grid, steps, 0, false);

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            EXPECT_EQ(grid[x][y].isAlive, expected_grid[x][y].isAlive) << "Mismatch at (" << x << "," << y << ")";
        }
    }
}


// =========================================================
// Test if runSimulation produces expected final state after given steps
// =========================================================
TEST(RunSimulation, WikipediaPattern){
    int width, height;
    auto grid = initializeGridFromFile(width, height, "../input/wikipedia_test.txt");
    auto expected_grid = initializeGridFromFile(width, height, "../input/wikipedia_test.txt");
    int steps = 30; // mcm of 2, 3 and 15 to return to original state
    runSimulation(grid, steps, 0, false);

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            EXPECT_EQ(grid[x][y].isAlive, expected_grid[x][y].isAlive) << "Mismatch at (" << x << "," << y << ")";
        }
    }
}


// =========================================================
// Test if saveFinalGrid creates a file
// =========================================================
TEST(SaveFinalGrid, FileCreation){
    int width = 80, height = 20;
    auto grid = initializeGridRandom(width, height);

    std::string savedFile = saveFinalGrid(grid, "test_output");
    std::ifstream fileCheck(savedFile);
    ASSERT_TRUE(fileCheck.is_open());
    fileCheck.close();
}


// =========================================================
// Test if saveFinalGrid correctly add .txt extension if missing
// =========================================================
TEST(SaveFinalGrid, FileExtension){
    int width = 80, height = 20;
    auto grid = initializeGridRandom(width, height);

    std::string savedFile = saveFinalGrid(grid, "test_output_no_ext");
    ASSERT_EQ(savedFile.substr(savedFile.size() - 4), ".txt");
}


// =========================================================
// Test if file content created by saveFinalGrid matches the grid
// =========================================================
TEST(SaveFinalGrid, FileContent){
    int width = 20, height = 10;
    auto grid = initializeGridRandom(width, height);

    std::string savedFile = saveFinalGrid(grid, "test_output_content");
    std::ifstream fileCheck(savedFile);

    int fileWidth, fileHeight;
    fileCheck >> fileWidth >> fileHeight;
    ASSERT_EQ(fileWidth, width);
    ASSERT_EQ(fileHeight, height);

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            char c;
            do{
                c = fileCheck.get();
            }while(std::isspace(static_cast<unsigned char>(c)) && fileCheck.good());

            bool isAliveInFile = (c == '1');
            EXPECT_EQ(grid[x][y].isAlive, isAliveInFile) << "Mismatch at (" << x << "," << y << ")";
        }
    }
    fileCheck.close();
}


// =========================================================
// Main for running all tests
// =========================================================
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    srand(time(NULL));
    return RUN_ALL_TESTS();
}