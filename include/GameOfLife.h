#pragma once

#include <string>
#include <vector>

// =========================================================
// Represents a single cell in the Game of Life.
// Fields:
//   - isAlive: current state
//   - nextState: temporary storage for next generation
// =========================================================
struct Cell {
    bool isAlive;
    bool nextState;
};


// =========================================================
// Prints the current state of the game board to the console.
// Parameters:
//   - grid: 2D vector of Cells indexed as grid[x][y]
// =========================================================
void printBoard(const std::vector<std::vector<Cell>> &grid);


// =========================================================
// Loads a grid from a text file.
// Parameters:
//   - gridSizeX: out parameter for width
//   - gridSizeY: out parameter for height
//   - filename: input filename (path)
// Returns: a 2D vector<Cell> with the loaded grid; returns an empty vector on error
// =========================================================
std::vector<std::vector<Cell>> initializeGridFromFile(int &gridSizeX, int &gridSizeY, const std::string &filename);


// =========================================================
// Initializes a random grid.
// Parameters:
//   - gridSizeX: width
//   - gridSizeY: height
//   - p: integer probability (0..100) that a cell starts alive. Default 20.
// Returns: a 2D vector<Cell> with random initial state
// =========================================================
std::vector<std::vector<Cell>> initializeGridRandom(int gridSizeX, int gridSizeY, int p = 20);


// =========================================================
// Computes the next generation and updates the grid in-place.
// Parameters:
//   - grid: the grid to update (modified in-place)
// =========================================================
void calculateNextState(std::vector<std::vector<Cell>> &grid);


// =========================================================
// Runs the simulation loop.
// Parameters:
//   - grid: the grid to simulate (modified in-place)
//   - stepsNumber: number of iterations to run
//   - delayMs: milliseconds to wait between iterations (default 100)
// =========================================================
void runSimulation(std::vector<std::vector<Cell>> &grid, int stepsNumber, int delayMs = 100);


// =========================================================
// Saves the grid to a text file. Adds ".txt" if missing.
// Parameters:
//   - grid: the grid to save
//   - filename: desired filename (may omit extension)
// Returns: the actual filename used (with extension), or empty string on error
// =========================================================
std::string saveFinalGrid(const std::vector<std::vector<Cell>> &grid, std::string filename = "final_constellation.txt");


// =========================================================
// Draws a horizontal line of length l using '-' characters.
// Parameters:
//   - l: length of the line
// =========================================================
void drawLine(int l);