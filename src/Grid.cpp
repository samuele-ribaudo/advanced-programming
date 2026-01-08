#include "Grid.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include "Cell.h"


Grid::Grid() : width(0), height(0) {}

Grid::Grid(int w, int h) : width(w), height(h), cells(w, std::vector<Cell>(h)) {}

int Grid::getWidth() const { return width; }
int Grid::getHeight() const { return height; }


Cell& Grid::at(int x, int y) { return cells[x][y]; }

const Cell& Grid::at(int x, int y) const { return cells[x][y]; }

bool Grid::isInside(int x, int y) const { return x >= 0 && x < width && y >= 0 && y < height; }


bool Grid::loadFromFile(const std::string& path) {
    // initializeGridFromFile() code
    std::ifstream patternFile(path);
    while(!patternFile.is_open() || !patternFile.good()){
        return false;
    }

    std::string sizeStr;
    patternFile >> sizeStr;
    width = std::stoi(sizeStr);
    patternFile >> sizeStr;
    height = std::stoi(sizeStr);

    cells.resize(width, std::vector<Cell>(height));

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            char c;
            do{
                c = patternFile.get();
            }while(std::isspace(static_cast<unsigned char>(c)) && patternFile.good());

            cells[x][y].setAlive(c == '1');
            cells[x][y].resetNextState();
        }
    }
    patternFile.close();
    return true;
}


void Grid::randomInit(int w, int h, int p) {
    //initializeGridRandom() code
    width = w;
    height = h;

    if(p < 0 || p > 100) p = 20; // default probability if out of range

    cells.resize(width, std::vector<Cell>(height));

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int randNum = rand() % 100;
            cells[x][y].setAlive(randNum < p);
            cells[x][y].resetNextState();
        }
    }
}


bool Grid::saveToFile(const std::string& path) const {
    //saveFinalGrid() code
    std::string filename = path;
    if(filename.find(".txt") == std::string::npos) filename += ".txt";

    std::ofstream outFile (filename);
    if(!outFile.is_open()){
        std::cout << "Error saving file!" << std::endl;
        return false;
    }

    outFile << width << std::endl << height << std::endl;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(cells[x][y].isAliveNow()) outFile << "1";
            else outFile << "0";
        }
        outFile << std::endl;
    }
    outFile.close();
    return true;
}

void Grid::stepClassicRules() {
    //calculateNextState() code

    // Rules:
    // 1) Any live cell with fewer than two live neighbours dies
    // 2) Any live cell with two or three live neighbours lives on to the next generation
    // 3) Any live cell with more than three live neighbours dies
    // 4) Any dead cell with exactly three live neighbours becomes a live cell

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
    resizeIfNeeded();

    //apply changes
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            cells[x][y].applyNextState();
        }
    }
}

void Grid::stepAlternativeRules() {
    // Too boring to implement
}

void drawLine(int l){
    for (int i = 0; i < l; i++) std::cout << "-";
    std::cout << '\n';
}

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


void Grid::resizeIfNeeded() {
    // expand the grid if a live cell reaches the border (infinity grid)
    // Check TOP row (y=0)
    for (int x = 0; x < width; x++) {
        if (cells[x][0].isAliveNow()) {
            // Add new empty row at TOP (y=-1)
            cells.insert(cells.begin(), std::vector<Cell>(width));
            height++;
            break;
        }
    }
    
    // Check BOTTOM row (y=height-1)
    for (int x = 0; x < width; x++) {
        if (cells[x][height-1].isAliveNow()) {
            // Add new empty row at BOTTOM (y=height)
            cells.push_back(std::vector<Cell>(width));
            height++;
            break;
        }
    }
    
    // Check LEFT column (x=0) - controlla TUTTE le righe
    for (int y = 0; y < height; y++) {
        if (cells[0][y].isAliveNow()) {
            // Add new empty column at LEFT (x=-1)
            for (int y_new = 0; y_new < height; ++y_new) {
                cells[y_new].insert(cells[y_new].begin(), Cell());
            }
            width++;
            break;
        }
    }
    
    // Check RIGHT column (x=width-1)
    for (int y = 0; y < height; y++) {
        if (cells[width-1][y].isAliveNow()) {
            // Add new empty column at RIGHT (x=width)
            for (int y_new = 0; y_new < height; ++y_new) {
                cells[y_new].push_back(Cell());
            }
            width++;
            break;
        }
    }
}