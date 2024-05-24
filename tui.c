#include "tui.h"

#include <stdio.h>
#include <stdlib.h>

void maze_tui_set_open_dir(maze_tui_t *tui, coordinate_t coor, direction_t dir)
{
    if (dir == DIRECTION_BLOCKED) {
        return;
    }

    tui->mbuf[(2 * coor.y + (dir == DIRECTION_DOWN) - (dir == DIRECTION_UP)) + 1][(2 * coor.x + (dir == DIRECTION_RIGHT) - (dir == DIRECTION_LEFT)) + 1] = MAZE_TUI_CELL_BORDER_EMPTY;
}

void maze_tui_generate_upper_border(maze_tui_t *tui, int row)
{
    tui->mbuf[row][0] = MAZE_TUI_CELL_BORDER_CORNER;

    for (uint16_t i = 1; i < tui->width - 1; i += 2) {
        tui->mbuf[row][i] = MAZE_TUI_CELL_BORDER_HORIZONTAL;
        tui->mbuf[row][i + 1] = MAZE_TUI_CELL_BORDER_CORNER;
    }

    tui->mbuf[row][tui->width - 1] = '\0';
}

void maze_tui_generate_middle_border(maze_tui_t *tui, int row)
{
    tui->mbuf[row][0] = MAZE_TUI_CELL_BORDER_VERTICAL;

    for (uint16_t i = 1; i < tui->width - 1; i += 2) {
        tui->mbuf[row][i] = MAZE_TUI_CELL_CENTER;
        tui->mbuf[row][i + 1] = MAZE_TUI_CELL_BORDER_VERTICAL;
    }

    tui->mbuf[row][tui->width - 1] = '\0';
}

void maze_tui_println(maze_tui_t *tui)
{
    for (uint16_t i = 0; i < tui->height; i++) {
        printf("%s\n", tui->mbuf[i]);
    }
}

void maze_tui_generate(maze_tui_t *tui)
{
    maze_tui_generate_upper_border(tui, 0);

    for (uint16_t i = 1; i < tui->height; i += 2) {
        maze_tui_generate_middle_border(tui, i);
        maze_tui_generate_upper_border(tui, i + 1);
    }
}

void maze_tui_open_wall(maze_tui_t *tui, uint16_t actual_width, uint16_t actual_height, coordinate_t coor)
{
    if (coor.y == 0) {
        maze_tui_set_open_dir(tui, coor, DIRECTION_UP);
        return;
    }
    if (coor.y == actual_height - 1) {
        maze_tui_set_open_dir(tui, coor, DIRECTION_DOWN);
        return;
    }
    if (coor.x == 0) {
        maze_tui_set_open_dir(tui, coor, DIRECTION_LEFT);
        return;
    }
    if (coor.x == actual_width - 1) {
        maze_tui_set_open_dir(tui, coor, DIRECTION_RIGHT);
        return;
    }
}


maze_tui_t *maze_tui_init(uint16_t width, uint16_t height)
{
    maze_tui_t *tui = malloc(sizeof *tui);
    tui->width = width * 2 + 1 + 1;
    tui->height = height * 2 + 1;
    tui->mbuf = malloc((sizeof *tui->mbuf) * tui->height);

    for (uint16_t i = 0; i < tui->height; i++) {
        tui->mbuf[i] = malloc((sizeof *tui->mbuf[i]) * tui->width);
    }

    maze_tui_generate(tui);
    return tui;
}

void maze_tui_cleanup(maze_tui_t *tui)
{
    for (uint16_t i = 0; i < tui->height; i++) {
        free(tui->mbuf[i]);
    }

    free(tui->mbuf);
    free(tui);
}
