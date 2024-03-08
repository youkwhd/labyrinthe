#include <stdlib.h>

#include "coordinate.h"

coordinate_t *coordinate_init(int x, int y)
{
    coordinate_t *c = malloc(sizeof(coordinate_t));
    c->x = x;
    c->y = y;

    return c;
}

void coordinate_move_to(coordinate_t *c, direction_t dir)
{
    c->y -= dir == DIRECTION_UP;
    c->x += dir == DIRECTION_RIGHT;
    c->y += dir == DIRECTION_DOWN;
    c->x -= dir == DIRECTION_LEFT;
}

void coordinate_cleanup(coordinate_t *c)
{
    free(c);
}
