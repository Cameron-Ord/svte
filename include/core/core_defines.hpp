#ifndef CORE_DEFINES_HPP
#define CORE_DEFINES_HPP
#define SPACE_CHAR ' '
#define NULL_CHAR '\0'
#define NEW_LINE '\n'

typedef enum { 
    CORE_NIL = -1,
    BAD_ID = -1,
    BAD_WRITE = 0, 
    BAD_READ = 1,
    BAD_PATH = 2,
    BAD_FILE_NAME = 3,
    ED_BAD_APPEND = -1,
    NO_BUFFER = 5,
    BAD_OPEN = 6, 
} ERR_CODES;

#endif