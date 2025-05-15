#ifndef SDL2_ST_ENUMS_HPP
#define SDL2_ST_ENUMS_HPP

typedef enum { 
    SDL2_NIL = -1,
    SDL2_ERR = 0, 
} SDL2_ERR_CODES;

typedef enum { RUN = 1, NO_RUN = 0 } RUN_STATE;
typedef enum { STOP_INPUT = 1, START_INPUT = 2} TEXT_INPUT_OPTS;

#endif