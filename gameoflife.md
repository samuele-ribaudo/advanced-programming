# Project: Conway's Game Of Life

Idea contributed by [Ludwig Kratzl](ludwig.kratzl@tum.de).


## Build and Run

To build and run the project on macOS or Linux, please follow these steps in your terminal:

1. Clone the repository (if you haven't already):
```
git clone <repository-url>
cd <repository-directory>
```

2. Initialize and update the Google Test submodule:
```
git submodule update --init --recursive
```

3. Create a build directory and navigate into it:
```
mkdir build
cd build
```

4. Generate the build files using CMake:
```
cmake ..
```

5. Build the project:
```
cmake --build .
```

6. Run the main program (replace `GameOfLife` with the actual executable name if different):
```
./GameOfLife
```

7. To run the game_tests (assuming the tests executable is named `game_tests`):
```
./game_tests
```

These instructions assume the project is configured to include Google Test as a submodule and that CMakeLists.txt is properly set up to build both the main application and the tests.

## Motivation
[Conway's Game Of Life](https://de.wikipedia.org/wiki/Conways_Spiel_des_Lebens) is an easy realization of a cellular automaton, which models dynamic but discrete systems. 
In this project, we create a simulation that realizes such systems.

### Background
We consider an arbitrary grid of squares. Each component of the grid is called a (biological) cell. At the beginning (`t=0`), all
cells are either "dead" or "alive".
Now we are going to add dynamics to the scene by applying the following rules at any time step `t = t + 1`:

A living cell
* stays alive, if it hast exact 2 or 3 neighbors
* dies due to loneliness if it has fewer neighbors
* dies du to overpopulation if it has more neighbors

A dead cell
* will be reborn if it has exactly 3 neighbors
* remains dead otherwise

Be aware that updates to the cells are performed in parallel and that a neighbor of a cell is defined by edges and vertices
,i.e. any cell that is not located at the boundaries of the grid has exactly 8 neighbors.

### Example
See the following picture as an example grid (thankfully extracted from Prof. Bader's slides in Scientific Computing I).

![Grid Example](img01.png)

## Sprint 1
In this sprint, we want to create a first prototype that simulates the system. The basic logic needed to store, update and display grids of cells should be in place by the end of the sprint.

### Definition of "done"
* Primitive data types for the grid and cells are specified and implemented
* A file format to define grids of cells is specified. (this should be a simple text-based format e.g. (0=dead, 1=alive), optimizations can come later)
* The specified file format can be read into the specified data types ([this](https://www.udacity.com/blog/2021/05/how-to-read-from-a-file-in-cpp.html) tutorial may be helpful), edge cases are handled appropriately
* The simulation per time step as described above is implemented (hint: use an additional grid that holds the grid at `t+1`)
* The grid gets printed to the console after every update
* Basic test cases are implemented to verify that the simulation works as expected (some grids found on [wikipedia](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Examples_of_patterns) can be used)
* (optional) Unit tests are implemented (preferably using a testing library like [gtest](https://github.com/google/googletest) )

## Sprint 2
In this sprint, we want to create an interactive simulation and add more dynamics to the system. The program should also be refactored into classes to facilitate more complex behaviour and multiple simulations.

### Definition of "done"
* The code is refactored into classes and OOP principles are observed
* Classes for Cell and Grid have been added as well 
* The simulation logic is also stored in a separate class (try to experiment here with a second different update rule for all cells)
* A workflow that allows the user to specify a file to load via the command line is implemented. The workflow is also stored in a separate class
* The option to create a random grid is added to the workflow (look at [this](https://www.cplusplus.com/reference/cstdlib/rand/) for reference)
* The user can specify the number of time steps for which the simulation runs, and can start the simulation
* The program does not exit after the last time step 
* The simulation is extended to an infinite grid: if a boundary of the grid is reached, additional rows/columns are added
* (optional) The simulation is animated in the terminal window (this can be done by clearing the terminal and adding delays between simulation steps)

## Sprint 3
In this sprint, we want to extend the simulation to an infinite grid. This should be done in an efficient way to optimize performance, STL containers or custom data structures may be used.

### Definition of "done"
* The simulation does not use more memory than required (see optional last item, which is an extension of this)
* A better file format is used (look at the [PBM](https://en.wikipedia.org/wiki/Netpbm#File_formats) file format) and grids can be saved in this file format
* The workflow includes the option to save the grid at every time step as an image file
* Grids are saved correctly (maybe using the format `grid_i.pbm` ; open the files locally to see if the format is correctly implemented)
* Unit test are implemented to test the dynamic grid size
* A [CMakelists](https://cmake.org/cmake/help/latest/guide/tutorial/index.html) or [Makefile](https://makefiletutorial.com/) that builds and tests the application is created
* You can also have a look at the SCCS guides for [Make](https://gitlab.lrz.de/tum-i05/public/studentstarterclues/-/blob/master/tools/buildTools/make.md) and [CMake](https://gitlab.lrz.de/tum-i05/public/studentstarterclues/-/blob/master/tools/buildTools/cmake.md).
* (optional) A sparse representation of the grid is used, so that memory and runtime are of the order of the number of live cells, and not the number of grid squares. Think of run-length encoded compressions
* (optional) The cells are processed in parallel. For this, you can read [this](https://bisqwit.iki.fi/story/howto/openmp/) guide. Alternatively, you can try [pthreads](https://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm)
* (optional) Run a [perf](https://www.thomas-krenn.com/de/wiki/Linux_Performance_Counters_mit_perf_und_perf-tools) analysis and fix potential bottlenecks