#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "args.h"
#include "coordinate.h"

#ifdef GUI
    #include "gui.h"
    static bool __gui = true;
#else 
    static bool __gui = false;
#endif // GUI

int main(int argc, char **argv)
{
    args_t args;
    args_parse(&args, argc, argv);

    if (!__gui && args.use_gui) {
        fprintf(stderr, "%s: No GUI available.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    maze_t maze;
    maze_init(&maze, args.maze_width, args.maze_height);

    coordinate_t start = args.starting_point;
    coordinate_t end = maze_generate(&maze, start);

    if (args.solve) {
        maze_solve(&maze, start, end, args.solving_strat);
    }

    maze_println(&maze);

#ifdef GUI
    if (args.use_gui) {
        gui_init(args.maze_width * GUI_CELL_SIZE, args.maze_height * GUI_CELL_SIZE);

        while (!gui_should_close()) {
            if (gui_key_pressed(KEY_SPACE)) {
                maze_reset(&maze);
                end = maze_generate(&maze, start);

                if (args.solve) {
                    maze_solve(&maze, start, end, args.solving_strat);
                }

                maze_println(&maze);
            }

            gui_begin_drawing();
            gui_draw_bg();
            gui_draw_maze(&maze);
            gui_draw_maze_path(&maze);
            gui_end_drawing();
        }

        gui_cleanup();
    }
#endif // GUI

    maze_cleanup(&maze);
    return 0;
}
