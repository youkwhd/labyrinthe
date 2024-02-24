#ifndef __LABYRINTHE_STACK_H
#define __LABYRINTHE_STACK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct stack_t {
    int cursor;
    size_t size;
    size_t type_size;
    void **__stack;
} stack_t;

void stack_init(stack_t *st, size_t size, size_t type_size);
bool stack_is_empty(stack_t *st);
void stack_push(stack_t *st, void *x);
void *stack_top(stack_t *st);
void *stack_pop(stack_t *st);
void stack_cleanup(stack_t *st);

#endif // __LABYRINTHE_STACK_H
