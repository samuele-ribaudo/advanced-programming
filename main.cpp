// use SDL2 or raylib to render the game
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>

#include <chrono>
#include <thread>

using std::cin;
using std::cout;
using std::string;
using std::atoi;

class Cell
{
public:
    bool isAlive;
    bool nextState;
};

void DrawLine(int l);

int main(int argc, char* argv[])
{
    srand(time(0));

    int max_step, gridSizeX, gridSizeY;

    switch (argc)
    {
        case(1):
        {
            max_step = 300;
            gridSizeX = 80;
            gridSizeY = 20;
            break;
        }
        case(2):
        {
            max_step = std::atoi(argv[1]);
            gridSizeX = 80;
            gridSizeY = 20;
            break;
        }
        case(4):
        {
            max_step = std::atoi(argv[1]);
            gridSizeX = std::atoi(argv[2]);
            gridSizeY = std::atoi(argv[3]);
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

            int randNum = rand() % 100;
            if (randNum < 20)
                grid[x][y].isAlive = true;
            else
                grid[x][y].isAlive = false;
        }
    }

    // Sim loop
    int loop = 0;
    while (loop < max_step)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        loop++;

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
            cout << '\n';
        }
        DrawLine(gridSizeX);
        cout << "Iteration: " << loop << '\n';

        // calculate next state
        for (int y = 0; y < gridSizeY; y++)
        {
            for (int x = 0; x < gridSizeX; x++)
            {
                // check wether neighbors are alive
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
