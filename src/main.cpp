// use SDL2 or raylib to render the game
#include <iostream>
#include <cstdlib>   // srand, rand
#include <ctime>     // time
#include <vector>
#include "GameOfLife.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;


int printIntroduction(){
    char option;

    system("clear");
    cout << "=====================" << endl;
    cout << "Conway's Game of Life" << endl;
    cout << "=====================" << endl;
    cout << "a) initialize random grid" << endl;
    cout << "b) load grid from file" << endl;
    cout << "Choose option (a/b): ";
    cin >> option;

    while(option != 'a' && option != 'b')
    {
        cout << "Invalid option! Choose option (a/b): ";
        cin >> option;
    }

    return option == 'a' ? 0 : 1;
}


int main(){

    srand(time(NULL));

    int stepsNumber = 30;
    int gridSizeX = 80;
    int gridSizeY = 20;
    std::vector<std::vector<Cell>> grid;

    if(printIntroduction()){
        // load from file
        string filename;
        cout << "Enter filename: ";
        cin >> filename;
        filename = "input/" + filename;
        grid = initializeGridFromFile(gridSizeX, gridSizeY, filename);
        if(grid.empty()){
            cout << "Failed to load grid from file. Exiting..." << endl;
            return 1;
        }
    }else{
        // random initialization
        cout << "Enter grid size X (ex. 80): ";
        cin >> gridSizeX;
        cout << "Enter grid size Y (ex. 20): ";
        cin >> gridSizeY;
        grid = initializeGridRandom(gridSizeX, gridSizeY);
    }

    cout << "Enter number of time steps (ex. 30): ";
    cin >> stepsNumber;

    // run simulation
    runSimulation(grid, stepsNumber);

    char answer;
    do{
        cout << "Save final constellation? (Y/N): ";
        cin >> answer;
    }while(answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');

    if(answer == 'Y' || answer == 'y'){
        string filename;
        cout << "Enter filename: ";
        cin >> filename;
        filename = "output/" + filename;

        string savedFile = saveFinalGrid(grid, filename);
        if(!savedFile.empty()) cout << "Final constellation saved to file: " << savedFile << endl;
        else cout << "Error saving final constellation!" << endl;
    }

    return 0;
}