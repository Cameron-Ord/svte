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

typedef enum {
    COMMAND_COMMIT = 0,
    BUFFER_MUTATION = 1,
    COMMAND_MUTATION = 2,
    BUFFER_CURSOR_MOVE = 4,
    COMMAND_CURSOR_MOVE = 5,
    DISPLAY_SIZE_CHANGED = 6,
    BUFFER_CHANGED = 7,
    NO_KEY = 9,
} EventKeys;

typedef enum {
    NO_OPTION = 0,
    STOP_TEXT_INPUT = 1,
    START_TEXT_INPUT = 2,
    NEW_BUFFER = 3,
    DISPLAY_UPDATE = 4,
} EventOpts;


typedef enum {
    CHAR_TOKEN = 0,
    OPERATOR_TOKEN = 1,
    PUNCT_TOKEN = 2, 
    NUMERIC_TOKEN = 3,
    SPACE_TOKEN = 4,
    QUOTATION_TOKEN = 5,
    UNKNOWN_TOKEN = 6,
    FUNCTION_TOKEN = 7,
    KEYWORD_TOKEN = 8,
    TYPE_TOKEN = 9,
    COMMENT_TOKEN = 10,
    ALPHA_NUMERIC = 11,
    VARIABLE_TOKEN = 12,
    ARGS_TOKEN = 13,
    OOB = 14,
}TOKEN_TYPES;


#endif