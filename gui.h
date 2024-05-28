#ifndef __LABYRINTHE_GUI_H
#define __LABYRINTHE_GUI_H

#include <raylib.h>
#include "maze.h"

#define GUI_WIN_WIDTH 400
#define GUI_WIN_HEIGHT 300

void gui_init();
void gui_run(maze_t *maze);
void gui_cleanup();

#endif // __LABYRINTHE_GUI_H
