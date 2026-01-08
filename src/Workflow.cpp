#include "Workflow.h"
#include <iostream>
#include <string>

Workflow::Workflow() {}

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

        game.run(true);
        saveFinalIfRequested();
        
        char continue_choice;
        std::cout << "Another simulation? (y/n): ";
        std::cin >> continue_choice;
        while (continue_choice != 'y' && continue_choice != 'Y' && continue_choice != 'n' && continue_choice != 'N') {
            std::cout << "Invalid option! Another simulation? (y/n): ";
            std::cin >> continue_choice;
        }

        if (continue_choice != 'y' && continue_choice != 'Y') break;
    }
}

void Workflow::showMenu() {
    system("clear");
    std::cout << "=====================" << std::endl;
    std::cout << "Conway's Game of Life" << std::endl;
    std::cout << "=====================" << std::endl;
    std::cout << "a) initialize random grid" << std::endl;
    std::cout << "b) load grid from file" << std::endl;
    std::cout << "Choose option (a/b): ";
}

bool Workflow::askLoadFromFile() {
    char option;
    std::cin >> option;

    while(option != 'a' && option != 'b'){
        std::cout << "Invalid option! Choose option (a/b): ";
        std::cin >> option;
    }
    return option == 'b';
}

std::string Workflow::askFilePath() {
    std::string filename;
    std::cout << "Enter filename (in input/): ";
    std::cin >> filename;
    return "input/" + filename;
}

void Workflow::askRandomGridParams(int& w, int& h, int& p) {
    std::cout << "Enter grid size X (ex. 80): ";
    std::cin >> w;
    std::cout << "Enter grid size Y (ex. 20): ";
    std::cin >> h;
    std::cout << "Enter live cell probability (0-100): ";
    std::cin >> p;
}

void Workflow::askStepsAndDelay(int& steps, int& delayMs) {
    std::cout << "Enter number of time steps (ex. 30): ";
    std::cin >> steps;
    std::cout << "Enter delay between steps (ms, ex. 100): ";
    std::cin >> delayMs;
}

bool Workflow::askSaveFinal() {
    char answer;
    do {
        std::cout << "Save final constellation? (Y/N): ";
        std::cin >> answer;
    } while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');
    return (answer == 'Y' || answer == 'y');
}

std::string Workflow::askOutputFilePath() {
    std::string filename;
    std::cout << "Enter filename (in output/): ";
    std::cin >> filename;
    return "output/" + filename;
}

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

void Workflow::initRandom() {
    int w, h, p;
    askRandomGridParams(w, h, p);
    Grid g;
    g.randomInit(w, h, p);
    game.setGrid(g);
}

void Workflow::saveFinalIfRequested() {
    if (askSaveFinal()) {
        std::string path = askOutputFilePath();
        game.getGrid().saveToFile(path);
    }
}
