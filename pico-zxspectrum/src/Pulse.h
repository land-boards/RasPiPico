#pragma once

#include "Pulse.h"
#include <pico/stdlib.h>

class Pulse {
  int _tstates;
  int _cursor;
  int _pulses;
public:
  Pulse() :
    _tstates(0),
    _cursor(0),
    _pulses(0)
  {
  }

  void __not_in_flash_func(reset)(int pulses, int tstates) {
    _tstates = tstates;
    _cursor = 0;
    _pulses = pulses;
  }
  
  void __not_in_flash_func(advance)(int *tstates, bool *pstate) {
    while (_pulses != 0 && *tstates > 0) {
      int rem = _tstates - _cursor;
      if (*tstates >= rem) {
        *tstates -= rem;
        _cursor = 0;
        --_pulses;
        *pstate = !*pstate;
      }
      else {
        _cursor += *tstates;
        *tstates = 0;
      }
    }
  }

  bool end() {
    return _pulses == 0;
  }
};
