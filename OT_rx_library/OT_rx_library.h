#include "hardware/pio.h"
#include "hardware/clocks.h"            // for clock_get_hz() in generated header
#include "OT_rx_library.pio.h"          // generated by pioasm

bool OT_rx_init (PIO pio, uint *p_sm, uint gpio);
