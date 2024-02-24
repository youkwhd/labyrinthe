#include "maze.h"

void maze_init(maze_t *maze)
{
    matrix_init(&maze->body);
}

void maze_cleanup(maze_t *maze)
{
    matrix_cleanup(&maze->body);
}
