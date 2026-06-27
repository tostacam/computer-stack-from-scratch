#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>
#include "bit.h"
#include "bus.h"
#include "decoder.h"

bus64 encode_amount(uint64_t n);
bit is_equal_bus_uint(bus64 a, uint64_t n);

#endif
