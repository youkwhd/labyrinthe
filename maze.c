#include "maze.h"

void maze_init(maze_t *maze)
{
    matrix_init(&maze->body);
    matrix_cleanup(&maze->body);
}
