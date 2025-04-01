#include <stdio.h>
#include "pico/stdlib.h"
#include "myproject_library.h"


const PIO pio = pio0;
const uint gpio = PICO_DEFAULT_LED_PIN;

int main() {
    stdio_init_all();

    uint sm;
    if (myproject_init (pio, &sm, gpio)) {
        puts ("running");
        while(true) {
            pio_sm_put (pio, sm, 1);    // send '1' to SM FIFO (LED on)
            sleep_ms (500);
            pio_sm_put (pio, sm, 0);    // send '0' to SM FIFO (LED off)
            sleep_ms (500);
        }
    }

    puts ("halted");
    while (true);
}