#include "maze.h"
#include "coordinate.h"
#include "labyrinthe.h"

#define MAZE_WIDTH 8
#define MAZE_HEIGHT 5

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    maze_t maze;
    maze_init(&maze, MAZE_WIDTH, MAZE_HEIGHT);
    maze_generate(&maze, (coordinate_t){ 0, 0 });
    __maze_ui_println(&maze);

    maze_cleanup(&maze);
    return 0;
}
