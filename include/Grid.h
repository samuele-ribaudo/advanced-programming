#pragma once

#include <vector>
#include <string>
#include "Cell.h"

class Grid {
public:
    // Constructors
    Grid();
    Grid(int width, int height);

    // Dimensions
    int getWidth() const;
    int getHeight() const;

    // Cell access
    Cell& at(int x, int y);
    const Cell& at(int x, int y) const;
    bool isInside(int x, int y) const;

    // File I/O
    bool loadFromFile(const std::string& path);
    bool saveToFile(const std::string& path) const;

    // Initialization
    void randomInit(int width, int height, int p = 20);

    // Simulation steps
    void stepClassicRules(bool resize = true);
    void stepAlternativeRules(bool resize = true);

    // Output
    void print() const;

private:
    int width;
    int height;
    std::vector<std::vector<Cell>> cells;

    int countAliveNeighbors(int x, int y) const;
    void resizeIfNeeded();
};
