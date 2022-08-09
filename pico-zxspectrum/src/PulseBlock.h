#pragma once

#include "PulseByte.h"
#include "PulseByteStream.h"
#include "PulsePreamble.h"

class PulseBlock {
  InputStream* _is;
  PulseByteStream _pulseByteStream;
  PulsePreamble _pulsePreamble;
  int _r;
  int _tstates;
public:
  PulseBlock() {}
  
  inline bool end() {
    return (_is == 0) || (_r < 0) || _pulseByteStream.end();
  }
  
  inline void reset(InputStream* is) {
    if ((_is != 0) && (_is != is)) _is->close();
    if (is) {
      int length = is->readWord();
      int marker = is->readByte();

      if ((length <= 0) || (marker < 0)) {
        is->close();
        return;
      }
      _pulsePreamble.reset(marker);
      _pulseByteStream.reset(is, length - 1); // Take off one for the marker
      _tstates = -10000000;
    }
    _is = is;
    _r = 0;
  }

  inline int advance(int tstates, bool *pstate) {
    if (end()) return -1;
    _tstates += tstates;
    _pulsePreamble.advance(&_tstates, pstate);
    if (!_pulsePreamble.end()) return 0;
    _r = _pulseByteStream.advance(&_tstates, pstate);
      if (_r == 0 && _pulseByteStream.end()) {
      reset(_is);
    }
    else if (_r < 0 || _pulseByteStream.end()) {
      _is->close();
      _is = 0;
    }
    return _r;
  }
};
