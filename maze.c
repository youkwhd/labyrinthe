#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "maze.h"
#include "stack.h"

void maze_node_printc(maze_node_t node)
{
    if (node == DIRECTION_NONE) {
        printf("0 ");
        return;
    }
    if (node == DIRECTION_BLOCKED) {
        printf("B ");
        return;
    }
    if (node == -1) {
        printf("! ");
        return;
    }

    if (node & DIRECTION_UP) {
        printf("U");
    }
    if (node & DIRECTION_RIGHT) {
        printf("R");
    }
    if (node & DIRECTION_DOWN) {
        printf("D");
    }
    if (node & DIRECTION_LEFT) {
        printf("L");
    }

    printf(" ");
}

void maze_node_println(maze_node_t node)
{
    if (node == DIRECTION_NONE) {
        puts("DIRECTION_NONE");
        return;
    }
    if (node == DIRECTION_BLOCKED) {
        puts("DIRECTION_BLOCKED");
        return;
    }
    if (node == -1) {
        puts("__DIRECTION_INVALID");
        return;
    }

    if (node & DIRECTION_UP) {
        puts("DIRECTION_UP");
    }
    if (node & DIRECTION_RIGHT) {
        puts("DIRECTION_RIGHT");
    }
    if (node & DIRECTION_DOWN) {
        puts("DIRECTION_DOWN");
    }
    if (node & DIRECTION_LEFT) {
        puts("DIRECTION_LEFT");
    }
}

void maze_println(maze_t *maze)
{
    printf("\n");
    for (int i = 0; i < maze->width; i++) {
        for (int j = 0; j < maze->height; j++) {
            maze_node_printc(maze->body[i][j]);
        }

        printf("\n");
    }
}

void maze_set_node_dir(maze_t *maze, coordinate_t coor, maze_node_t dir)
{
    maze->body[coor.y][coor.x] |= dir;
}

maze_node_t maze_get(maze_t *maze, coordinate_t coor)
{
    return maze->body[coor.y][coor.x];
}

void maze_get_neighbors(maze_t *maze, coordinate_t coor, maze_node_t *neighbors)
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

void maze_get_available_neighbors(maze_t *maze, coordinate_t coor, maze_node_t neighbors[4], size_t *neighbors_len)
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

void maze_init(maze_t *maze, uint16_t height, uint16_t width)
{
    maze->body = malloc(sizeof(*maze->body) * width);
    for (size_t i = 0; i < width; i++) {
        maze->body[i] = malloc(sizeof(*maze->body[i]) * height);

        for (size_t j = 0; j < height; j++) {
            maze->body[i][j] = DIRECTION_NONE;
        }
    }

    maze->height = height;
    maze->width = width;
}

void maze_generate(maze_t *maze, coordinate_t start)
{
    srand(time(NULL));

    coordinate_t cur_coor = start;

    stack_t stack;
    stack_init(&stack, maze->height * maze->width);

    maze_node_t __avail_neighbors[4] = {DIRECTION_NONE};
    size_t __avail_neighbors_len = 0;
    maze_get_available_neighbors(maze, cur_coor, __avail_neighbors, &__avail_neighbors_len);

    maze_node_t __dir = __avail_neighbors[rand() % __avail_neighbors_len];
    maze_set_node_dir(maze, cur_coor, __dir);
    stack_push(&stack, cur_coor);

    cur_coor.y -= __dir == DIRECTION_UP;
    cur_coor.x += __dir == DIRECTION_RIGHT;
    cur_coor.y += __dir == DIRECTION_DOWN;
    cur_coor.x -= __dir == DIRECTION_LEFT;

    while (!stack_is_empty(&stack)) {
        maze_node_t avail_neighbors[4] = {DIRECTION_NONE};
        size_t avail_neighbors_len = 0;
        maze_get_available_neighbors(maze, cur_coor, avail_neighbors, &avail_neighbors_len);

        if (avail_neighbors_len == 0) {
            if (maze_get(maze, cur_coor) == DIRECTION_NONE) {
                maze_set_node_dir(maze, cur_coor, DIRECTION_BLOCKED);
            }

            cur_coor = stack_pop(&stack);
            continue;
        }

        maze_node_t dir = avail_neighbors[rand() % avail_neighbors_len];
        maze_set_node_dir(maze, cur_coor, dir);
        stack_push(&stack, cur_coor);

        cur_coor.y -= dir == DIRECTION_UP;
        cur_coor.x += dir == DIRECTION_RIGHT;
        cur_coor.y += dir == DIRECTION_DOWN;
        cur_coor.x -= dir == DIRECTION_LEFT;
    }

    maze_println(maze);
    stack_cleanup(&stack);
}

void maze_cleanup(maze_t *maze)
{
    for (size_t i = 0; i < maze->width; i++) {
        free(maze->body[i]);
    }

    free(maze->body);
}
