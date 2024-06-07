#include <stdlib.h>

#include "queue.h"

void queue_init(queue_t *q, size_t size)
{
    q->cursor = 0;
    q->size = size;
    q->arr = malloc((sizeof *q->arr) * q->size);
}

bool queue_is_empty(queue_t *q)
{
    return q->cursor == 0;
}

void queue_enqueue(queue_t *q, coordinate_t x)
{
    q->arr[q->cursor++] = x;
}

coordinate_t queue_head(queue_t *q)
{
    return q->arr[0];
}

coordinate_t queue_dequeue(queue_t *q)
{
    coordinate_t val = q->arr[0];

    for (size_t i = 0; i < q->cursor; i++)
        q->arr[i] = q->arr[i + 1];

    q->cursor--;
    return val;
}

void queue_cleanup(queue_t *q)
{
    free(q->arr);
}
