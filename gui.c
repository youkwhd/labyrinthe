#include "gui.h"
#include "direction.h"

void gui_init(int width, int height)
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(width + 20, height + 20, "labyrinthe");
    SetExitKey(KEY_Q);
    SetTargetFPS(60);
}

void gui_draw_grid(int width, int height)
{
    for (int i = 0; i < height; i++) {
        int posy = (i * GUI_CELL_SIZE) + 10;

        for (int j = 0; j < width; j++) {
            int posx = (j * GUI_CELL_SIZE) + 10;

            DrawLine(posx, posy, posx + GUI_CELL_SIZE, posy, RAYWHITE);
            DrawLine(posx, posy, posx, posy + GUI_CELL_SIZE, RAYWHITE);
            DrawLine(posx + GUI_CELL_SIZE, posy, posx + GUI_CELL_SIZE, posy + GUI_CELL_SIZE, RAYWHITE);
            DrawLine(posx, posy + GUI_CELL_SIZE, posx + GUI_CELL_SIZE, posy + GUI_CELL_SIZE, RAYWHITE);
        }
    }
}

void gui_draw_maze_horizontal_corner_border(maze_t *maze, int row, int pos_y)
{
    for (int i = 0; i < maze->width; i++) {
        if (maze->grid[row][i] & DIRECTION_OPENED)
            continue;
        
        int pos_x = (i * GUI_CELL_SIZE) + 10;
        DrawLine(pos_x, pos_y, pos_x + GUI_CELL_SIZE, pos_y, RAYWHITE);
    }
}

void gui_draw_maze_horizontal_border(maze_t *maze, int row, int pos_y)
{
    for (int i = 0; i < maze->width; i++) {
        if (maze->grid[row][i] & DIRECTION_DOWN || (row + 1 < maze->height && maze->grid[row + 1][i] & DIRECTION_UP))
            continue;
        
        int pos_x = (i * GUI_CELL_SIZE) + 10;
        DrawLine(pos_x, pos_y, pos_x + GUI_CELL_SIZE, pos_y, RAYWHITE);
    }
}

void gui_draw_maze_vertical_border(maze_t *maze, int row, int pos_y)
{
    int pos_x = 0 + 10;
    if (!((row >= 1 && row < maze->height - 1) && maze->grid[row][0] & DIRECTION_OPENED)) {
        DrawLine(pos_x, pos_y, pos_x, pos_y + GUI_CELL_SIZE, RAYWHITE);
    }

    for (int i = 0; i < maze->width; i++) {
        bool is_wall_open = (i == maze->width - 1 && (row >= 1 && row < maze->height - 1) && maze->grid[row][i] & DIRECTION_OPENED);
        pos_x += GUI_CELL_SIZE;

        if (maze->grid[row][i] & DIRECTION_RIGHT || (i + 1 < maze->width && maze->grid[row][i + 1] & DIRECTION_LEFT) || is_wall_open) {
            continue;
        } 

        DrawLine(pos_x, pos_y, pos_x, pos_y + GUI_CELL_SIZE, RAYWHITE);
    }
}

void gui_draw_maze(maze_t *maze)
{
    int pos_y = 0 + 10;
    gui_draw_maze_horizontal_corner_border(maze, 0, pos_y);

    for (int i = 0; i < maze->height; i++, pos_y += GUI_CELL_SIZE) {
        gui_draw_maze_vertical_border(maze, i, pos_y);

        if (i != maze->height - 1)
            gui_draw_maze_horizontal_border(maze, i, pos_y + GUI_CELL_SIZE);
    }

    gui_draw_maze_horizontal_corner_border(maze, maze->height - 1, pos_y);
}

void gui_draw_maze_path(maze_t *maze)
{
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            if (maze->grid[i][j] & DIRECTION_TRAVERSED) {
                DrawText("+", (j * GUI_CELL_SIZE) + 15, (i * GUI_CELL_SIZE) + 12, 16, MAROON);
            }
        }
    }
}

void gui_draw_bg()
{
    ClearBackground(BLACK);
}

bool gui_should_close()
{
    return WindowShouldClose();
}

void gui_cleanup()
{
    CloseWindow();
}
