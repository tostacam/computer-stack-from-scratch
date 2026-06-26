#ifndef SHIFTERS_H
#define SHIFTERS_H

#include <stdint.h>
#include "bus.h"

bus64 shift_left(bus64 a, uint64_t n);
bus64 shift_right(bus64 a, uint64_t n);

#endif
