#ifndef __LABYRINTHE_TUI_H
#define __LABYRINTHE_TUI_H

#include "coordinate.h"
#include <stdint.h>

#define MAZE_TUI_CELL_CENTER ' '
#define MAZE_TUI_CELL_BORDER_EMPTY ' '
#define MAZE_TUI_CELL_BORDER_HORIZONTAL '-'
#define MAZE_TUI_CELL_BORDER_VERTICAL '|'
#define MAZE_TUI_CELL_BORDER_CORNER '*'

typedef struct maze_tui_t {
    char **mbuf;
    uint16_t width, height;
} maze_tui_t;

maze_tui_t *maze_tui_init(uint16_t width, uint16_t height);
void maze_tui_set_open_dir(maze_tui_t *tui, coordinate_t coor, direction_t dir);
void maze_tui_open_wall(maze_tui_t *tui, uint16_t actual_width, uint16_t actual_height, coordinate_t coor);
void maze_tui_println(maze_tui_t *tui);
void maze_tui_cleanup(maze_tui_t *tui);

#endif // __LABYRINTHE_TUI_H
