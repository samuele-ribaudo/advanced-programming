#include "Game.h"
#include <iostream>
#include <thread>
#include <chrono>

Game::Game() : stepsNumber(30), delayMs(300), useAltRules(false) {}

Game::Game(const Grid& initialGrid) : grid(initialGrid), stepsNumber(30), delayMs(100), useAltRules(false) {}

void Game::setGrid(const Grid& g) {
    grid = g;
}

Grid& Game::getGrid() {
    return grid;
}

const Grid& Game::getGrid() const {
    return grid;
}

void Game::setSteps(int steps) {
    stepsNumber = steps;
}

void Game::setDelay(int delayMs_) {
    delayMs = delayMs_;
}

void Game::setUseAlternativeRules(bool useAlt) {
    useAltRules = useAlt;
}

void Game::run(bool printEachStep) {
    for (int step = 0; step <= stepsNumber; ++step) {
        if (printEachStep) {
            // TODO: system("clear") or equivalent to clear console
            grid.print();
            std::cout << "Iteration: " << step << " / " << stepsNumber << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        
        if (step == stepsNumber) break;
        
        stepOnce();
    }
}

void Game::stepOnce() {
    if (useAltRules) {
        grid.stepAlternativeRules();
    } else {
        grid.stepClassicRules();
    }
}
