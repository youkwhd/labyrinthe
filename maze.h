#ifndef __LABYRINTHE_MAZE_H
#define __LABYRINTHE_MAZE_H

#include <stddef.h>
#include <stdint.h>

#include "direction.h"
#include "coordinate.h"

typedef direction_t maze_node_t;

typedef struct maze_t {
    uint16_t height, width;
    maze_node_t **body;

    char **_maze_str;
    size_t _maze_str_width, _maze_str_height;
} maze_t;

void maze_init(maze_t *maze, uint16_t height, uint16_t width);
void maze_generate(maze_t *maze, coordinate_t start);
void maze_cleanup(maze_t *maze);

#endif // __LABYRINTHE_MAZE_H
