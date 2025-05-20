#ifndef CORE_EVENT_MGR_HPP
#define CORE_EVENT_MGR_HPP

#include <string>
#include <cstdint>

struct EventKeys {
    const uint8_t COMMAND_COMMIT = 0;
    const uint8_t BUFFER_INPUT = 1;
    const uint8_t COMMAND_INPUT = 2;
    const uint8_t INPUT_ACTIVE = 3;
    const uint8_t BUFFER_CURSOR_MOVED = 4;
    const uint8_t COMMAND_CURSOR_MOVED = 5;
    const uint8_t DISPLAY_SIZE_CHANGED = 6;
    const uint8_t CHANGE_BUFFER = 7;
    const uint8_t NO_KEY = 8;
};

struct EventOpts {
    const uint8_t NO_OPTION = 0;
    const uint8_t STOP_TEXT_INPUT = 1;
    const uint8_t START_TEXT_INPUT = 2;
    const uint8_t NEW_BUFFER = 3;
};

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