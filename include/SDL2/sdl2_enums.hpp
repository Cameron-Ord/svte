#ifndef SDL2_ST_ENUMS_HPP
#define SDL2_ST_ENUMS_HPP

typedef enum { RUN = 1, NO_RUN = 0 } RUN_STATE;
typedef enum { STOP_INPUT = 1, START_INPUT = 2} TEXT_INPUT_OPTS;

typedef enum { 
    SDL2_NIL = -1,
    BAD_WINDOW = 0, 
    BAD_RENDERER = 1, 
    BAD_INIT = 2,
    BAD_FONT = 3,
    BAD_TEXTURE = 4,
    BAD_ALLOC = 5,
} SDL2_ERR_CODES;

#endif