#ifndef CORE_EVENT_MGR_HPP
#define CORE_EVENT_MGR_HPP

#include <string>
#include <cstdint>

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

class EventResult
{
  public:
    EventResult(
        const uint8_t KEY_PARAM,
        const uint8_t OPT_PARAM,
        const int32_t ID_PARAM) : KEY(KEY_PARAM), OPT(OPT_PARAM), ID(ID_PARAM) {}

    const int32_t get_event_id(void) const { return ID; }
    const uint8_t get_opt(void) const { return OPT; }
    const uint8_t get_key(void) const { return KEY; }

  private:
    const uint8_t KEY;
    const uint8_t OPT;
    const int32_t ID;
};


#endif