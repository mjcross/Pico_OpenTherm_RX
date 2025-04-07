#include <stdio.h>
#include "pico/stdlib.h"
#include "OT_rx_library.h"


const PIO pio = pio0;
const uint gpio = PICO_DEFAULT_LED_PIN;

int main() {
    stdio_init_all();

    uint rx_sm;
    uint32_t frame;
    if (OT_rx_init (pio, &rx_sm, gpio)) {
        puts ("running");
        while(true) {
            if (pio_sm_is_tx_fifo_empty (pio, rx_sm) == false) {
                frame = pio_sm_get_blocking (pio, rx_sm);
                printf("RX: 0x%08lu\n", frame);
            }
 
            sleep_ms (1000);
        }
    }

    puts ("halted");
    while (true);
}