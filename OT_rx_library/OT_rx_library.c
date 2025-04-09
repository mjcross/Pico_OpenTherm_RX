#include <stdio.h>
#include "hardware/gpio.h"
#include "OT_rx_library.h"

bool OT_rx_init (PIO pio, uint *p_sm, uint gpio) {
    // add the program to the PIO shared instruction memory
    if (pio_can_add_program (pio, &OT_rx_program) == false) {
        puts ("could not add the pio program");
        return false;
    }
    uint offset = pio_add_program (pio, &OT_rx_program);

    // claim a free state machine
    int sm = pio_claim_unused_sm (pio, false);
    if (sm == -1) {
        puts ("could not claim a state machine");
        return false;
    } else {
        *p_sm = (uint)sm;
    }

    // no need to configure IN pins, GPIOs are always readable
 
    // configure and enable the state machine
    OT_rx_sm_init (pio, *p_sm, offset, gpio);

    return true;
}