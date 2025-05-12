#ifndef ERROR_CODES_HPP
#define ERROR_CODES_HPP

typedef enum { 
    CORE_NIL = -1,
    BAD_WRITE = 0, 
    BAD_READ = 1,
    BAD_PATH = 2,
    BAD_FILE_NAME = 3,
    ED_BAD_APPEND = 4,
    NO_BUFFER = 5,
    BAD_OPEN = 6, 
    BAD_OPENL = 7
} ERR_CODES;

#endif