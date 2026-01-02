# Complete UML-Class specification – Game of Life

```- private```, ```+ public```, ```# protected```

## class Display

```- Run& current_run```

```- int ms_displaytime```
_0 for stepwise confirmed execution, otherwise wait time in ms_

```- int display_min_x, display_min_y, display_max_x, display_max_y```
_border of display_

```- bool extend_display_automatically```
_display extends automatically if grid grows_

```- bool tighten_display_automatically```
_display tightens automatically to fit active cells_

```+ Display()```
_initialises Display, creates Run via GridInit_

```+ void execute()```
_main loop: update-steps, Display, UX_

```+ void show()```
_outputs write() + info + user options in terminal_

```+ string write() const```
_string-representation of the grid within the display borders_

```+ void interrupt()```
_dialogue for the interruption of the execution, can lead to changing of display or simulation options_

```+ void save_grid() const```
_save current grid in a file_

```+ void change_display_options()```
_edit display options_

---

## class Run

```- Grid current_grid```

```- Grid original_grid```

```- Rule current_rule```

```- int current_step```

```- int max_step```

```+ Run(const Grid& grid, const Rule& rule, int max_step = 1000)```
_sets current_step = 0, current_grid = copy of grid_

```+ void update()```
_calculates next grid based on current_rule, overwrites current_grid_

```+ void change_simulation_options()```
_change of Rule, max_step etc._

```+ void reset()```

```+ const Grid& get_current_grid() const```

```+ const Grid& get_original_grid() const```

```+ const Rule& get_current_rule() const```

```+ int get_current_step() const```

```+ int get_max_step() const```

```+ void set_current_grid(const Grid&)```

```+ void set_current_rule(const Rule&)```

```+ void set_max_step(int)```

---

## struct Cell

```+ int32_t x```

```+ int32_t y```

```+ bool operator==(const Cell&) const```
_compares x and y_

```+ size_t hash() const```
_combines x and y to a 64-Bit Hash value for unordered_set/map_

---

## class Grid

```- unordered_set<Cell> alive_cells```

```- int min_x, min_y, max_x, max_y```

```+ Grid()```

```+ Grid(const unordered_set<Cell>& cells)```
_automatically sets min_x, max_y, etc... with respective maximal values_

```+ Grid(const unordered_set<Cell>& cells, int min_x, int min_y, int max_x, int max_y)```

```+ string to_string() const```
_summary of grid size and active cell count_

```+ string write_all_cells() const```
_complete representation of grid_

```+ bool operator==(const Grid& other) const```
_compares the outputs of write_all_cells()_

```+ Grid copy_with_new_cells(const unordered_set<Cell>&)```

```+ int alive_cell_count() const```

```+ const unordered_set<Cell>& get_alive_cells() const```

```+ int get_min_x() const```

```+ int get_min_y() const```

```+ int get_max_x() const```

```+ int get_max_y() const```

```+ void set_min_x(int)```

```+ void set_min_y(int)```

```+ void set_max_x(int)```

```+ void set_max_y(int)```

---

## class Rule

```- vector<pair<int,int>> relative_neighbor_map```

```- int survival_count```

```- int birth_count```

```- bool edge_drop_off```

```+ Rule()```
_creation of standard Conway's Game of Life_

```+ Rule(string b_s_format, bool edge_drop_off = false)```

```+ Rule(int birth_count, int survival_count, bool edge_drop_off = false)```

```+ Rule(const vector<pair<int,int>>& neighbors, int survival_count, int birth_count, bool edge_drop_off = false)```

```+ vector<Cell> get_neighbors(const Cell& cell) const```

```+ bool cell_survives(const Grid& prev_grid, const Cell& cell, int no_of_neighbors) const```

```+ string to_string() const```

---

## class GridInit (abstract)

```# Rule rule```

```# string rule_flag```

```# string init_flag```

```# int initial_min_x = 0```

```# int initial_min_y = 0```

```# int initial_max_x```

```# int initial_max_y```

```# int initial_current_step = 0```

```# int initial_max_step```

```+ GridInit()```

```+ GridInit(string init_flag)```

```+ GridInit(string rule_flag, string init_flag)```

```+ Run create()```

```- Rule execute_rule_flag(string rule_flag)```

```# virtual void execute_init_flag() = 0```

```# virtual Grid set_initial_cells() = 0```

---

## class GridInitRandom : public GridInit

```# double probability```

```# void execute_init_flag()```

```# Grid set_initial_cells()```

---

## class GridInitFile : public GridInit

```# string file_name```

```# void execute_init_flag()```

```# Grid set_initial_cells()```

---

## class GridInitStructure : public GridInit

```# vector<string> structure_names```

```# void execute_init_flag()```

```# Grid set_initial_cells()```
