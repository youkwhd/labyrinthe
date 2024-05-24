#ifndef __LABYRINTHE_COORDINATE_H
#define __LABYRINTHE_COORDINATE_H

#include "direction.h"

#include <stdbool.h>

typedef struct coordinate_t {
    int x, y;
} coordinate_t;

coordinate_t *coordinate_init(int x, int y);
void coordinate_println(coordinate_t c);
void coordinate_move_to(coordinate_t *c, direction_t dir);
bool coordinate_equal(coordinate_t c1, coordinate_t c2);
coordinate_t coordinate_parse_from_string(char *str);
void coordinate_cleanup(coordinate_t *c);

#endif // __LABYRINTHE_COORDINATE_H
