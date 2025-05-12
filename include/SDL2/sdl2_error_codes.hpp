#ifndef SDL2_ERROR_CODES_HPP
#define SDL2_ERROR_CODES_HPP

typedef enum { 
    SDL2_NIL = -1,
    BAD_WINDOW = 0, 
    BAD_RENDERER = 1, 
    BAD_INIT = 2,
    BAD_FONT = 3,
    BAD_TEXTURE = 4,
} SDL2_ERR_CODES;

#endif