#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "maze.h"

bool maze_dir_valid(maze_node_t prev, maze_node_t next)
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

void maze_init(maze_t *maze)
{
    srand(time(NULL));
    matrix_init(&maze->body, 5, 5, sizeof(uint8_t));
    return;

    maze_node_t prev = MAZE_NODE_DIR_DOWN;
    uint32_t i = 0, j = 0;
    matrix_set(&maze->body, i, j, (void *)prev);
    i++;

    maze_node_t next = 2 + (rand() % (5 - 1));
    while (!maze_dir_valid(prev, next)) {
        next = 2 + (rand() % (5 - 1));
    }

    switch (next) {
    case MAZE_NODE_DIR_DOWN:
        matrix_set(&maze->body, i++, j, (void *)next);
        break;
    case MAZE_NODE_DIR_UP:
        matrix_set(&maze->body, i--, j, (void *)next);
        break;
    case MAZE_NODE_DIR_LEFT:
        matrix_set(&maze->body, i, j--, (void *)next);
        break;
    case MAZE_NODE_DIR_RIGHT:
        matrix_set(&maze->body, i, j++, (void *)next);
        break;
    default:
        exit(69);
    }
}

void maze_cleanup(maze_t *maze)
{
    matrix_cleanup(&maze->body);
}
