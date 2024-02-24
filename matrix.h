#ifndef __LABYRINTHE_MATRIX_H
#define __LABYRINTHE_MATRIX_H

typedef struct matrix_t {
    void **__matrix;
} matrix_t;

void matrix_init(matrix_t *mt);
void matrix_cleanup(matrix_t *mt);

#endif // __LABYRINTHE_MATRIX_H
