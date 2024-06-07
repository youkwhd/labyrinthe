#ifndef __LABYRINTHE_QUEUE_H
#define __LABYRINTHE_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

#include "coordinate.h"

typedef struct queue_t {
    size_t cursor, size;
    coordinate_t *arr;
} queue_t;

void queue_init(queue_t *q, size_t size);
bool queue_is_empty(queue_t *q);
void queue_enqueue(queue_t *q, coordinate_t x);
coordinate_t queue_head(queue_t *q);
coordinate_t queue_dequeue(queue_t *q);
void queue_cleanup(queue_t *q);

#endif // __LABYRINTHE_QUEUE_H
