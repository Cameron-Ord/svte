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
    PUNCT_TOKEN = 3, 
    NUMERIC_TOKEN = 4,
    SPACE_TOKEN = 5,
    QUOTATION_TOKEN = 7,
    UNKNOWN_TOKEN = 8,
    ALHPA_NUMERIC = 9, 
    OOB = 10,
}TOKEN_TYPES;

typedef enum {
    GENERIC_TEXT = 0,
    OPERATORS = 1, 
    PUNCTUATION = 2,
    DIGITS = 3,
    TYPE_DEFINITION = 4,
    KEYWORD = 5,
    FUNCTION = 6,
    IGNORE = 7,
    COMMENT = 8, 
    STRING = 9
}SUBSTR_TYPES;


#endif