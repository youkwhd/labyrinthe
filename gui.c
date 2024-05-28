#include "gui.h"
#include "direction.h"

#define CELL_SIZE 20

void gui_init()
{
    InitWindow(GUI_WIN_WIDTH, GUI_WIN_HEIGHT, "labyrinthe");
    SetExitKey(KEY_Q);
    SetTargetFPS(60);
}

void gui_update()
{
}

void gui_draw_grid(int width, int height)
{
    for (int i = 0; i < height; i++) {
        int posy = (i * CELL_SIZE) + 10;

        for (int j = 0; j < width; j++) {
            int posx = (j * CELL_SIZE) + 10;

            DrawLine(posx, posy, posx + CELL_SIZE, posy, RAYWHITE);
            DrawLine(posx, posy, posx, posy + CELL_SIZE, RAYWHITE);
            DrawLine(posx + CELL_SIZE, posy, posx + CELL_SIZE, posy + CELL_SIZE, RAYWHITE);
            DrawLine(posx, posy + CELL_SIZE, posx + CELL_SIZE, posy + CELL_SIZE, RAYWHITE);
        }
    }
}

void gui_draw_maze_horizontal_corner_border(maze_t *maze, int row, int pos_y)
{
    for (int i = 0; i < maze->width; i++) {
        if (maze->grid[row][i] & DIRECTION_OPENED)
            continue;
        
        int pos_x = (i * CELL_SIZE) + 10;
        DrawLine(pos_x, pos_y, pos_x + CELL_SIZE, pos_y, RAYWHITE);
    }
}

void gui_draw_horizontal_border(maze_t *maze, int row, int pos_y)
{
    for (int i = 0; i < maze->width; i++) {
        if (maze->grid[row][i] & DIRECTION_DOWN || (row + 1 < maze->height && maze->grid[row + 1][i] & DIRECTION_UP))
            continue;
        
        int pos_x = (i * CELL_SIZE) + 10;
        DrawLine(pos_x, pos_y, pos_x + CELL_SIZE, pos_y, RAYWHITE);
    }
}

void gui_draw_vertical_border(maze_t *maze, int row, int pos_y)
{
    int pos_x = 0 + 10;
    if (!((row >= 1 && row < maze->height - 1) && maze->grid[row][0] & DIRECTION_OPENED)) {
        DrawLine(pos_x, pos_y, pos_x, pos_y + CELL_SIZE, RAYWHITE);
    }

    for (int i = 0; i < maze->width; i++) {
        bool is_wall_open = (i == maze->width - 1 && (row >= 1 && row < maze->height - 1) && maze->grid[row][i] & DIRECTION_OPENED);
        pos_x += CELL_SIZE;

        if (maze->grid[row][i] & DIRECTION_RIGHT || (i + 1 < maze->width && maze->grid[row][i + 1] & DIRECTION_LEFT) || is_wall_open) {
            continue;
        } 

        DrawLine(pos_x, pos_y, pos_x, pos_y + CELL_SIZE, RAYWHITE);
    }
}

void gui_draw_maze(maze_t *maze)
{
    int pos_y = 0 + 10;
    gui_draw_maze_horizontal_corner_border(maze, 0, pos_y);

    for (int i = 0; i < maze->height; i++, pos_y += CELL_SIZE) {
        gui_draw_vertical_border(maze, i, pos_y);

        if (i != maze->height - 1)
            gui_draw_horizontal_border(maze, i, pos_y + CELL_SIZE);
    }

    gui_draw_maze_horizontal_corner_border(maze, maze->height - 1, pos_y);
}

void gui_draw(maze_t *maze)
{
    BeginDrawing();
    ClearBackground(BLACK);

    /* gui_draw_grid((GUI_WIN_WIDTH - 20) / CELL_SIZE, (GUI_WIN_HEIGHT - 20) / CELL_SIZE); */
    /* gui_draw_grid(8, 2); */
    gui_draw_maze(maze);

    EndDrawing();
}

void gui_run(maze_t *maze)
{
    while (!WindowShouldClose()) {
        gui_update();
        gui_draw(maze);
    }
}

void gui_cleanup()
{
    CloseWindow();
}
