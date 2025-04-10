# Pico OpenTherm RX

Robustly decode an OpenTherm frame on an RPi Pico (RP2040) using a PIO state machine.

The [OpenTherm 2.2 specification](OT-Protocol-Specification-v2-2.pdf) defines a point-to-point communications protocol between a central heating controller ('master') and boiler ('slave').

The repository is structured as a VS Code project with CMake. It is configured to
manage the target device via a [Picoprobe](https://github.com/raspberrypi/picoprobe) USB/SWD bridge.

## Notes

OpenTherm encodes bits at 1kb/s using Manchester / Bi-phase-L (rising edge '0', falling edge '1'). Frames consist of a leading '1', 32 data bits and a final '1'. The first data bit is an (even) parity bit.

As the interface controls a boiler it is important that bits are decoded robustly. The PIO code therefore enforces the bit-timing requirements of the specification. If the code detects a mis-timed transition then returns a dummy frame with bad parity.

In normal operation the PIO code pushes a 32-bit word to the RX-FIFO for every frame received. If an invalid frame is detected then the calling program must discard and restart the conversation.

## DevLog

01/04/2025
- create project on GitHub from my newly fixed [pico-pio-template](https://github.com/mjcross/pico-pio-template)
- update readme.md *(this file)*

02/04/2025
- set project name and update filenames and CMakeLists

07/04/2025
- add PIO code to receive and decode a frame (compiles OK)
- add `OT_tx_library` with PIO code to transmit a frame for loopback testing (compiles OK)

09/04/2025
- test loopback code, fix error in `OT_rx_library`
- link to 'pico/rand' to randomise frame content

10/04/2025
- define data type for frame bitfields