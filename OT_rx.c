#include <stdio.h>
#include "pico/stdlib.h"
#include "OT_rx_library.h"
#include "OT_tx_library.h"


const PIO pio = pio0;
const uint gpio = 15;

int main() {
    stdio_init_all();

    uint tx_sm, rx_sm;
    uint32_t tx_frame = 0x55555555UL, rx_frame;

    if (OT_tx_init (pio, &tx_sm, gpio)) {
        puts ("tramsmitter running");
    } else {
        puts ("problem loading tx code");
        while (true);
    }

    if (OT_rx_init (pio, &rx_sm, gpio)) {
        puts ("receiver running");
    } else {
        puts ("problem loading rx code");
        while (true);
    }

    while (true) {
        sleep_ms (100);
        printf("TX: 0x%032lb\t", tx_frame);
        pio_sm_put_blocking (pio, tx_sm, tx_frame);

        sleep_ms (1000);
        printf ("RX: ");
        if (pio_sm_is_rx_fifo_empty (pio, rx_sm)) {
            putchar ('-');
        } else {
            while (pio_sm_is_rx_fifo_empty (pio, rx_sm) == false) {
                rx_frame = pio_sm_get_blocking (pio, rx_sm);
                printf ("0x%032lb ", rx_frame);
            }
        }
        putchar ('\n');
    }
}