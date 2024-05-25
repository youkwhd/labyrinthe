#include "direction.h"

#include <stdio.h>

void direction_println(direction_t dir)
{
    if (dir == DIRECTION_INVALID) {
        printf("DIRECTION_INVALID\n");
        return;
    }
    if (dir & DIRECTION_NONE) {
        printf("DIRECTION_NONE\n");
    }
    if (dir & DIRECTION_BLOCKED) {
        printf("DIRECTION_BLOCKED\n");
    }
    if (dir & DIRECTION_LEFT) {
        printf("DIRECTION_LEFT\n");
    }
    if (dir & DIRECTION_RIGHT) {
        printf("DIRECTION_RIGHT\n");
    }
    if (dir & DIRECTION_DOWN) {
        printf("DIRECTION_DOWN\n");
    }
    if (dir & DIRECTION_UP) {
        printf("DIRECTION_UP\n");
    }
    if (dir & DIRECTION_OPENED) {
        printf("DIRECTION_OPENED\n");
    }
    if (dir & DIRECTION_TRAVERSED) {
        printf("DIRECTION_TRAVERSED\n");
    }

    printf("\n");
}
