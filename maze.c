#include "maze.h"

void maze_init(maze_t *maze)
{
    matrix_init(&maze->body, 5, 5, sizeof(uint8_t));
}

void maze_cleanup(maze_t *maze)
{
    matrix_cleanup(&maze->body);
}
