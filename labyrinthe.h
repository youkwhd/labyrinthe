#ifndef __LABYRINTHE_H
#define __LABYRINTHE_H

#include <stdlib.h>

#define UNUSED(var) (void)var
#define UNIMPLEMENTED() \
    do { \
        fprintf(stderr, "labyrinthe: function `%s` is not implemented yet, at %s:%d\n", __func__, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    } while (0)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif // __LABYRINTHE_H
