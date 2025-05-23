#ifndef CORE_EVENT_MGR_HPP
#define CORE_EVENT_MGR_HPP

#include <string>
#include <cstdint>

#include "core_defines.hpp"

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