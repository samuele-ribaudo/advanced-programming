// use SDL2 or raylib to render the game
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <stdlib.h>
#include <vector>

#include <chrono>
#include <thread>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::atoi;

class Cell
{
public:
    bool isAlive;
    bool nextState;
};

void drawLine(int l);

int printIntroduction(){
    // Print introduction message ad returns:
    //      0 if the user wants to initialize a random grid
    //      1 if the user wants to load a grid from a file

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


void printBoard(const std::vector<std::vector<Cell>> &grid){
    system("clear");
    int gridSizeY = grid[0].size();
    int gridSizeX = grid.size();

    drawLine(gridSizeX);

    for(int y = 0; y < gridSizeY; y++){
        for(int x = 0; x < gridSizeX; x++){
            if (grid[x][y].isAlive) cout << "█";
            else cout << " ";
        }
        cout << endl;
    }
    drawLine(gridSizeX);
}


std::vector<std::vector<Cell>> initializeGridFromFile(int &gridSizeX, int &gridSizeY){
    
    std::ifstream patternFile;
    string filename;

    while(true){
        cout << "Enter filename: ";
        cin >> filename;
        patternFile.open(filename);

        if(patternFile.is_open() && patternFile.good()) break;

        cout << "Error opening file! Try again..." << endl;
        patternFile.clear();
    }

    string sizeStr;
    patternFile >> sizeStr;
    gridSizeX = std::stoi(sizeStr);
    patternFile >> sizeStr;
    gridSizeY = std::stoi(sizeStr);

    std::vector<std::vector<Cell>> grid(gridSizeX, std::vector<Cell>(gridSizeY));

    for(int y = 0; y < gridSizeY; y++){
        for(int x = 0; x < gridSizeX; x++){
            char c;
            do{
                c = patternFile.get();
            }while(std::isspace(static_cast<unsigned char>(c)) && patternFile.good());

            grid[x][y].isAlive = (c == '1');
            grid[x][y].nextState = false;
        }
    }
    patternFile.close();
    return grid;
}


std::vector<std::vector<Cell>> initializeGridRandom(int &gridSizeX, int &gridSizeY) {
    cout << "Enter grid size X (ex. 80): ";
    cin >> gridSizeX;
    cout << "Enter grid size Y (ex. 20): ";
    cin >> gridSizeY;

    std::vector<std::vector<Cell>> grid(gridSizeX, std::vector<Cell>(gridSizeY));

    for (int y = 0; y < gridSizeY; y++) {
        for (int x = 0; x < gridSizeX; x++) {
            int randNum = rand() % 100;
            grid[x][y].isAlive = (randNum < 20);
            grid[x][y].nextState = false;
        }
    }
    return grid;
}


int main()
{
    srand(time(NULL));

    int max_step = 30;
    int gridSizeX = 80;
    int gridSizeY = 20;
    std::vector<std::vector<Cell>> grid;

    if(printIntroduction()){
        // load from file
        grid = initializeGridFromFile(gridSizeX, gridSizeY);
    }else{
        // random initialization
        grid = initializeGridRandom(gridSizeX, gridSizeY);
    }

    cout << "Enter number of time steps (ex. 30): ";
    cin >> max_step;

    // Sim loop
    for (int step = 0; step <= max_step; step++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // print grid
        printBoard(grid);
        cout << "Iteration: " << step << " / " << max_step << "\n";

        if (step == max_step)
            break;
        // calculate next state
        for (int y = 0; y < gridSizeY; y++)
        {
            for (int x = 0; x < gridSizeX; x++)
            {
                // count the neighbors that are alive
                int aliveNeighbours = 0;
                for (int x_off = -1; x_off < 2; x_off++)
                {
                    for (int y_off = -1; y_off < 2; y_off++)
                    {
                        // ignore itself
                        if (x_off == 0 && y_off == 0)
                            continue;

                        int x_neighbour = x + x_off;
                        int y_neighbour = y + y_off;

                        // skip if the cell is at the edge
                        if (x_neighbour < 0 || x_neighbour >= gridSizeX || y_neighbour < 0 || y_neighbour >= gridSizeY)
                             continue;

                        /*
                        // Loop at edge
                        if (x_neighbour < 0)
                            x_neighbour = gridSizeX - 1;
                        if (x_neighbour >= gridSizeX)
                            x_neighbour = 0;
                        if (y_neighbour < 0)
                            y_neighbour = gridSizeY - 1;
                        if (y_neighbour >= gridSizeY)
                            y_neighbour = 0;
                        */

                        if (grid[x_neighbour][y_neighbour].isAlive)
                            aliveNeighbours += 1;
                    }
                }

                // Rules:
                // 1) Any live cell with fewer than two live neighbours dies
                // 2) Any live cell with two or three live neighbours lives on to the next generation
                // 3) Any live cell with more than three live neighbours dies
                // 4) Any dead cell with exactly three live neighbours becomes a live cell

                if (grid[x][y].isAlive)
                {
                    if (aliveNeighbours == 2 || aliveNeighbours == 3) {
                        grid[x][y].nextState = true;
                    }
                    else {
                        grid[x][y].nextState = false;
                    }
                }
                else
                {
                    if (aliveNeighbours == 3)
                        grid[x][y].nextState = true;
                    else
                        grid[x][y].nextState = false;
                }
            }
        }

        // apply changes
        for (int y = 0; y < gridSizeY; y++)
        {
            for (int x = 0; x < gridSizeX; x++)
            {
                grid[x][y].isAlive = grid[x][y].nextState;
            }
        }
    }
    string saveFinalConstellation;
    cout << "Save final constellation? (Y/N): ";
    cin >> saveFinalConstellation;
    if (saveFinalConstellation == "Y") {
        cout << "Enter filename: ";
        cin >> saveFinalConstellation;
        if(saveFinalConstellation.find(".txt") == string::npos) saveFinalConstellation += ".txt";
        std::ofstream outFile (saveFinalConstellation);
        outFile << gridSizeX << endl << gridSizeY << endl;
        for (int y = 0; y < gridSizeY; y++)
        {
            for (int x = 0; x < gridSizeX; x++)
            {
                if (grid[x][y].isAlive)
                {
                    outFile << "1";
                }
                else
                {
                    outFile << "0";
                }
            }
            outFile << endl;
        }

    }
    return 0;
}

void drawLine(int l)
{

    for (int i = 0; i < l; i++)
    {
        cout << "-";
    }
    cout << '\n';
}
