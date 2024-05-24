#include "maze.h"
#include "args.h"
#include "coordinate.h"
#include "labyrinthe.h"

#define MAZE_WIDTH 8
#define MAZE_HEIGHT 5

int main(int argc, char **argv)
{
    args_t args;
    args_parse(&args, argc, argv);

    maze_t maze;
    maze_init(&maze, MAZE_WIDTH, MAZE_HEIGHT);
    maze_generate(&maze, (coordinate_t){ 0, 0 });
    maze_println(&maze);

    maze_cleanup(&maze);
    return 0;
}
