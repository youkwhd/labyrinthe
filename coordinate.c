#include <stdlib.h>
#include <stdio.h>

#include "coordinate.h"

coordinate_t *coordinate_init(int x, int y)
{
    coordinate_t *c = malloc(sizeof(coordinate_t));
    c->x = x;
    c->y = y;

    return c;
}

void coordinate_println(coordinate_t c)
{
    printf("X: %d\n", c.x);
    printf("Y: %d\n", c.y);
}

void coordinate_move_to(coordinate_t *c, direction_t dir)
{
    c->y -= dir == DIRECTION_UP;
    c->x += dir == DIRECTION_RIGHT;
    c->y += dir == DIRECTION_DOWN;
    c->x -= dir == DIRECTION_LEFT;
}

bool coordinate_equal(coordinate_t c1, coordinate_t c2)
{
    return c1.x == c2.x && c1.y == c2.y;
}

coordinate_t coordinate_parse_from_string(char *str)
{
    coordinate_t c = {0};

    if (sscanf(str, "%d,%d", &c.x, &c.y) != 2) {
        /* TODO: print progname instead
         */
        fprintf(stderr, "coordinate_parse_from_string(): could not parse coordinate\n");
        exit(EXIT_FAILURE);
    }

    return c;
}

void coordinate_cleanup(coordinate_t *c)
{
    free(c);
}
