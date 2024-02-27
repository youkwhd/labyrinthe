#include <stdio.h>

#include "maze.h"
#include "labyrinthe.h"

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    maze_t maze;
    maze_init(&maze, 5, 5);
    maze_generate(&maze);

    maze_cleanup(&maze);
    return 0;
}
