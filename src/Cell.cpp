#include "Cell.h"

Cell::Cell() : isAlive(false), nextState(false) {}

Cell::Cell(bool alive) : isAlive(alive), nextState(false) {}

bool Cell::isAliveNow() const {
    return isAlive;
}

void Cell::setAlive(bool alive) {
    isAlive = alive;
}

bool Cell::getNextState() const {
    return nextState;
}

void Cell::setNextState(bool state) {
    nextState = state;
}

void Cell::resetNextState() {
    nextState = false;
}

void Cell::applyNextState() {
    isAlive = nextState;
    nextState = false;
}
