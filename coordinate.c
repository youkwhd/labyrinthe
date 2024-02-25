#include <stdlib.h>

#include "coordinate.h"

coordinate_t *coordinate_init(int x, int y)
{
    coordinate_t *c = malloc(sizeof(coordinate_t));
    c->x = x;
    c->y = y;

    return c;
}

void coordinate_cleanup(coordinate_t *c)
{
    free(c);
}
