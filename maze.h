#ifndef __LABYRINTHE_MAZE_H
#define __LABYRINTHE_MAZE_H

#include <stddef.h>
#include <stdint.h>

#include "direction.h"
#include "coordinate.h"

typedef direction_t maze_cell_t;

typedef enum maze_solving_strategy_t {
    MAZE_SOLVING_STRATEGY_A_STAR = 0,
    MAZE_SOLVING_STRATEGY_BFS,
    MAZE_SOLVING_STRATEGY_DFS,
} maze_solving_strategy_t;

typedef struct maze_t {
    uint16_t height, width;
    maze_cell_t **grid;
} maze_t;

void maze_init(maze_t *maze, uint16_t width, uint16_t height);
coordinate_t maze_generate(maze_t *maze, coordinate_t start);
void maze_reset_traversed(maze_t *maze);
void maze_solve(maze_t *maze, coordinate_t start, coordinate_t end, maze_solving_strategy_t strat);
void maze_println(maze_t *maze);
void maze_cleanup(maze_t *maze);

#endif // __LABYRINTHE_MAZE_H
