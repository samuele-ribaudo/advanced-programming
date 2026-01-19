#pragma once

#include "Grid.h"
#include <chrono>

class Game {
public:
    // Constructors
    Game();
    explicit Game(const Grid& initialGrid);

    // Configuration
    void setGrid(const Grid& g);
    Grid& getGrid();
    const Grid& getGrid() const;
    void setSteps(int steps);
    void setDelay(int delayMs);
    void setUseAlternativeRules(bool useAlt);

    // Simulation
    void run(bool printEachStep, bool saveFrames = false, std::string prefix = "");

private:
    Grid grid;
    int stepsNumber;
    int delayMs;
    bool useAltRules;

    void stepOnce();
};
