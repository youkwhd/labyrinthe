#ifndef __LABYRINTHE_MAZE_H
#define __LABYRINTHE_MAZE_H

#include <stddef.h>
#include <stdint.h>

#include "direction.h"
#include "tui.h"
#include "coordinate.h"

typedef direction_t maze_cell_t;

typedef struct maze_t {
    uint16_t height, width;
    maze_cell_t **grid;
    maze_tui_t *tui;
} maze_t;

void maze_init(maze_t *maze, uint16_t width, uint16_t height);
void maze_generate(maze_t *maze, coordinate_t start);
void maze_println(maze_t *maze);
void maze_cleanup(maze_t *maze);

#endif // __LABYRINTHE_MAZE_H
