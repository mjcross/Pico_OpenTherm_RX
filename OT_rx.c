#include <stdio.h>
#include "pico/stdlib.h"
#include "OT_rx_library.h"
#include "OT_tx_library.h"


const PIO pio = pio0;
const uint gpio = 15;

int main() {
    stdio_init_all();

    uint tx_sm;
    uint32_t tx_frame = 0x0fa1;
    if (OT_tx_init (pio, &tx_sm, gpio)) {
        puts ("tx code loaded");
        while (true) {
            sleep_ms (1000);
            putchar('.');
            pio_sm_put_blocking (pio, tx_sm, tx_frame);
        }
    } else {
        puts ("problem loading tx code");
        while (true);
    }


    uint rx_sm;
    uint32_t rx_frame;
    if (OT_rx_init (pio, &rx_sm, gpio)) {
        puts ("running");
        while(true) {
            if (pio_sm_is_tx_fifo_empty (pio, rx_sm) == false) {
                rx_frame = pio_sm_get_blocking (pio, rx_sm);
                printf("RX: 0x%08lu\n", rx_frame);
            }
 
            sleep_ms (1000);
        }
    }

    puts ("halted");
    while (true);
}