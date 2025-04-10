#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "OT_rx_library.h"
#include "OT_tx_library.h"

const PIO pio = pio0;
const uint opentherm_gpio = 15;   // GPIO to use for OpenTherm loopback

typedef union {
    struct {
        uint32_t data: 16;
        uint32_t id: 8;
        uint32_t spare: 4;
        uint32_t type: 3;
        uint32_t parity: 1;
    };
    uint32_t raw;
} frame_t;  // bitfields in frame (OpenTherm Protocol Spec v2.2)


int main() {
    stdio_init_all();

    uint tx_sm, rx_sm;
    frame_t tx_frame, rx_frame;

    // load and start TX state machine
    if (OT_tx_init (pio, &tx_sm, opentherm_gpio)) {
        puts ("TX running");
    } else {
        puts ("failed to load TX");
        while (true);
    }

    // load and start RX state machine
    if (OT_rx_init (pio, &rx_sm, opentherm_gpio)) {
        puts ("RX running");
    } else {
        puts ("failed to load RX");
        while (true);
    }

    while (true) {
        tx_frame.raw = get_rand_32();   // from pico/rand

        printf("sent: 0x%08lx\t", tx_frame.raw);
        pio_sm_put_blocking (pio, tx_sm, tx_frame.raw);

        sleep_ms (1000);
        printf ("received: ");
        if (pio_sm_is_rx_fifo_empty (pio, rx_sm)) {
            puts ("nothing");
        } else {
            while (pio_sm_is_rx_fifo_empty (pio, rx_sm) == false) {
                rx_frame.raw = pio_sm_get_blocking (pio, rx_sm);
                printf ("0x%08lx ", rx_frame.raw);
            }
        }

        // show raw frame and bitfields
        printf ("= %032lb = %01b %03b %04b %08b %016b\n", 
            rx_frame.raw, 
            rx_frame.parity, 
            rx_frame.type, 
            rx_frame.spare, 
            rx_frame.id,
            rx_frame.data
        );
    }
}