# Advanced Programming Bonus Project: Conway's Game of Life

## By Simon Stark, Cedric Beyel and Samuele Ribaudo



## Sprint 1: Minimal Viable Implementation

- [x] Minimal viable implementation of Conway's Game of Life (random initial arrangement, fixed grid size).
- [x] Enable reading a simple text file (0 = dead, 1 = alive) to initialize the grid.
- [X] Save the final grid configuration in the same text format.
- [X] Allow selection of initialization method (random, file, size) via console input or execution parameters.
- [X] Allow saving of final configuration output via console input or execution parameters.
- [ ] Write unit tests for known configurations (e.g., patterns from Wikipedia).
- [ ] (Optional) Implement unit tests using a testing library (e.g., Google Test).
- [ ] Create a branch `sprint1` and contribute all work there (preferably via merge requests). **We need to keep the main-branch clean**
- [ ] Open a merge request from `sprint1` to the main branch.

---

## Sprint 2: Interactive Simulation & Refactoring

- [ ] extract parts of program into functions for better readability and adjustability (only possible when  working with vectors)
- [ ] Refactor code into classes, following OOP principles.
- [ ] Add classes for Cell and Grid.
- [ ] Store simulation logic in a separate class (experiment with alternative update rules).
- [ ] Implement workflow for specifying input file via command line (store workflow in a separate class).
- [ ] Add option to create a random grid in the workflow.
- [ ] Allow user to specify number of time steps and start simulation.
- [ ] Program should not exit after last time step.
- [ ] Extend simulation to infinite grid (add rows/columns when boundaries are reached).
- [ ] (Optional) Animate simulation in terminal (clear terminal, add delays).

---

## Sprint 3: Efficient Infinite Grid & Advanced Features

- [ ] Optimize simulation to use only required memory.
- [ ] Implement improved file format (PBM), support saving/loading grids in PBM format.
- [ ] Add workflow option to save grid as image file at every time step.
- [ ] Ensure grids are saved correctly (e.g., `grid_i.pbm`).
- [ ] Write unit tests for dynamic grid size.
- [ ] Create CMakeLists or Makefile to build and test the application.
- [ ] (Optional) Use sparse grid representation (e.g., run-length encoding) for memory/runtime efficiency.
- [ ] (Optional) Process cells in parallel (OpenMP, pthreads).
- [ ] (Optional) Run performance analysis (perf) and fix bottlenecks.
