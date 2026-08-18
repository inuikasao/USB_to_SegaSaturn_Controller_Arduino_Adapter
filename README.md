# USB → Sega Saturn Controller Arduino adapter


An adapter that allows USB HID controllers to be used with the Sega Saturn, combining a general-purpose logic IC (TC74HC153) with an Arduino.

## Overview

The Sega Saturn controller protocol is delegated entirely to the TC74HC153 (4-to-1 multiplexer), while the Arduino only handles Hi/Lo switching of GPIO outputs. The IC automatically processes Saturn's page scan timing, so no timing control is required on the microcontroller side.

```
[USB HID Controller]
        ↓ USB
[Arduino UNO + USB Host Shield]
        ↓ 13 GPIO pins
[TC74HC153 × 2]
        ↓
[Saturn Controller Port]
```

## Hardware

### Parts List

| Part | Qty |
|------|-----|
| TC74HC153 (or equivalent) | 2 |
| 16-pin DIP IC socket | 2 |
| Arduino UNO R3 (or compatible) | 1 |
| USB Host Shield 2.0 | 1 |
| Saturn controller extension cable (for male connector) | 1 |
| Universal PCB | 1 |
| Ceramic capacitor 0.1μF | 2 (across Vcc-GND of each IC; add if operation is unstable) |

## IC Pin Connections

### U1 (handles d0/d1)

| IC Pin | Signal | Connection |
|--------|--------|------------|
| pin1 (1G̅) | Enable (active low) | GND (always enabled) |
| pin2 (B) | S1 | Saturn cable pin5 |
| pin3 (1C3) | d0 page3 input | GND (fixed 0) |
| pin4 (1C2) | d0 page2 input | Arduino D4 (UP) |
| pin5 (1C1) | d0 page1 input | Arduino D0 (Y) |
| pin6 (1C0) | d0 page0 input | Arduino D2 (Z) |
| pin7 (1Y) | d0 output | Saturn cable pin3 |
| pin8 (GND) | GND | GND |
| pin9 (2Y) | d1 output | Saturn cable pin2 |
| pin10 (2C0) | d1 page0 input | Arduino D3 (B) |
| pin11 (2C1) | d1 page1 input | Arduino D6 (C) |
| pin12 (2C2) | d1 page2 input | Arduino D5 (DOWN) |
| pin13 (2C3) | d1 page3 input | GND (fixed 0) |
| pin14 (A) | S0 | Saturn cable pin4 |
| pin15 (2G̅) | Enable (active low) | GND (always enabled) |
| pin16 (Vcc) | Power | 5V |

### U2 (handles d2/d3)

| IC Pin | Signal | Connection |
|--------|--------|------------|
| pin1 (1G̅) | Enable (active low) | GND (always enabled) |
| pin2 (B) | S1 | Saturn cable pin5 ← shared bus with U1 |
| pin3 (1C3) | d2 page3 input | Vcc (fixed 1) |
| pin4 (1C2) | d2 page2 input | Arduino A0 (LEFT) |
| pin5 (1C1) | d2 page1 input | Arduino A5 (A) |
| pin6 (1C0) | d2 page0 input | Arduino D1 (X) |
| pin7 (1Y) | d2 output | Saturn cable pin8 |
| pin8 (GND) | GND | GND |
| pin9 (2Y) | d3 output | Saturn cable pin7 |
| pin10 (2C0) | d3 page0 input | Arduino A1 (R) |
| pin11 (2C1) | d3 page1 input | Arduino A2 (START) |
| pin12 (2C2) | d3 page2 input | Arduino A3 (RIGHT) |
| pin13 (2C3) | d3 page3 input | Arduino A4 (L) |
| pin14 (A) | S0 | Saturn cable pin4 ← shared bus with U1 |
| pin15 (2G̅) | Enable (active low) | GND (always enabled) |
| pin16 (Vcc) | Power | 5V |

### Arduino GPIO Assignment

| Arduino | IC Pin | Button |
|---------|--------|--------|
| D0 | U1 pin5 (1C1) | Y |
| D1 | U2 pin6 (1C0) | X |
| D2 | U1 pin6 (1C0) | Z |
| D3 | U1 pin10 (2C0) | B |
| D4 | U1 pin4 (1C2) | UP |
| D5 | U1 pin12 (2C2) | DOWN |
| D6 | U1 pin11 (2C1) | C |
| A0 | U2 pin4 (1C2) | LEFT |
| A1 | U2 pin10 (2C0) | R |
| A2 | U2 pin11 (2C1) | START |
| A3 | U2 pin12 (2C2) | RIGHT |
| A4 | U2 pin13 (2C3) | L |
| A5 | U2 pin5 (1C1) | A |

> **Note**: D0/D1 are shared with serial communication (RX/TX). When uploading sketches or using the Serial Monitor, disconnect the D0/D1 jumper wires from the IC board.

## Power and Pin Conflicts

### Power

By sharing the Saturn controller port Vcc lines (pin1, pin6) with the IC Vcc (pin16), the adapter can run entirely on power from the Saturn console. GND (pin9) must be shared with the Arduino.

Note: Do not connect Saturn and USB at the same time. To upload sketches, disconnect from the Saturn and use a USB cable. If operation is unstable, leave pin1/pin6 floating and power via USB instead.

### Prohibited Pins

The following pins conflict with the USB Host Shield and cannot be used for the IC board.

| Pin | USB Host Shield usage |
|-----|-----------------------|
| D7 | MAX3421E RESET |
| D8 | MAX3421E GPX |
| D9 | INT |
| D10 | SS |
| D11 | MOSI |
| D12 | MISO |
| D13 | SCK (also prohibited for LED use) |

### Notes on USB Host Shield Compatible Boards

Some compatible boards require the VBUS PWR jumper to be bridged on the **3.3V side** (not the 5V side) to function correctly (due to a 74HC125 issue). Some boards also require explicit bridging of the INT and SS jumpers.

| Jumper | Setting |
|--------|---------|
| VBUS PWR | Bridge 3.3V side (not 5V) |
| 5V | Bridge |
| 3.3V | Bridge |
| INT | Bridge to D9 |
| SS | Bridge to D10 |

## References

- [SEGA Saturn ControlPAD Schematic by H.Kashima](https://kaele.com/~kashima/games/saturn.html)
- [Saturn Controller - GameSX](https://gamesx.com/controldata/saturn.htm)
- [Saturn Controller - Hardware Book](https://www.hardwarebook.info/Saturn_controller)
- [Saturn I/O Pinout - plutiedev](https://www.plutiedev.com/io-pinout)
- [USB Host Shield Library 2.0](https://github.com/felis/USB_Host_Shield_2.0)

## License
MIT License — see [LICENSE](LICENSE) for details.
