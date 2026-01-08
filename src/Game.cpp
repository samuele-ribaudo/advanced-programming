#include "Game.h"
#include <iostream>
#include <thread>
#include <chrono>

// Default constructor initializes stepsNumber, delay, and rule mode
Game::Game() : stepsNumber(30), delayMs(100), useAltRules(false) {}

// Constructor initializes the grid and default parameters
// Parameters: initialGrid - the initial state of the grid
Game::Game(const Grid& initialGrid) : grid(initialGrid), stepsNumber(30), delayMs(100), useAltRules(false) {}

// Sets the game grid to g
// Parameters: g - the new grid to set
void Game::setGrid(const Grid& g) { grid = g; }

// Returns a reference to the game grid
// Returns: reference to the grid
Grid& Game::getGrid() { return grid; }

// Returns a const reference to the game grid
// Returns: const reference to the grid
const Grid& Game::getGrid() const { return grid; }

// Sets the number of steps to run
// Parameters: steps - number of steps
void Game::setSteps(int steps) { stepsNumber = steps; }

// Sets the delay in milliseconds between steps
// Parameters: delayMs_ - delay in milliseconds
void Game::setDelay(int delayMs_) { delayMs = delayMs_; }

// Sets whether to use alternative rules
// Parameters: useAlt - true to use alternative rules, false for classic
void Game::setUseAlternativeRules(bool useAlt) { useAltRules = useAlt; }

// Runs the game for the specified number of steps
// Parameters: printEachStep - if true, prints the grid at each step
void Game::run(bool printEachStep) {
    for (int step = 0; step <= stepsNumber; step++) {
        if (printEachStep) {
            system("clear");
            grid.print();
            std::cout << "Iteration: " << step << " / " << stepsNumber << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        if (step == stepsNumber) break;
        stepOnce();
    }
}

// Advances the game by one step using the selected rules
void Game::stepOnce() {
    if (useAltRules) {
        grid.stepAlternativeRules();
    } else {
        grid.stepClassicRules();
    }
}
