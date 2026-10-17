# RT-950 Pro Pinout and Signal Map

This map uses the OEM V0.27 firmware image and reported tests on a radio. The [V0.27 analysis note](rt950-v027-analysis.md) gives the function addresses and limits for each firmware check. The [earlier pin list](pinout-legacy.md) keeps the full set of board labels from prior work.

**Evidence terms:** "Firmware check" means that code in the V0.27 image uses the stated port and pin mask. "Board report" means that another project reports a test on a radio; this review did not repeat that test. A firmware check does not prove the physical signal name.

## Signals checked in V0.27

| Circuit | Port and pins | Evidence status |
| --- | --- | --- |
| BK4829 serial bus | PE10 clock, PE11 data, PE8 select for chip 0, PE15 select for chip 1 | Firmware check at `0x0801E260`, `0x0801F090`, and `0x0801F0E4`. |
| External SPI flash | PB12 chip select; SPI2 base `0x40003800` | Firmware check at `0x08017000`. PB13 clock, PB14 MISO, and PB15 MOSI come from the board pin map and the SPI2 alternate function. |
| GPS serial port | PB10 with USART3 base `0x40004800` | Firmware check at `0x08016B20`. PB11 as the other serial pin is a board map claim. |
| Bluetooth serial port | PA9 with USART1 base `0x40013800` | Firmware literal check near `0x0800A578`. PA10 as the other serial pin is a board map claim. |
| Programming serial port | PC10 and PC11 with UART4 base `0x40004C00` | Firmware setup near `0x08025738`. Connector wiring needs a board check. |
| Display data bus | PD8 to PD15 | Firmware writes bytes into GPIOD bits 8 to 15 near `0x080297B8`. |
| Display control | PD0 write strobe, PD1 chip select, PD3 command/data | Firmware check near `0x080297B8` and `0x08029818`. |
| Backlight | PC6 and PB3 | Firmware check at `0x08017C40`. Both pins change together. |
| RF path relays | PE13, PE12, PE14, PE7, PB0, PB1 | Firmware check at `0x0801D268`. Check idle levels on the board before a transmit test. |

An aligned word scan found no SPI1 base address in the V0.27 image. The RF bus checked above uses GPIOE, and the flash setup uses SPI2. Earlier descriptions of a BK4829 on SPI1 and flash on GPIOA do not match these V0.27 functions.

## Pins reported from board work

| Circuit | Reported pins | Check still needed |
| --- | --- | --- |
| Keypad matrix | PC0 to PC3 columns; PD4 to PD7 rows | The [Hertzz58 pin map](https://github.com/Hertzz58/Radtel-RT950-Pro-Firmware/blob/main/include/rt950_pinmap.h) reports a live GPIO scan. The earlier published scanner address did not identify a scanner in this Ghidra check. |
| Rotary encoder | PB4 and PB5 | Board report. Read both inputs while the knob turns. |
| PTT inputs | PE3 and PE2 | Board report. Keep these inputs separate from the RF relay outputs above. |
| Program buttons | PA12 and PE5 | Firmware check in the bootloader at `0x0800049C`. It reads both pins low twice before update mode. Check the button labels on the radio. |
| SI4732 serial bus | PB6 clock and PB7 data | Board map claim. This review did not find a reliable V0.27 function at the published address. |
| Display reset | PD2 | Board map claim. The display control pins in the first table were checked; reset was not. |
| Status LEDs | PC13 red and PC14 green | Board report. |

The roles of PA0 and PA1 for battery and VOX measurements remain open. The earlier pin list assigns them, but the project pin map reports a conflict. Do not use either role for a displayed battery value until a board test confirms it.

The [pinout slides](pinout.pptx) give a visual view of MCU pins. The [RF schematic](RT950Pro.pdf) shows RF circuits and signal names. The schematic does not assign all MCU pins.
