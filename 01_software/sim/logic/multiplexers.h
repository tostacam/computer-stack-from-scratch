#ifndef MULTIPLEXERS_H
#define MULTIPLEXERS_H

#include "gates.h"
#include "bus.h"

bit mux(bit a, bit b, bit sel);
bus64 mux64(bus64 a, bus64 b, bit sel);

#endif 
