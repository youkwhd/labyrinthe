#include <stdlib.h>

#include "stack.h"

void stack_init(stack_t *st, size_t size, size_t type_size)
{
    st->size = size;
    st->type_size = type_size;
    st->cursor = -1;
    st->__stack = malloc(st->type_size * st->size);
}

bool stack_is_empty(stack_t *st)
{
    return st->cursor == -1;
}

void stack_push(stack_t *st, void *x)
{
    st->__stack[++st->cursor * st->type_size] = x;
}

void *stack_top(stack_t *st)
{
    return st->__stack[st->cursor * st->type_size];
}

void *stack_pop(stack_t *st)
{
    return st->__stack[st->cursor-- * st->type_size];
}

void stack_cleanup(stack_t *st)
{
    free(st->__stack);
}

