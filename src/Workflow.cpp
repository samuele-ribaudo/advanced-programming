#include "Workflow.h"
#include <iostream>
#include <string>

// Constructor for Workflow class
Workflow::Workflow() {}


// Runs the main loop of the workflow.
// Handles menu display, grid initialization, game execution, and user prompts for continuation.
void Workflow::runMainLoop() {
    while (true){
        game = Game(); // reset game
        showMenu();
        if(askLoadFromFile()) initFromFile();
        else initRandom();

        int steps, delayMs;
        askStepsAndDelay(steps, delayMs);
        game.setSteps(steps);
        game.setDelay(delayMs);

        std::string prefix;
        bool saveFrames = askSaveFrames(prefix);

        game.run(true, saveFrames, prefix);

        saveFinalIfRequested();
        
        char continue_choice;
        std::cout << "Another simulation? (Y/N): ";
        std::cin >> continue_choice;
        while (continue_choice != 'y' && continue_choice != 'Y' && continue_choice != 'n' && continue_choice != 'N') {
            std::cout << "Invalid option! Another simulation? (Y/N): ";
            std::cin >> continue_choice;
        }

        if (continue_choice != 'y' && continue_choice != 'Y') break;
    }
}


// Displays the main menu to the user
void Workflow::showMenu() {
    system("clear");
    std::cout << "=====================" << std::endl;
    std::cout << "Conway's Game of Life" << std::endl;
    std::cout << "=====================" << std::endl;
    std::cout << "a) initialize random grid" << std::endl;
    std::cout << "b) load grid from file" << std::endl;
    std::cout << "Choose option (a/b): ";
}


// Asks the user whether to load the grid from a file or initialize randomly.
// Return true if user chooses to load from file, false otherwise.
bool Workflow::askLoadFromFile() {
    char option;
    std::cin >> option;

    while(option != 'a' && option != 'b'){
        std::cout << "Invalid option! Choose option (a/b): ";
        std::cin >> option;
    }
    return option == 'b';
}


// Asks the user for the file path to load the grid from.
// @return the full path to the input file as a string.
std::string Workflow::askFilePath() {
    std::string filename;
    std::cout << "Enter filename (in input/): ";
    std::cin >> filename;
    if(filename.size() < 4 || filename.substr(filename.size() - 4) != ".pbm") filename += ".pbm";
    return "../input/" + filename;
}

// Helper function to ensure a positive integer input.
int checkPositive(int val) {
    while (val < 0) {
        std::cout << "Please enter a positive integer: ";
        std::cin >> val;
    }
    return val;
}

// Asks the user for parameters to initialize a random grid.
// parameters:
// - w Reference to grid width.
// - h Reference to grid height.
// - p Reference to live cell probability (0-100).

void Workflow::askRandomGridParams(int& w, int& h, int& p) {
    std::cout << "Enter grid size X (ex. 80): ";
    std::cin >> w;
    w = checkPositive(w);
    std::cout << "Enter grid size Y (ex. 20): ";
    std::cin >> h;
    h = checkPositive(h);
    std::cout << "Enter live cell probability (0-100): ";
    std::cin >> p;
    p = checkPositive(p);
}


// Asks the user for the number of time steps and delay between steps.
// parameters: 
// - steps Reference to number of time steps.
// - delayMs Reference to delay between steps in milliseconds.
void Workflow::askStepsAndDelay(int& steps, int& delayMs) {
    std::cout << "Enter number of time steps (ex. 30): ";
    std::cin >> steps;
    steps = checkPositive(steps);
    std::cout << "Enter delay between steps (ms, ex. 100): ";
    std::cin >> delayMs;
    delayMs = checkPositive(delayMs);
}


// Asks the user whether to save the final grid constellation.
// Return true if user wants to save, false otherwise.
bool Workflow::askSaveFinal() {
    char answer;
    do {
        std::cout << "Save final constellation? (Y/N): ";
        std::cin >> answer;
    } while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');
    return (answer == 'Y' || answer == 'y');
}

// Asks the user for the output file path to save the final grid.
// Return the full path to the output file as a string.
std::string Workflow::askOutputFilePath() {
    std::string filename;
    std::cout << "Enter filename (in output/): ";
    std::cin >> filename;
    if(filename.size() < 4 || filename.substr(filename.size() - 4) != ".pbm") filename += ".pbm";
    return "../output/" + filename;
}


// Initializes the grid from a file specified by the user.
// If loading fails, falls back to random grid initialization.
void Workflow::initFromFile() {
    std::string path = askFilePath();
    Grid g;
    if (!g.loadFromFile(path)) {
        std::cout << "Error loading file. Falling back to random grid.\n";
        int w = 80, h = 20, p = 20;
        askRandomGridParams(w, h, p);
        g.randomInit(w, h, p);
    }
    game.setGrid(g);
}


// Initializes the grid randomly based on user-specified parameters.
void Workflow::initRandom() {
    int w, h, p;
    askRandomGridParams(w, h, p);
    Grid g;
    g.randomInit(w, h, p);
    game.setGrid(g);
}


// Saves the final grid to a file if the user requests it.
void Workflow::saveFinalIfRequested() {
    if (askSaveFinal()) {
        std::string path = askOutputFilePath();
        game.getGrid().saveToFile(path);
    }
}

// Asks the user whether to save each frame of the simulation as PBM files.
bool Workflow::askSaveFrames(std::string& prefix) {
    while(true){
        std::cout << "Do you want to save each frame as PBM? (Y/N): ";
        char ans;
        std::cin >> ans;
        if (ans == 'y' || ans == 'Y') {
            std::cout << "Enter filename prefix: ";
            std::cin >> prefix;
            return true;
        } else if (ans == 'n' || ans == 'N') {
            return false;
        }
    }
}