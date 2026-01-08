#pragma once

class Cell {
public:
    // Constructors
    Cell();
    explicit Cell(bool alive);

    // Current state
    bool isAliveNow() const;
    void setAlive(bool alive);

    // Next state buffer
    bool getNextState() const;
    void setNextState(bool state);
    void resetNextState();
    void applyNextState();
    
private:
    bool isAlive;
    bool nextState;
};
