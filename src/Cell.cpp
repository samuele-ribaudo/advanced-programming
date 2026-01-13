#include "Cell.h"

// Default constructor initializes the cell as dead with nextState false
Cell::Cell() : isAlive(false), nextState(false) {}

// Constructor initializes the cell with the given alive state and nextState false
// Parameters: alive - initial alive state of the cell
Cell::Cell(bool alive) : isAlive(alive), nextState(false) {}

// Returns whether the cell is currently alive
// Returns: true if the cell is alive, false otherwise
bool Cell::isAliveNow() const {
    return isAlive;
}

// Sets the current alive state of the cell
// Parameters: alive - the new alive state to set
void Cell::setAlive(bool alive) {
    isAlive = alive;
}

// Returns the next state of the cell (to be applied in the next update)
// Returns: true if the cell will be alive in the next state, false otherwise
bool Cell::getNextState() const {
    return nextState;
}

// Sets the next state of the cell
// Parameters: state - the next state to set for the cell
void Cell::setNextState(bool state) {
    nextState = state;
}

// Resets the next state to false (dead)
void Cell::resetNextState() {
    nextState = false;
}

// Applies the next state to the current state and resets nextState to false
void Cell::applyNextState() {
    isAlive = nextState;
    nextState = false;
}
