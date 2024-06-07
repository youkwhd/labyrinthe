#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "labyrinthe.h"
#include "maze.h"
#include "queue.h"
#include "stack.h"

void maze_reset_traversed(maze_t *maze)
{
    for (uint16_t i = 0; i < maze->height; i++) {
        for (uint16_t j = 0; j < maze->width; j++) {
            maze->grid[i][j] &= ~DIRECTION_TRAVERSED;
        }
    }
}

void maze_println_horizontal_corner_border(maze_t *maze, int row)
{
    printf("+");
    for (int i = 0; i < maze->width; i++) {
        if (maze->grid[row][i] & DIRECTION_OPENED) {
            printf("  ");
        } else {
            printf("--");
        }

        printf("+");
    }
    printf("\n");
}

void maze_println_horizontal_border(maze_t *maze, int row)
{
    printf("+");
    for (int i = 0; i < maze->width; i++) {
        if (maze->grid[row][i] & DIRECTION_DOWN || (row + 1 < maze->height && maze->grid[row + 1][i] & DIRECTION_UP)) {
            printf("  ");
        } else {
            printf("--");
        }

        printf("+");
    }
    printf("\n");
}

void maze_println_vertical_border(maze_t *maze, int row)
{
    if ((row >= 1 && row < maze->height - 1) && maze->grid[row][0] & DIRECTION_OPENED) {
        printf(" ");
    } else {
        printf("|");
    }

    for (int i = 0; i < maze->width; i++) {
        bool is_wall_open = (i == maze->width - 1 && (row >= 1 && row < maze->height - 1) && maze->grid[row][i] & DIRECTION_OPENED);

        printf(maze->grid[row][i] & DIRECTION_TRAVERSED ? "<>" : "  ");

        if (maze->grid[row][i] & DIRECTION_RIGHT || (i + 1 < maze->width && maze->grid[row][i + 1] & DIRECTION_LEFT) || is_wall_open) {
            printf(" ");
        } else {
            printf("|");
        }
    }
    printf("\n");
}

void maze_println(maze_t *maze)
{
    maze_println_horizontal_corner_border(maze, 0);

    for (int i = 0; i < maze->height; i++) {
        maze_println_vertical_border(maze, i);

        if (i != maze->height - 1)
            maze_println_horizontal_border(maze, i);
    }

    maze_println_horizontal_corner_border(maze, maze->height - 1);
}

void maze_set_cell_dir(maze_t *maze, coordinate_t coor, maze_cell_t dir)
{
    maze->grid[coor.y][coor.x] |= dir;
}

maze_cell_t maze_get(maze_t *maze, coordinate_t coor)
{
    return maze->grid[coor.y][coor.x];
}

void maze_get_neighbors(maze_t *maze, coordinate_t coor, maze_cell_t *neighbors)
{
    if ((int)coor.y - 1 >= 0) {
        neighbors[0] = maze->grid[coor.y - 1][coor.x];
    }

    if ((int)coor.x + 1 < maze->width) {
        neighbors[1] = maze->grid[coor.y][coor.x + 1];
    }

    if ((int)coor.y + 1 < maze->height) {
        neighbors[2] = maze->grid[coor.y + 1][coor.x];
    }

    if ((int)coor.x - 1 >= 0) {
        neighbors[3] = maze->grid[coor.y][coor.x - 1];
    }
}

void maze_get_untraversed_neighbors(maze_t *maze, coordinate_t coor, maze_cell_t neighbors[4], size_t *neighbors_len)
{
    *neighbors_len = 0;

    if ((maze->grid[coor.y][coor.x] & DIRECTION_UP) && !(maze->grid[coor.y - 1][coor.x] & DIRECTION_TRAVERSED)) {
        neighbors[(*neighbors_len)++] = DIRECTION_UP;
    }

    if ((maze->grid[coor.y][coor.x] & DIRECTION_RIGHT) && !(maze->grid[coor.y][coor.x + 1] & DIRECTION_TRAVERSED)) {
        neighbors[(*neighbors_len)++] = DIRECTION_RIGHT;
    }

    if ((maze->grid[coor.y][coor.x] & DIRECTION_DOWN) && !(maze->grid[coor.y + 1][coor.x] & DIRECTION_TRAVERSED)) {
        neighbors[(*neighbors_len)++] = DIRECTION_DOWN;
    }

    if ((maze->grid[coor.y][coor.x] & DIRECTION_LEFT) && !(maze->grid[coor.y][coor.x - 1] & DIRECTION_TRAVERSED)) {
        neighbors[(*neighbors_len)++] = DIRECTION_LEFT;
    }
}

void maze_get_available_neighbors(maze_t *maze, coordinate_t coor, maze_cell_t neighbors[4], size_t *neighbors_len)
{
    *neighbors_len = 0;

    if (((int)coor.y - 1 >= 0) && maze->grid[coor.y - 1][coor.x] == DIRECTION_NONE) {
        neighbors[(*neighbors_len)++] = DIRECTION_UP;
    }

    if (((int)coor.x + 1 < maze->width) && maze->grid[coor.y][coor.x + 1] == DIRECTION_NONE) {
        neighbors[(*neighbors_len)++] = DIRECTION_RIGHT;
    }

    if (((int)coor.y + 1 < maze->height) && maze->grid[coor.y + 1][coor.x] == DIRECTION_NONE) {
        neighbors[(*neighbors_len)++] = DIRECTION_DOWN;
    }

    if (((int)coor.x - 1 >= 0) && maze->grid[coor.y][coor.x - 1] == DIRECTION_NONE) {
        neighbors[(*neighbors_len)++] = DIRECTION_LEFT;
    }
}

void maze_init(maze_t *maze, uint16_t width, uint16_t height)
{
    maze->width = width;
    maze->height = height;
    maze->grid = malloc(sizeof(*maze->grid) * height);

    for (size_t i = 0; i < height; i++) {
        maze->grid[i] = malloc(sizeof(*maze->grid[i]) * width);

        for (size_t j = 0; j < width; j++) {
            maze->grid[i][j] = DIRECTION_NONE;
        }
    }
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

void maze_get_dead_ends(maze_t *maze, coordinate_t **dead_ends, size_t *dead_ends_length)
{
    if (maze->height <= 0 || maze->width <= 0)
        return;

/* TODO:
 * dynamic array is really painful
 */
#define __DEAD_END_ARR_INITIAL_LEN 5
    *dead_ends_length = 0;
    size_t __dead_ends_length = __DEAD_END_ARR_INITIAL_LEN;
    *dead_ends = malloc(sizeof(**dead_ends) * __dead_ends_length);

    for (int i = 1; i < maze->width - 1; i++) {
        if (maze_get(maze, (coordinate_t){i, 0}) == DIRECTION_BLOCKED) {
            if (*dead_ends_length >= __dead_ends_length) {
                __dead_ends_length += __DEAD_END_ARR_INITIAL_LEN;
                *dead_ends = realloc(*dead_ends, sizeof(**dead_ends) * __dead_ends_length);
            }

            (*dead_ends)[(*dead_ends_length)++] = (coordinate_t){i, 0};
        }

        if (maze_get(maze, (coordinate_t){i, maze->height - 1}) == DIRECTION_BLOCKED) {
            if (*dead_ends_length >= __dead_ends_length) {
                __dead_ends_length += __DEAD_END_ARR_INITIAL_LEN;
                *dead_ends = realloc(*dead_ends, sizeof(**dead_ends) * __dead_ends_length);
            }

            (*dead_ends)[(*dead_ends_length)++] = (coordinate_t){i, maze->height - 1};
        }
    }

    for (int i = 0; i < maze->height; i++) {
        if (maze_get(maze, (coordinate_t){0, i}) == DIRECTION_BLOCKED) {
            if (*dead_ends_length >= __dead_ends_length) {
                __dead_ends_length += __DEAD_END_ARR_INITIAL_LEN;
                *dead_ends = realloc(*dead_ends, sizeof(**dead_ends) * __dead_ends_length);
            }

            (*dead_ends)[(*dead_ends_length)++] = (coordinate_t){0, i};
        }

        if (maze_get(maze, (coordinate_t){maze->width - 1, i}) == DIRECTION_BLOCKED) {
            if (*dead_ends_length >= __dead_ends_length) {
                __dead_ends_length += __DEAD_END_ARR_INITIAL_LEN;
                *dead_ends = realloc(*dead_ends, sizeof(**dead_ends) * __dead_ends_length);
            }

            (*dead_ends)[(*dead_ends_length)++] = (coordinate_t){maze->width - 1, i};
        }
    }

    if (*dead_ends_length != 0)
        return;

    /* No dead end on the corner found, we find for any.
     */
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            if (!(maze_get(maze, (coordinate_t){ j, i }) & DIRECTION_BLOCKED)) {
                continue;
            }

            if (*dead_ends_length >= __dead_ends_length) {
                __dead_ends_length += __DEAD_END_ARR_INITIAL_LEN;
                *dead_ends = realloc(*dead_ends, sizeof(**dead_ends) * __dead_ends_length);
            }

            (*dead_ends)[(*dead_ends_length)++] = (coordinate_t){j, i};
        }
    }
}

void maze_reset(maze_t *maze)
{
    for (uint16_t i = 0; i < maze->height; i++) {
        for (uint16_t j = 0; j < maze->width; j++) {
            maze->grid[i][j] = DIRECTION_NONE;
        }
    }
}

coordinate_t maze_generate(maze_t *maze, coordinate_t start)
{
    srand(time(NULL));

    stack_t stack;
    stack_init(&stack, maze->width * maze->height);

    coordinate_t cur_coor = maze_set_starting_point(maze, start);
    maze_set_cell_dir(maze, start, DIRECTION_OPENED);
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

    /* TODO: need to resolve this
     */
    if (dead_ends_length == 0) {
        fprintf(stderr, "labyrinthe: no dead end found. (rare case)\n");
        exit(1);
    }

    coordinate_t finish = dead_ends[rand() % dead_ends_length];
    maze_set_cell_dir(maze, finish, DIRECTION_OPENED);

    free(dead_ends);
    stack_cleanup(&stack);
    return finish;
}

void maze_solve_best_first_search(maze_t *maze, coordinate_t start, coordinate_t end)
{
    stack_t stack;
    stack_init(&stack, maze->width * maze->height);

    coordinate_t cur_coor = start;
    while (!coordinate_equal(cur_coor, end)) {
        maze_cell_t neighbors[4] = {DIRECTION_NONE};
        size_t neighbors_len = 0;
        maze_get_untraversed_neighbors(maze, cur_coor, neighbors, &neighbors_len);

        if (neighbors_len == 0) {
            maze_set_cell_dir(maze, cur_coor, DIRECTION_TRAVERSED);
            cur_coor = stack_pop(&stack);
            continue;
        }

        size_t selected = 0;
        for (size_t i = 1; i < neighbors_len; i++) {
            coordinate_t coor = cur_coor;
            coordinate_move_to(&coor, neighbors[i]);

            coordinate_t min_coor = cur_coor;
            coordinate_move_to(&min_coor, neighbors[selected]);

            int coor_distance = coordinate_distance(coor, end);
            int min_coor_distance = coordinate_distance(min_coor, end);

            if (MIN(coor_distance, min_coor_distance) == coor_distance) {
                selected = i;
            }
        }

        stack_push(&stack, cur_coor);
        maze_set_cell_dir(maze, cur_coor, DIRECTION_TRAVERSED);
        coordinate_move_to(&cur_coor, neighbors[selected]);
    }

    stack_push(&stack, cur_coor);
    maze_set_cell_dir(maze, cur_coor, DIRECTION_TRAVERSED);

    /* TODO: this is correct but the closure
     * should not be within this function
     */
    maze_reset_traversed(maze);
    while (!stack_is_empty(&stack))
        maze_set_cell_dir(maze, stack_pop(&stack), DIRECTION_TRAVERSED);

    stack_cleanup(&stack);
}

void maze_solve_breadth_first_search(maze_t *maze, coordinate_t start, coordinate_t end)
{
    queue_t queue;
    queue_init(&queue, maze->width * maze->height);

    stack_t path;
    stack_init(&path, maze->width * maze->height);
    stack_push(&path, start);

    queue_enqueue(&queue, path);

    while (!queue_is_empty(&queue)) {
        stack_t __path = queue_dequeue(&queue);
        coordinate_t coor = stack_top(&__path);
        maze_set_cell_dir(maze, coor, DIRECTION_TRAVERSED);

        if (coordinate_equal(coor, end)) {
            path = __path;
            break;
        }

        maze_cell_t neighbors[4] = {DIRECTION_NONE};
        size_t neighbors_len = 0;
        maze_get_untraversed_neighbors(maze, coor, neighbors, &neighbors_len);

        for (size_t i = 0; i < neighbors_len; i++) {
            coordinate_t neighbor_coor = coor;
            coordinate_move_to(&neighbor_coor, neighbors[i]);

            stack_t npath;
            stack_copy(&npath, &__path);

            stack_push(&npath, neighbor_coor);
            queue_enqueue(&queue, npath);
        }

        stack_cleanup(&__path);
    }

    /* TODO: this is correct but the closure
     * should not be within this function
     *
     * also, this assumes the solution is
     * always present.
     */
    maze_reset_traversed(maze);
    while (!stack_is_empty(&path))
        maze_set_cell_dir(maze, stack_pop(&path), DIRECTION_TRAVERSED);

    stack_cleanup(&path);


    /* TODO: refactor queue to keep
     * char *p as it's array item
     */
    while (!queue_is_empty(&queue)) {
        stack_t __path = queue_dequeue(&queue);
        stack_cleanup(&__path);
    }

    queue_cleanup(&queue);
}

void maze_solve_depth_first_search(maze_t *maze, coordinate_t start, coordinate_t end)
{
    stack_t stack;
    stack_init(&stack, maze->width * maze->height);

    coordinate_t cur_coor = start;
    while (!coordinate_equal(cur_coor, end)) {
        maze_cell_t neighbors[4] = {DIRECTION_NONE};
        size_t neighbors_len = 0;
        maze_get_untraversed_neighbors(maze, cur_coor, neighbors, &neighbors_len);

        if (neighbors_len == 0) {
            maze_set_cell_dir(maze, cur_coor, DIRECTION_TRAVERSED);
            cur_coor = stack_pop(&stack);
            continue;
        }

        stack_push(&stack, cur_coor);
        maze_set_cell_dir(maze, cur_coor, DIRECTION_TRAVERSED);
        coordinate_move_to(&cur_coor, neighbors[rand() % neighbors_len]);
    }

    stack_push(&stack, cur_coor);

    /* TODO: this is correct but the closure
     * should not be within this function
     */
    maze_reset_traversed(maze);
    while (!stack_is_empty(&stack))
        maze_set_cell_dir(maze, stack_pop(&stack), DIRECTION_TRAVERSED);

    stack_cleanup(&stack);
}

void maze_solve(maze_t *maze, coordinate_t start, coordinate_t end, maze_solving_strategy_t strat)
{
    switch (strat) {
    case MAZE_SOLVING_STRATEGY_BEST_FIRST_SEARCH:
        maze_solve_best_first_search(maze, start, end);
        return;
    case MAZE_SOLVING_STRATEGY_BREADTH_FIRST_SEARCH:
        maze_solve_breadth_first_search(maze, start, end);
        return;
    case MAZE_SOLVING_STRATEGY_DEPTH_FIRST_SEARCH:
        maze_solve_depth_first_search(maze, start, end);
        return;
    }
}

void maze_cleanup(maze_t *maze)
{
    for (size_t i = 0; i < maze->height; i++) {
        free(maze->grid[i]);
    }

    free(maze->grid);
}
