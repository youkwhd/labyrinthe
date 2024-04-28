#ifndef __LABYRINTHE_MAZE_H
#define __LABYRINTHE_MAZE_H

#include <stddef.h>
#include <stdint.h>

#include "direction.h"
#include "coordinate.h"

typedef direction_t maze_cell_t;

typedef struct maze_t {
    uint16_t height, width;
    maze_cell_t **grid;

    char **__maze_ui;
    size_t __maze_ui_width, __maze_ui_height;
} maze_t;

void maze_init(maze_t *maze, uint16_t width, uint16_t height);
void maze_generate(maze_t *maze, coordinate_t start);
void maze_ui_println(maze_t *maze);
void maze_cleanup(maze_t *maze);

#endif // __LABYRINTHE_MAZE_H
