#pragma once

#include <string>
#include "Game.h"

class Workflow {
public:
    Workflow();
    void runMainLoop();

private:
    Game game;
    
    void showMenu();
    bool askLoadFromFile();
    std::string askFilePath();
    void askRandomGridParams(int& w, int& h, int& p);
    void askStepsAndDelay(int& steps, int& delayMs);
    bool askSaveFinal();
    std::string askOutputFilePath();

    void initFromFile();
    void initRandom();
    void saveFinalIfRequested();

    bool askSaveFrames(std::string& prefix);
};
