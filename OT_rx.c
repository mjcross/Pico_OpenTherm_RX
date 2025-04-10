#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "OT_rx_library.h"
#include "OT_tx_library.h"

const PIO pio = pio0;
const uint opentherm_gpio = 15;   // GPIO to use for OpenTherm loopback


typedef union {
    struct {
        uint32_t data_value: 16;
        uint32_t data_id: 8;
        uint32_t spare: 4;
        uint32_t msg_type: 3;
        uint32_t parity: 1;
    };
    uint32_t raw;
} frame_t;  // bitfields in frame (OpenTherm Protocol Spec v2.2)


uint32_t OT_frame_calc_parity (frame_t frame) {
    int parity = 0;
    while (frame.raw) {
        frame.raw <<= 1;    // intentionally excludes first bit from calc
        parity ^= frame.parity;
    }
    return parity;
}

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
        printf ("= %032lb = parity %01b msg_type %03b spare %04b data_id %08b data_value %016b ", 
            rx_frame.raw, 
            rx_frame.parity,
            rx_frame.msg_type, 
            rx_frame.spare, 
            rx_frame.data_id,
            rx_frame.data_value
        );

        puts (rx_frame.parity == OT_frame_calc_parity (rx_frame) ? "parity good": "parity bad");
    }
}