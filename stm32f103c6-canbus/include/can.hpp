#ifndef CONTROLLER_CAN_H
#define CONTROLLER_CAN_H

#include <stdint.h>
#include <string.h>

namespace CAN {

static const int max_can_msg_size{8};

class Frame {
public:
  Frame()
     // : id(0), extended_id(false), rtr(false), filter_id(0), len(0), ts(0) {}
      : id(0), extended_id(true), rtr(false), filter_id(0), len(0), ts(0) {}
  Frame(int i, bool e, bool r, uint8_t fid, uint8_t l, uint8_t d[8], uint16_t t)
      : id(i), extended_id(e), rtr(r), filter_id(fid), len(l), ts(t) {
    memcpy(data, d, l);
  }

public:
  uint32_t id;
  bool extended_id;
  bool rtr;
  uint8_t filter_id;
  uint8_t len;
  uint8_t data[max_can_msg_size];
  uint16_t ts;

public:
  Frame &operator=(Frame &other) {
    if (this != &other) {
      id = other.id;
      extended_id = other.extended_id;
      rtr = other.rtr;
      filter_id = other.filter_id;
      len = other.len;
      ts = other.ts;
      memcpy(data, other.data, len);
    }
    return *this;
  }
};

void init();
bool has_pending_messages();
bool pop(Frame &out);
int transmit(Frame &frame);

} // namespace CAN

#endif // ifndef CONTROLLER_CAN_H
