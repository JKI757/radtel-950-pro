# RT-950 Pro Display Notes

These notes describe the OEM V0.27 application. See the [V0.27 analysis note](rt950-v027-analysis.md) for the image hash and analysis method.

## Interface

- The firmware writes display data to GPIOD pins PD8 to PD15. It inserts each byte into bits 8 to 15 of the port value.
- PD0 is the write strobe. PD1 is chip select. PD3 selects a command or data byte. Ghidra shows these masks in the functions at `0x080297B8` and `0x08029818`.
- PC6 and PB3 change together in the backlight function at `0x08017C40`.
- The board pin map calls PD2 the reset pin. This review did not confirm that signal in the firmware.
- The [Hertzz58 project](https://github.com/Hertzz58/Radtel-RT950-Pro-Firmware) reports an ST7789V panel. This review did not read a panel ID.

## Pixel transfer

The checked V0.27 transfer loop uses software writes to GPIOD. It does not start a DMA transfer in that loop. A call at `0x0802994C` sends command `0x2C` before pixel data.

The checked loop reads bytes from `0x20000C50` and limits one transfer to `0x9600` bytes. Its coordinate limits include `0x140` (320) and `0xF0` (240). These limits do not establish the visible screen rotation. The board project reports a 240 by 320 panel.

A full 240 by 320 RGB565 frame needs 153,600 bytes. The AT32F403A xG linker describes 96 KB of SRAM. Thus, a full frame cannot fit in that SRAM. Use a partial buffer for new display code.

Older notes named LCD functions near `0x080267B8` and a DMA frame buffer at `0x20000BD0`. In this V0.27 import, Ghidra reads floating-point code at the published function address. The checked display writes are near `0x080297B8`. Keep the older notes in [pinout-legacy.md](pinout-legacy.md) for reference.

## Open checks

- Read the panel ID on a radio before selecting a controller driver.
- Check PD2 reset with a scope or a firmware trace.
- Measure the visible rotation and the write timing.
