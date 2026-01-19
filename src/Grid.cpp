#include "Grid.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <deque>
#include <string>
#include "Cell.h"

// Default constructor: creates an empty grid
Grid::Grid() : width(0), height(0) {}

// Constructor: initializes a grid of given width (w) and height (h) with dead cells
Grid::Grid(int w, int h) : width(w), height(h), cells(w, std::deque<Cell>(h)) {}

// Returns the current width of the grid
int Grid::getWidth() const { return width; }

// Returns the current height of the grid
int Grid::getHeight() const { return height; }

// Returns a reference to the cell at position (x, y)
Cell& Grid::at(int x, int y) { return cells[x][y]; }
const Cell& Grid::at(int x, int y) const { return cells[x][y]; }

// Checks if coordinates (x, y) are inside the grid boundaries
bool Grid::isInside(int x, int y) const { return x >= 0 && x < width && y >= 0 && y < height; }

// Loads grid size and cell states from a PBM file at 'path'
// Returns true if successful, false otherwise
bool Grid::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string magic;
    file >> magic;
    
    // Strict PBM check
    if (magic != "P1") {
        return false; 
    }
    
    file >> width >> height;
    cells.assign(width, std::deque<Cell>(height)); // Resize & Clear

    int val;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            file >> val;
            cells[x][y].setAlive(val == 1);
            cells[x][y].resetNextState();
        }
    }
    return true;
}


// Randomly initializes a grid of width w, height h, with probability p (0-100) of cells being alive
void Grid::randomInit(int w, int h, int p) {
    //initializeGridRandom() code
    width = w;
    height = h;

    if(p < 0 || p > 100) p = 20; // default probability if out of range

    cells.resize(width, std::deque<Cell>(height));

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int randNum = rand() % 100;
            cells[x][y].setAlive(randNum < p);
            cells[x][y].resetNextState();
        }
    }
}


// Saves the current grid state to a text file at 'path'. Returns true if successful
bool Grid::saveToFile(const std::string& path) const {
    std::string filename = path;

    if (filename.find(".pbm") == std::string::npos) filename += ".pbm";

    std::ofstream file(filename);
    if (!file.is_open()) return false;

    file << "P1\n" << width << " " << height << "\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            file << (cells[x][y].isAliveNow() ? "1 " : "0 ");
        }
        file << "\n";
    }
    return true;
}


// Applies the classic Game of Life rules to each cell
// 'resize' indicates whether the grid should expand if alive cells reach boundaries
// Rules:
//  1) Any live cell with fewer than two live neighbours dies
//  2) Any live cell with two or three live neighbours lives on to the next generation
//  3) Any live cell with more than three live neighbours dies
//  4) Any dead cell with exactly three live neighbours becomes a live cell
void Grid::stepClassicRules(bool resize) {
    //calculateNextState() code

    // Determine next state for each cell
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int aliveNeighbors = countAliveNeighbors(x, y);
            if(cells[x][y].isAliveNow()){
                // Any live cell with two or three live neighbours survives.
                if(aliveNeighbors == 2 || aliveNeighbors == 3){
                    cells[x][y].setNextState(true);
                } else {
                    cells[x][y].setNextState(false);
                }
            } else {
                // Any dead cell with exactly three live neighbours becomes a live cell.
                if(aliveNeighbors == 3){
                    cells[x][y].setNextState(true);
                } else {
                    cells[x][y].setNextState(false);
                }
            }
        }
    }

    // resize grid if needed (infinity grid)
    if(resize) resizeIfNeeded();

    //apply changes
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            cells[x][y].applyNextState();
        }
    }
}

// Placeholder for alternative rules;
//  1) Any live cell with fewer than two live neighbours dies
//  2) Any live cell with two or four live neighbours lives on to the next generation
//  3) Any live cell with three live neighbours or more than four dies
//  4) Any dead cell with exactly three or four live neighbours becomes a live cell
void Grid::stepAlternativeRules(bool resize) {
    // Too boring to implement
    // Determine next state for each cell
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int aliveNeighbors = countAliveNeighbors(x, y);
            if(cells[x][y].isAliveNow()){
                // Any live cell with two or four live neighbours survives.
                if(aliveNeighbors == 2 || aliveNeighbors == 4){
                    cells[x][y].setNextState(true);
                } else {
                    cells[x][y].setNextState(false);
                }
            } else {
                // Any dead cell with exactly three live neighbours becomes a live cell.
                if(aliveNeighbors == 3 || aliveNeighbors == 4){
                    cells[x][y].setNextState(true);
                } else {
                    cells[x][y].setNextState(false);
                }
            }
        }
    }

    // resize grid if needed (infinity grid)
    if(resize) resizeIfNeeded();

    //apply changes
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            cells[x][y].applyNextState();
        }
    }
}

// Prints a horizontal line of length l
void drawLine(int l){
    for (int i = 0; i < l; i++) std::cout << "-";
    std::cout << '\n';
}

// Prints the current grid to the console, live cells as '█', dead as space
void Grid::print() const {
    // printBoard() code
    drawLine(width);
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if (cells[x][y].isAliveNow()) std::cout << "█";
            else std::cout << " ";
        }
        std::cout << std::endl;
    }
    drawLine(width);
}

// Counts the number of alive neighbors around the cell at (x, y)
int Grid::countAliveNeighbors(int x, int y) const {
    // count alive neighbors (ignoring out-of-bounds)
    int count = 0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 && j == 0) continue;
            int nx = x + i;
            int ny = y + j;
            if(isInside(nx, ny) && cells[nx][ny].isAliveNow()) count++;
        }
    }
    return count;
}

// Expands the grid by one row/column on any side if live cells are on the edge
void Grid::resizeIfNeeded() {
    // TOP row (y=0)
    for (int x = 0; x < width; x++) {
        if (cells[x][0].isAliveNow()) {
            for (int xCol = 0; xCol < width; xCol++) {
                cells[xCol].push_front(Cell());
            }
            height++;
            break;
        }
    }

    // BOTTOM row (y=height-1)
    for (int x = 0; x < width; x++) {
        if (cells[x][height-1].isAliveNow()) {
            for (int xCol = 0; xCol < width; xCol++) {
                cells[xCol].push_back(Cell());
            }
            height++;
            break;
        }
    }

    // LEFT column (x=0)
    for (int y = 0; y < height; y++) {
        if (cells[0][y].isAliveNow()) {
            cells.push_front(std::deque<Cell>(height));
            width++;
            break;
        }
    }

    // RIGHT column (x=width-1)
    for (int y = 0; y < height; y++) {
        if (cells[width-1][y].isAliveNow()) {
            cells.push_back(std::deque<Cell>(height));
            width++;
            break;
        }
    }
}