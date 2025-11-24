// use SDL2 or raylib to render the game
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <ctime>
#include <stdlib.h>

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

void DrawLine(int l);

/*
parameters for execution:
- None -> creates random 80x20 grid with 20% probability of alive cells, runs 30 time steps
- 1 parameter: no. time steps -> creates random 80x20 grid with 20% probability of alive cells,
                                 uns given no. of time steps
- 2 parameter: no. time steps, filename -> opens given file, that has grid size seperated and followed by line breaks, followed by
                                 representation of alive ('1') and dead (any "non-space" char) cells, all endl and spacings are ignored,
                                 if not enough cells are defined, the grid is filled with dead cellc,
                                 runs given no. of time steps
- 3 parameters: no. time steps, grideSizeX, gridSizeY ->
                                 creates random grideSizeX x gridSizeY grid with 20% probability of alive cells,
                                 runs given no. time steps
*/
int main(int argc, char* argv[])
{
    srand(time(0));

    int max_step, gridSizeX, gridSizeY;
    bool isFileInput = false;
    std::ifstream patternFile;

    switch (argc)
    {
        case(1):
        {
            max_step = 30;
            gridSizeX = 80;
            gridSizeY = 20;
            break;
        }
        case(2):
        {
            max_step = atoi(argv[1]);
            gridSizeX = 80;
            gridSizeY = 20;
            break;
        }
        case(3):
        {
            max_step = atoi(argv[1]);
            patternFile.open(argv[2]);
            string sizeStr;
            patternFile >> sizeStr;
            gridSizeX = std::stoi(sizeStr);
            patternFile >> sizeStr;
            gridSizeY = std::stoi(sizeStr);
            isFileInput = true;
            break;
        }
        case(4):
        {
            max_step = atoi(argv[1]);
            gridSizeX = atoi(argv[2]);
            gridSizeY = atoi(argv[3]);
            break;
        }
    }


    // create grid with random alive cells
    Cell grid[gridSizeX][gridSizeY];
    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {
            grid[x][y].nextState = false;

            if (isFileInput)
            {
                if ( patternFile.is_open() && patternFile.good())
                {
                    char c;
                    do {
                        c = patternFile.get();
                    } while (std::isspace(static_cast<unsigned char>(c)) && patternFile.good());
                    grid[x][y].isAlive = c == '1';
                }
                else
                {
                    grid[x][y].isAlive = false;
                }
            }
            else
            {
                int randNum = rand() % 100;
                grid[x][y].isAlive = randNum < 20;
            }
        }
    }
    if (isFileInput && patternFile)
        patternFile.close();

    // Sim loop
    for (int step = 0; step <= max_step; step++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // print grid
        system("clear");
        DrawLine(gridSizeX);

        for (int y = 0; y < gridSizeY; y++)
        {
            for (int x = 0; x < gridSizeX; x++)
            {
                if (grid[x][y].isAlive)
                {
                    cout << "X";
                }
                else
                {
                    cout << " ";
                }
            }
            cout << endl;
        }
        DrawLine(gridSizeX);
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

void DrawLine(int l)
{

    for (int i = 0; i < l; i++)
    {
        cout << "-";
    }
    cout << '\n';
}
