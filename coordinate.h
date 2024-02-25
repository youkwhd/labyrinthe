#ifndef __LABYRINTHE_COORDINATE_H
#define __LABYRINTHE_COORDINATE_H

typedef struct coordinate_t {
    int x, y;
} coordinate_t;

coordinate_t *coordinate_init(int x, int y);
void coordinate_cleanup(coordinate_t *c);

#endif // __LABYRINTHE_COORDINATE_H
