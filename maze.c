#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "maze.h"
#include "coordinate.h"
#include "stack.h"

bool maze_node_dir_is_valid(maze_node_t prev, maze_node_t next)
{
    switch (next) {
    case MAZE_NODE_DIR_UP:
        return prev != MAZE_NODE_DIR_DOWN;
    case MAZE_NODE_DIR_DOWN:
        return prev != MAZE_NODE_DIR_UP;
    case MAZE_NODE_DIR_LEFT:
        return prev != MAZE_NODE_DIR_RIGHT;
    case MAZE_NODE_DIR_RIGHT:
        return prev != MAZE_NODE_DIR_LEFT;
    default:
        return false;
    }
}

void maze_node_printc(maze_node_t node)
{
    switch (node) {
    case MAZE_NODE_DIR_NONE:
        printf("0 ");
        break;
    case MAZE_NODE_DIR_BLOCKED:
        printf("B ");
        break;
    case MAZE_NODE_DIR_UP:
        printf("U ");
        break;
    case MAZE_NODE_DIR_RIGHT:
        printf("R ");
        break;
    case MAZE_NODE_DIR_DOWN:
        printf("D ");
        break;
    case MAZE_NODE_DIR_LEFT:
        printf("L ");
        break;
    default:
        printf("! ");
        break;
    }
}

void maze_node_println(maze_node_t node)
{
    switch (node) {
    case MAZE_NODE_DIR_NONE:
        puts("MAZE_NODE_DIR_NONE");
        break;
    case MAZE_NODE_DIR_BLOCKED:
        puts("MAZE_NODE_DIR_BLOCKED");
        break;
    case MAZE_NODE_DIR_UP:
        puts("MAZE_NODE_DIR_UP");
        break;
    case MAZE_NODE_DIR_RIGHT:
        puts("MAZE_NODE_DIR_RIGHT");
        break;
    case MAZE_NODE_DIR_DOWN:
        puts("MAZE_NODE_DIR_DOWN");
        break;
    case MAZE_NODE_DIR_LEFT:
        puts("MAZE_NODE_DIR_LEFT");
        break;
    default:
        puts("__MAZE_NODE_DIR_INVALID");
        break;
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
    maze->body[coor.y][coor.x] = dir;
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
        printf("x: %d __ y: %d\n", cur_coor->x, cur_coor->y);

        maze_node_t neighbors[4] = {-1, -1, -1, -1}, empty_neighbors[4] = {0};
        maze_get_neighbors(maze, *cur_coor, neighbors);

        size_t empty_neighbors_length = 0;
        for (size_t i = 0; i < 4; i++) {
            if (neighbors[i] == MAZE_NODE_DIR_NONE) {
                empty_neighbors[empty_neighbors_length++] = 2 + i;
            }
        }

        if (empty_neighbors_length == 0) {
            if (maze->body[cur_coor->y][cur_coor->x] == MAZE_NODE_DIR_NONE) {
                maze_set_node_dir(maze, *cur_coor, MAZE_NODE_DIR_BLOCKED);
            }

            coordinate_t prev_coor = stack_pop(&stack);
            cur_coor->x = prev_coor.x;
            cur_coor->y = prev_coor.y;
            continue;
        }

        for (size_t i = 0; i < empty_neighbors_length; i++) {
            maze_node_println(empty_neighbors[i]);
        }

        maze_node_t dir = empty_neighbors[rand() % empty_neighbors_length];
        printf("SETTING: ");
        maze_node_println(dir);
        maze_set_node_dir(maze, *cur_coor, dir);
        stack_push(&stack, *cur_coor);

        cur_coor->y -= dir == MAZE_NODE_DIR_UP;
        cur_coor->x += dir == MAZE_NODE_DIR_RIGHT;
        cur_coor->y += dir == MAZE_NODE_DIR_DOWN;
        cur_coor->x -= dir == MAZE_NODE_DIR_LEFT;

        maze_println(maze);
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
