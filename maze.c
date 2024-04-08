#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "maze.h"
#include "stack.h"

void maze_cell_printc(maze_cell_t cell)
{
    if (cell == DIRECTION_NONE) {
        printf("0 ");
        return;
    }
    if (cell == DIRECTION_BLOCKED) {
        printf("B ");
        return;
    }
    if (cell == -1) {
        printf("! ");
        return;
    }

    if (cell & DIRECTION_UP) {
        printf("U");
    }
    if (cell & DIRECTION_RIGHT) {
        printf("R");
    }
    if (cell & DIRECTION_DOWN) {
        printf("D");
    }
    if (cell & DIRECTION_LEFT) {
        printf("L");
    }

    printf(" ");
}

void maze_println(maze_t *maze)
{
    printf("\n");
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze_cell_printc(maze->body[i][j]);
        }

        printf("\n");
    }
}

void __maze_ui_set_open_dir(maze_t *maze, coordinate_t coor, maze_cell_t dir)
{
    if (dir == DIRECTION_BLOCKED) {
        return;
    }

    maze->__maze_ui[(2 * coor.y + (dir == DIRECTION_DOWN) - (dir == DIRECTION_UP)) + 1][(2 * coor.x + (dir == DIRECTION_RIGHT) - (dir == DIRECTION_LEFT)) + 1] = ' ';
}

void maze_set_cell_dir(maze_t *maze, coordinate_t coor, maze_cell_t dir)
{
    maze->body[coor.y][coor.x] |= dir;
    __maze_ui_set_open_dir(maze, coor, dir);
}

maze_cell_t maze_get(maze_t *maze, coordinate_t coor)
{
    return maze->body[coor.y][coor.x];
}

void maze_get_neighbors(maze_t *maze, coordinate_t coor, maze_cell_t *neighbors)
{
    if ((int)coor.y - 1 >= 0) {
        neighbors[0] = maze->body[coor.y - 1][coor.x];
    }

    if ((int)coor.x + 1 < maze->width) {
        neighbors[1] = maze->body[coor.y][coor.x + 1];
    }

    if ((int)coor.y + 1 < maze->height) {
        neighbors[2] = maze->body[coor.y + 1][coor.x];
    }

    if ((int)coor.x - 1 >= 0) {
        neighbors[3] = maze->body[coor.y][coor.x - 1];
    }
}

void maze_get_available_neighbors(maze_t *maze, coordinate_t coor, maze_cell_t neighbors[4], size_t *neighbors_len)
{
    *neighbors_len = 0;

    if (((int)coor.y - 1 >= 0) && maze->body[coor.y - 1][coor.x] == DIRECTION_NONE) {
        neighbors[(*neighbors_len)++] = DIRECTION_UP;
    }

    if (((int)coor.x + 1 < maze->width) && maze->body[coor.y][coor.x + 1] == DIRECTION_NONE) {
        neighbors[(*neighbors_len)++] = DIRECTION_RIGHT;
    }

    if (((int)coor.y + 1 < maze->height) && maze->body[coor.y + 1][coor.x] == DIRECTION_NONE) {
        neighbors[(*neighbors_len)++] = DIRECTION_DOWN;
    }

    if (((int)coor.x - 1 >= 0) && maze->body[coor.y][coor.x - 1] == DIRECTION_NONE) {
        neighbors[(*neighbors_len)++] = DIRECTION_LEFT;
    }
}

void __maze_gen_ui_upper_border(maze_t *maze, char *str)
{
    str[0] = '*';

    for (size_t i = 1; i < maze->__maze_ui_width - 1; i += 2) {
        str[i] = '-';
        str[i + 1] = '*';
    }

    str[maze->__maze_ui_width - 1] = '\0';
}

void __maze_gen_ui_middle_border(maze_t *maze, char *str)
{
    str[0] = '|';

    for (size_t i = 1; i < maze->__maze_ui_width - 1; i += 2) {
        str[i] = ' ';
        str[i + 1] = '|';
    }

    str[maze->__maze_ui_width - 1] = '\0';
}

void __maze_ui_println(maze_t *maze)
{
    for (size_t i = 0; i < maze->__maze_ui_height; i++) {
        printf("%s\n", maze->__maze_ui[i]);
    }
}

void __maze_gen_ui(maze_t *maze)
{
    __maze_gen_ui_upper_border(maze, maze->__maze_ui[0]);

    for (size_t i = 1; i < maze->__maze_ui_height; i += 2) {
        __maze_gen_ui_middle_border(maze, maze->__maze_ui[i]);
        __maze_gen_ui_upper_border(maze, maze->__maze_ui[i + 1]);
    }
}

void maze_init(maze_t *maze, uint16_t width, uint16_t height)
{
    maze->body = malloc(sizeof(*maze->body) * height);
    for (size_t i = 0; i < height; i++) {
        maze->body[i] = malloc(sizeof(*maze->body[i]) * width);

        for (size_t j = 0; j < width; j++) {
            maze->body[i][j] = DIRECTION_NONE;
        }
    }

    maze->height = height;
    maze->width = width;

    /* TODO: explain the pluses
     */
    maze->__maze_ui_height = maze->height * 2 + 1;
    maze->__maze_ui_width = maze->width * 2 + 1 + 1;

    maze->__maze_ui = malloc((sizeof *maze->__maze_ui) * maze->__maze_ui_height);
    for (size_t i = 0; i < maze->__maze_ui_height; i++) {
        maze->__maze_ui[i] = malloc((sizeof *maze->__maze_ui) * maze->__maze_ui_width);
    }

    __maze_gen_ui(maze);
}

coordinate_t maze_set_starting_point(maze_t *maze, coordinate_t start)
{
    maze_cell_t avail_neighbors[4] = {DIRECTION_NONE};
    size_t avail_neighbors_len = 0;
    maze_get_available_neighbors(maze, start, avail_neighbors, &avail_neighbors_len);

    maze_cell_t dir = avail_neighbors[rand() % avail_neighbors_len];
    maze_set_cell_dir(maze, start, dir);

    coordinate_move_to(&start, dir);
    return start;
}

void __maze_ui_open_wall(maze_t *maze, coordinate_t coor)
{
    if (coor.y == 0) {
        __maze_ui_set_open_dir(maze, coor, DIRECTION_UP);
        return;
    }
    if (coor.y == maze->height - 1) {
        __maze_ui_set_open_dir(maze, coor, DIRECTION_DOWN);
        return;
    }
    if (coor.x == 0) {
        __maze_ui_set_open_dir(maze, coor, DIRECTION_LEFT);
        return;
    }
    if (coor.x == maze->width - 1) {
        __maze_ui_set_open_dir(maze, coor, DIRECTION_RIGHT);
        return;
    }
}

void maze_get_dead_ends(maze_t *maze, coordinate_t **dead_ends, size_t *dead_ends_length)
{
    if (maze->height <= 0 || maze->width <= 0)
        return;

#define __DEAD_END_ARR_INITIAL_LEN 5
    *dead_ends_length = 0;
    size_t __dead_ends_length = __DEAD_END_ARR_INITIAL_LEN;
    *dead_ends = malloc(sizeof(**dead_ends) * __dead_ends_length);

    for (int i = 1; i < maze->width - 1; i++) {
        if (maze->body[0][i] == DIRECTION_BLOCKED) {
            if (*dead_ends_length >= __dead_ends_length) {
                __dead_ends_length += __DEAD_END_ARR_INITIAL_LEN;
                *dead_ends = realloc(*dead_ends, __dead_ends_length);
            }

            (*dead_ends)[(*dead_ends_length)++] = (coordinate_t){i, 0};
        }
    }

    for (int i = 1; i < maze->width - 1; i++) {
        if (maze->body[maze->height - 1][i] == DIRECTION_BLOCKED) {
            if (*dead_ends_length >= __dead_ends_length) {
                __dead_ends_length += __DEAD_END_ARR_INITIAL_LEN;
                *dead_ends = realloc(*dead_ends, __dead_ends_length);
            }

            (*dead_ends)[(*dead_ends_length)++] = (coordinate_t){i, maze->height - 1};
        }
    }

    for (int i = 0; i < maze->height; i++) {
        if (maze->body[i][0] == DIRECTION_BLOCKED) {
            if (*dead_ends_length >= __dead_ends_length) {
                __dead_ends_length += __DEAD_END_ARR_INITIAL_LEN;
                *dead_ends = realloc(*dead_ends, __dead_ends_length);
            }

            (*dead_ends)[(*dead_ends_length)++] = (coordinate_t){0, i};
        }
    }

    for (int i = 0; i < maze->height; i++) {
        if (maze->body[i][maze->width - 1] == DIRECTION_BLOCKED) {
            if (*dead_ends_length >= __dead_ends_length) {
                __dead_ends_length += __DEAD_END_ARR_INITIAL_LEN;
                *dead_ends = realloc(*dead_ends, __dead_ends_length);
            }

            (*dead_ends)[(*dead_ends_length)++] = (coordinate_t){maze->width - 1, i};
        }
    }
}

void maze_generate(maze_t *maze, coordinate_t start)
{
    srand(time(NULL));

    stack_t stack;
    stack_init(&stack, maze->width * maze->height);

    coordinate_t cur_coor = maze_set_starting_point(maze, start);
    __maze_ui_open_wall(maze, start);
    stack_push(&stack, start);

    while (!stack_is_empty(&stack)) {
        maze_cell_t avail_neighbors[4] = {DIRECTION_NONE};
        size_t avail_neighbors_len = 0;
        maze_get_available_neighbors(maze, cur_coor, avail_neighbors, &avail_neighbors_len);

        if (avail_neighbors_len == 0) {
            if (maze_get(maze, cur_coor) == DIRECTION_NONE) {
                maze_set_cell_dir(maze, cur_coor, DIRECTION_BLOCKED);
            }

            cur_coor = stack_pop(&stack);
            continue;
        }

        maze_cell_t dir = avail_neighbors[rand() % avail_neighbors_len];
        maze_set_cell_dir(maze, cur_coor, dir);
        stack_push(&stack, cur_coor);
        coordinate_move_to(&cur_coor, dir);
    }

    coordinate_t *dead_ends = NULL;
    size_t dead_ends_length = 0;
    maze_get_dead_ends(maze, &dead_ends, &dead_ends_length);

    coordinate_t finish = dead_ends[rand() % dead_ends_length];
    __maze_ui_open_wall(maze, finish);

    free(dead_ends);
    stack_cleanup(&stack);
}

void maze_cleanup(maze_t *maze)
{
    for (size_t i = 0; i < maze->height; i++) {
        free(maze->body[i]);
    }

    free(maze->body);

    for (size_t i = 0; i < maze->__maze_ui_height; i++) {
        free(maze->__maze_ui[i]);
    }

    free(maze->__maze_ui);
}
