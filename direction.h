#ifndef __LABYRINTHE_DIRECTION_H
#define __LABYRINTHE_DIRECTION_H

typedef enum direction_t {
    DIRECTION_INVALID = -1,
    DIRECTION_NONE = 0,
    DIRECTION_BLOCKED = 1,
    DIRECTION_UP = 1 << 1,
    DIRECTION_RIGHT = 1 << 2,
    DIRECTION_DOWN = 1 << 3,
    DIRECTION_LEFT = 1 << 4,
} direction_t;

#endif // __LABYRINTHE_DIRECTION_H
