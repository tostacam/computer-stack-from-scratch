#ifndef DECODER_ENCODER_H
#define DECODER_ENCODER_H

#include <stdint.h>
#include "bus.h"

uint64_t decode_amount(bus64 a);
bus64 encode_amount(uint64_t n);

#endif
