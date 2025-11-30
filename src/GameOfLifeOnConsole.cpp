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


// =========================================================
// Draws a horizontal line of length l using '-' characters.
// Parameters:
//   - l: length of the line to draw.
// Returns: None
// =========================================================
void drawLine(int l){
    for (int i = 0; i < l; i++) cout << "-";
    cout << '\n';
}


// ====================================================================================
// Displays the introduction menu and prompts the user to choose initialization method.
// Parameters: None
// Returns: 0 for random grid initialization, 1 for loading grid from file.
// ====================================================================================
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


// ==========================================================
// Prints the current state of the game board to the console.
// Parameters:
//   - grid: 2D vector of Cells representing the game grid.
// Returns: None
// ==========================================================
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


// ======================================================
// Initializes the grid by loading a pattern from a file.
// Parameters:
//   - gridSizeX: reference to int to store grid width.
//   - gridSizeY: reference to int to store grid height.
//   - filename: string filename to load grid from
// Returns: 2D vector of Cells initialized from the file.
// ======================================================
std::vector<std::vector<Cell>> initializeGridFromFile(int &gridSizeX, int &gridSizeY, const std::string &filename){
    
    std::ifstream patternFile(filename);

    while(!patternFile.is_open() || !patternFile.good()){
        cout << "Error opening file: " << filename << endl;
        return std::vector<std::vector<Cell>>(); // return empty grid on error
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


// ================================================================================
// Initializes the grid randomly with a given probability of alive cells.
// Parameters:
//   - gridSizeX: grid width.
//   - gridSizeY: grid height.
//   - p: integer percentage probability (0-100) that a cell is alive (default 20).
// Returns: 2D vector of Cells initialized randomly.
// ================================================================================
std::vector<std::vector<Cell>> initializeGridRandom(int gridSizeX, int gridSizeY, int p = 20){
    
    if(p < 0 || p > 100) p = 20; // default probability if out of range

    std::vector<std::vector<Cell>> grid(gridSizeX, std::vector<Cell>(gridSizeY));

    for (int y = 0; y < gridSizeY; y++){
        for (int x = 0; x < gridSizeX; x++){
            int randNum = rand() % 100;
            grid[x][y].isAlive = (randNum < p);
            grid[x][y].nextState = false;
        }
    }
    return grid;
}


// =======================================================================================
// Calculates the next state of each cell in the grid according to the Game of Life rules.
// Parameters:
//   - grid: 2D vector of Cells representing the current game grid (modified in place).
// Returns: None
// =======================================================================================
void calculateNextState(std::vector<std::vector<Cell>> &grid){
    
    int gridSizeX = grid.size();
    int gridSizeY = grid[0].size();

    for(int y = 0; y < gridSizeY; y++){
        for(int x = 0; x < gridSizeX; x++){
            //count the neighbors that are alive
            int aliveNeighbours = 0;
            for(int x_off = -1; x_off < 2; x_off++){
                for(int y_off = -1; y_off < 2; y_off++){
                    //ignore itself
                    if(x_off == 0 && y_off == 0) continue;
                    
                    int x_neighbour = x + x_off;
                    int y_neighbour = y + y_off;

                    //skip if the cell is at the edge
                    if(x_neighbour < 0 || x_neighbour >= gridSizeX || y_neighbour < 0 || y_neighbour >= gridSizeY) continue;
                    if(grid[x_neighbour][y_neighbour].isAlive) aliveNeighbours++;
                }
            }

            // Rules:
            // 1) Any live cell with fewer than two live neighbours dies
            // 2) Any live cell with two or three live neighbours lives on to the next generation
            // 3) Any live cell with more than three live neighbours dies
            // 4) Any dead cell with exactly three live neighbours becomes a live cell

            if(grid[x][y].isAlive){
                if(aliveNeighbours == 2 || aliveNeighbours == 3) grid[x][y].nextState = true;
                else grid[x][y].nextState = false;
            
            }else{
                if(aliveNeighbours == 3) grid[x][y].nextState = true;
                else grid[x][y].nextState = false;
            }
        }
    }

    // apply changes
    for(int y = 0; y < gridSizeY; y++){
        for(int x = 0; x < gridSizeX; x++){
            grid[x][y].isAlive = grid[x][y].nextState;
        }
    }
}


// ========================================================================================
// Runs the Game of Life simulation for a given number of steps with a delay between steps.
// Parameters:
//   - grid: 2D vector of Cells representing the game grid (modified in place).
//   - stepsNumber: number of iterations to run the simulation.
//   - delayMs: delay in milliseconds between each iteration (default 100).
// Returns: None
// ========================================================================================
void runSimulation(std::vector<std::vector<Cell>> &grid, int stepsNumber, int delayMs = 100){
    int gridSizeX = grid.size();
    int gridSizeY = grid[0].size();

    for(int step = 0; step <= stepsNumber; step++){
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));

        // print grid
        printBoard(grid);
        cout << "Iteration: " << step << " / " << stepsNumber << "\n";

        if (step == stepsNumber)
            break;

        // calculate next state
        calculateNextState(grid);
    }
}


// ======================================================================================
// Saves the final grid state to a text file.
// Parameters:
//   - grid: 2D vector of Cells representing the final game grid.
//   - filename: optional string filename to save to (default "final_constellation.txt").
// Returns: the filename used for saving (empty string if error).
// ======================================================================================
std::string saveFinalGrid(const std::vector<std::vector<Cell>> &grid, std::string filename = "final_constellation.txt"){
    int gridSizeX = grid.size();
    int gridSizeY = grid[0].size();

    if(filename.find(".txt") == string::npos) filename += ".txt";
    
    std::ofstream outFile (filename);
    if(!outFile.is_open()){
        cout << "Error saving file!" << endl;
        return "";
    }

    outFile << gridSizeX << endl << gridSizeY << endl;
    for(int y = 0; y < gridSizeY; y++){
        for(int x = 0; x < gridSizeX; x++){
            if(grid[x][y].isAlive) outFile << "1";
            else outFile << "0";
        }
        outFile << endl;
    }
    outFile.close();
    
    return filename;
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

        string savedFile = saveFinalGrid(grid, filename);
        if(!savedFile.empty()) cout << "Final constellation saved to file: " << savedFile << endl;
        else cout << "Error saving final constellation!" << endl;
    }

    return 0;
}