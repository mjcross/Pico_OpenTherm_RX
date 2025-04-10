# Pico OpenTherm RX

**OpenTherm** is a protocol for central heating boilers and controllers to communicate using 32-bit frames (see [OpenTherm 2.2 spec](OT-Protocol-Specification-v2-2.pdf)).

This is a project for the RPi Pico (RP2040) to send and receive OpenTherm frames using PIO state machines: one to encode/transmit and the other to receive/decode. Both SMs are connected to the same GPIO (loopback test).

The repository is structured as a VS Code project with CMake. It is configured to manage the target device via a [Picoprobe](https://github.com/raspberrypi/picoprobe) USB/SWD bridge.

## Notes

OpenTherm encodes bits with Manchester / Bi-phase-L (rising edge '0', falling edge '1'). Frames consist of a leading '1', 32 data bits and a final '1'. Frames are sent LSB first at 1kb/s with the first data bit being an (even) parity bit.

The transmit and receive processes communicate with the CPU via the PIO state machine FIFO queues at one 32-bit word per frame.

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
- display binary frame fields
- add parity check
- update documentation