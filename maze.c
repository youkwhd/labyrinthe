#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "maze.h"
#include "stack.h"

#define MAZE_NODE_WIDTH 3
#define MAZE_NODE_HEIGHT 3
#define MAZE_NODE_SIZE (MAZE_NODE_WIDTH * MAZE_NODE_HEIGHT)

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

void __maze_str_set_open_dir(maze_t *maze, coordinate_t coor, maze_node_t dir)
{
    if (dir == DIRECTION_BLOCKED) {
        return;
    }

    maze->_maze_str[(2 * coor.y + (dir == DIRECTION_DOWN) - (dir == DIRECTION_UP)) + 1][(2 * coor.x + (dir == DIRECTION_RIGHT) - (dir == DIRECTION_LEFT)) + 1] = ' ';
}

void maze_set_node_dir(maze_t *maze, coordinate_t coor, maze_node_t dir)
{
    maze->body[coor.y][coor.x] |= dir;
    __maze_str_set_open_dir(maze, coor, dir);
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

void _maze_gen_str_upper_border(maze_t *maze, char *str)
{
    str[0] = '*';

    for (size_t i = 1; i < maze->_maze_str_width - 1; i += 2) {
        str[i] = '-';
        str[i + 1] = '*';
    }

    str[maze->_maze_str_width - 1] = '\0';
}

void _maze_gen_str_middle_border(maze_t *maze, char *str)
{
    str[0] = '|';

    for (size_t i = 1; i < maze->_maze_str_width - 1; i += 2) {
        str[i] = ' ';
        str[i + 1] = '|';
    }

    str[maze->_maze_str_width - 1] = '\0';
}

void __maze_println(maze_t *maze)
{
    for (size_t i = 0; i < maze->_maze_str_height; i++) {
        printf("%s\n", maze->_maze_str[i]);
    }
}

void __maze_gen_str(maze_t *maze)
{
    _maze_gen_str_upper_border(maze, maze->_maze_str[0]);

    for (size_t i = 1; i < maze->_maze_str_height; i += 2) {
        _maze_gen_str_middle_border(maze, maze->_maze_str[i]);
        _maze_gen_str_upper_border(maze, maze->_maze_str[i + 1]);
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

    /* TODO: explain the pluses
     */
    maze->_maze_str_width = maze->width * 2 + 1 + 1;
    maze->_maze_str_height = maze->height * 2 + 1;

    maze->_maze_str = malloc((sizeof *maze->_maze_str) * maze->_maze_str_height);
    for (size_t i = 0; i < maze->_maze_str_height; i++) {
        maze->_maze_str[i] = malloc((sizeof *maze->_maze_str) * maze->_maze_str_width);
    }

    __maze_gen_str(maze);
}

coordinate_t maze_set_starting_point(maze_t *maze, coordinate_t start)
{
    maze_node_t avail_neighbors[4] = {DIRECTION_NONE};
    size_t avail_neighbors_len = 0;
    maze_get_available_neighbors(maze, start, avail_neighbors, &avail_neighbors_len);

    maze_node_t dir = avail_neighbors[rand() % avail_neighbors_len];
    maze_set_node_dir(maze, start, dir);

    coordinate_move_to(&start, dir);
    return start;
}

void maze_generate(maze_t *maze, coordinate_t start)
{
    srand(time(NULL));

    stack_t stack;
    stack_init(&stack, maze->height * maze->width);

    coordinate_t cur_coor = maze_set_starting_point(maze, start);
    stack_push(&stack, start);

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
        coordinate_move_to(&cur_coor, dir);
    }

    stack_cleanup(&stack);
}

void maze_cleanup(maze_t *maze)
{
    for (size_t i = 0; i < maze->width; i++) {
        free(maze->body[i]);
    }

    free(maze->body);

    for (size_t i = 0; i < maze->_maze_str_height; i++) {
        free(maze->_maze_str[i]);
    }

    free(maze->_maze_str);
}
