#ifndef __LABYRINTHE_MATRIX_H
#define __LABYRINTHE_MATRIX_H

#include <stddef.h>
#include <stdint.h>

typedef struct matrix_t {
    uint32_t rows, cols;
    size_t type_size;
    void ***__matrix;
} matrix_t;

void matrix_init(matrix_t *mt, uint32_t rows, uint32_t cols, size_t type_size);
void matrix_set(matrix_t *mt, uint32_t row, uint32_t col, void *var);
void matrix_cleanup(matrix_t *mt);

#endif // __LABYRINTHE_MATRIX_H
