#ifndef __LABYRINTHE_STACK_H
#define __LABYRINTHE_STACK_H

#include <stdbool.h>
#include <stddef.h>

#include "coordinate.h"

typedef struct stack_t {
    int cursor;
    size_t size;
    coordinate_t *__stack;
} stack_t;

void stack_init(stack_t *st, size_t size);
bool stack_is_empty(stack_t *st);
void stack_push(stack_t *st, coordinate_t x);
void stack_copy(stack_t *st, stack_t *from);
coordinate_t stack_top(stack_t *st);
coordinate_t stack_pop(stack_t *st);
void stack_cleanup(stack_t *st);

#endif // __LABYRINTHE_STACK_H
