#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "OT_rx_library.h"
#include "OT_tx_library.h"


const PIO pio = pio0;
const uint gpio = 15;

int main() {
    stdio_init_all();

    uint tx_sm, rx_sm;
    uint32_t tx_frame, rx_frame;

    // load and start TX state machine
    if (OT_tx_init (pio, &tx_sm, gpio)) {
        puts ("TX running");
    } else {
        puts ("failed to load TX");
        while (true);
    }

    // load and start RX state machine
    if (OT_rx_init (pio, &rx_sm, gpio)) {
        puts ("RX running");
    } else {
        puts ("failed to load RX");
        while (true);
    }

    while (true) {
        tx_frame = get_rand_32();

        printf("sent: 0x%08lx\t", tx_frame);
        pio_sm_put_blocking (pio, tx_sm, tx_frame);

        sleep_ms (1000);
        printf ("received: ");
        if (pio_sm_is_rx_fifo_empty (pio, rx_sm)) {
            puts ("nothing");
        } else {
            while (pio_sm_is_rx_fifo_empty (pio, rx_sm) == false) {
                rx_frame = pio_sm_get_blocking (pio, rx_sm);
                printf ("0x%08lx ", rx_frame);
            }
            putchar ('\n');
        }
    }
}