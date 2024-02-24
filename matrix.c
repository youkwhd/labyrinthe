#include <stdlib.h>

#include "labyrinthe.h"
#include "matrix.h"

void matrix_init(matrix_t *mt, uint32_t rows, uint32_t cols, size_t type_size)
{
    mt->rows = rows;
    mt->cols = cols;
    mt->type_size = type_size;

    mt->__matrix = malloc(sizeof(mt->__matrix[0]) * mt->rows);

    for (uint32_t i = 0; i < mt->rows; i++) {
        mt->__matrix[i] = malloc(sizeof(mt->__matrix[0][i]) * mt->cols * mt->type_size);

        for (uint32_t j = 0; j < mt->cols; j++) {
            mt->__matrix[i][j * mt->type_size] = (void *)0;
        }
    }
}

void matrix_cleanup(matrix_t *mt)
{
    for (uint32_t i = 0; i < mt->rows; i++) {
        free(mt->__matrix[i]);
    }

    free(mt->__matrix);
}
