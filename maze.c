#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "maze.h"
#include "coordinate.h"
#include "stack.h"

void maze_node_printc(maze_node_t node)
{
    if (node == MAZE_NODE_DIR_NONE) {
        printf("0 ");
        return;
    }
    if (node == MAZE_NODE_DIR_BLOCKED) {
        printf("B ");
        return;
    }
    if (node == -1) {
        printf("! ");
        return;
    }

    if (node & MAZE_NODE_DIR_UP) {
        printf("U");
    }
    if (node & MAZE_NODE_DIR_RIGHT) {
        printf("R");
    }
    if (node & MAZE_NODE_DIR_DOWN) {
        printf("D");
    }
    if (node & MAZE_NODE_DIR_LEFT) {
        printf("L");
    }

    printf(" ");
}

void maze_node_println(maze_node_t node)
{
    if (node == MAZE_NODE_DIR_NONE) {
        puts("MAZE_NODE_DIR_NONE");
        return;
    }
    if (node == MAZE_NODE_DIR_BLOCKED) {
        puts("MAZE_NODE_DIR_BLOCKED");
        return;
    }
    if (node == -1) {
        puts("__MAZE_NODE_DIR_INVALID");
        return;
    }

    if (node & MAZE_NODE_DIR_UP) {
        puts("MAZE_NODE_DIR_UP");
    }
    if (node & MAZE_NODE_DIR_RIGHT) {
        puts("MAZE_NODE_DIR_RIGHT");
    }
    if (node & MAZE_NODE_DIR_DOWN) {
        puts("MAZE_NODE_DIR_DOWN");
    }
    if (node & MAZE_NODE_DIR_LEFT) {
        puts("MAZE_NODE_DIR_LEFT");
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

    if (((int)coor.y - 1 >= 0) && maze->body[coor.y - 1][coor.x] == MAZE_NODE_DIR_NONE) {
        neighbors[(*neighbors_len)++] = 1 << (0 + 2);
    }

    if (((int)coor.x + 1 < maze->width) && maze->body[coor.y][coor.x + 1] == MAZE_NODE_DIR_NONE) {
        neighbors[(*neighbors_len)++] = 1 << (1 + 2);
    }

    if (((int)coor.y + 1 < maze->height) && maze->body[coor.y + 1][coor.x] == MAZE_NODE_DIR_NONE) {
        neighbors[(*neighbors_len)++] = 1 << (2 + 2);
    }

    if (((int)coor.x - 1 >= 0) && maze->body[coor.y][coor.x - 1] == MAZE_NODE_DIR_NONE) {
        neighbors[(*neighbors_len)++] = 1 << (3 + 2);
    }
}

void maze_init(maze_t *maze, uint16_t height, uint16_t width)
{
    maze->body = malloc(sizeof(*maze->body) * width);
    for (size_t i = 0; i < width; i++) {
        maze->body[i] = malloc(sizeof(*maze->body[i]) * height);

        for (size_t j = 0; j < height; j++) {
            maze->body[i][j] = MAZE_NODE_DIR_NONE;
        }
    }

    maze->height = height;
    maze->width = width;
}

void maze_generate(maze_t *maze)
{
    srand(time(NULL));

    stack_t stack;
    stack_init(&stack, maze->height * maze->width);

    coordinate_t *cur_coor = coordinate_init(0, 0);
    maze_set_node_dir(maze, *cur_coor, MAZE_NODE_DIR_RIGHT);
    stack_push(&stack, *cur_coor);
    cur_coor->x++;

    for (; !stack_is_empty(&stack); ) {
        maze_node_t avail_neighbors[4] = {0};
        size_t avail_neighbors_len = 0;
        maze_get_available_neighbors(maze, *cur_coor, avail_neighbors, &avail_neighbors_len);

        if (avail_neighbors_len == 0) {
            if (maze_get(maze, *cur_coor) == MAZE_NODE_DIR_NONE) {
                maze_set_node_dir(maze, *cur_coor, MAZE_NODE_DIR_BLOCKED);
            }

            coordinate_t prev_coor = stack_pop(&stack);
            cur_coor->x = prev_coor.x;
            cur_coor->y = prev_coor.y;
            continue;
        }

        maze_node_t dir = avail_neighbors[rand() % avail_neighbors_len];
        maze_set_node_dir(maze, *cur_coor, dir);
        stack_push(&stack, *cur_coor);

        cur_coor->y -= dir == MAZE_NODE_DIR_UP;
        cur_coor->x += dir == MAZE_NODE_DIR_RIGHT;
        cur_coor->y += dir == MAZE_NODE_DIR_DOWN;
        cur_coor->x -= dir == MAZE_NODE_DIR_LEFT;
    }

    maze_println(maze);

    coordinate_cleanup(cur_coor);
    stack_cleanup(&stack);
}

void maze_cleanup(maze_t *maze)
{
    for (size_t i = 0; i < maze->width; i++) {
        free(maze->body[i]);
    }

    free(maze->body);
}
