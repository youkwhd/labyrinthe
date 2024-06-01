#ifndef __LABYRINTHE_GUI_H
#define __LABYRINTHE_GUI_H

#include <raylib.h>
#include "maze.h"

#define GUI_CELL_SIZE 20

void gui_init(int width, int height);
void gui_draw_maze(maze_t *maze);
void gui_draw_maze_path(maze_t *maze);
void gui_draw_bg();

#define gui_begin_drawing() BeginDrawing()
#define gui_end_drawing() EndDrawing()

#define gui_key_pressed(key) IsKeyPressed(key)

bool gui_should_close();
void gui_cleanup();

#endif // __LABYRINTHE_GUI_H
