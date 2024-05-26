#include "maze.h"
#include "args.h"
#include "coordinate.h"

#define MAZE_WIDTH 8
#define MAZE_HEIGHT 5

int main(int argc, char **argv)
{
    args_t args;
    args_parse(&args, argc, argv);

    maze_t maze;
    maze_init(&maze, MAZE_WIDTH, MAZE_HEIGHT);
    coordinate_t end = maze_generate(&maze, args.starting_point);
    maze_println(&maze);
    maze_solve(&maze, args.starting_point, end, MAZE_SOLVING_STRATEGY_A_STAR);

    maze_cleanup(&maze);
    return 0;
}
