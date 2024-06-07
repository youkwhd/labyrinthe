#include <stdlib.h>

#include "stack.h"

void stack_init(stack_t *st, size_t size)
{
    st->size = size;
    st->cursor = -1;
    st->arr = malloc((sizeof *st->arr) * st->size);
}

bool stack_is_empty(stack_t *st)
{
    return st->cursor == -1;
}

void stack_push(stack_t *st, coordinate_t x)
{
    st->arr[++st->cursor] = x;
}

void stack_copy(stack_t *st, stack_t *from)
{
    st->size = from->size;
    st->cursor = from->cursor;
    st->arr = malloc((sizeof *st->arr) * st->size);

    for (int i = 0; i <= from->cursor; i++)
        st->arr[i] = from->arr[i];
}

coordinate_t stack_top(stack_t *st)
{
    return st->arr[st->cursor];
}

coordinate_t stack_pop(stack_t *st)
{
    return st->arr[st->cursor--];
}

void stack_cleanup(stack_t *st)
{
    free(st->arr);
}

