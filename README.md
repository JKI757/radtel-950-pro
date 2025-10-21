# Radtel 950 Pro Reverse Engineering Workspace

This repository collects notes, tooling, and extracted firmware artifacts for the Radtel 950 Pro dual-band mobile radio. The short-term goal is to document the hardware and OEM firmware behaviour; the long-term goal is to enable an open replacement firmware.

## Project Status

- **MCU**: Artery AT32F403A (Cortex-M4F). Reference BSP, linker scripts, and peripheral headers are archived under `reference project/` and `artery_cortex-m4/`.
- **OEM Firmware**: Decrypted flash images and Ghidra projects live in `firmware/`. Decompiled outputs (e.g. `dissassembled_v3_mem_map.c`) are the primary source material for renaming work.
- **Peripherals**:
  - BK4829 RF transceivers: One on hardware SPI1, the second via bit-banged SPI on GPIOA.
  - SI4732 broadcast receiver: bit-banged I2C on PA8/PA9.
  - Display: 320×240 TFT on a GPIOD[15:8] 8080 bus, driven through custom routines (`LCD_WriteCommand`, `LCD_WriteData`, etc.).
  - Keypad, encoder, PTT relays, band relays, and audio front-end have been mapped in `docs/pinout.md` and `docs/display.md`.
- **Function Catalogue**: Human-readable names and annotations are tracked in `docs/Function_Names.csv` and `firmware/RE/gemini_remap.csv`.

## Repository Layout

```
.
├── docs/                     # Markdown notes, pinout tables, function catalogue
├── firmware/                 # Decompiled sources, decrypted binaries, Ghidra project
├── reference project/        # Vendor AT32 reference firmware project
├── artery_cortex-m4/         # AT32 BSP / SDK manuals
```


## Hardware Notes

- **MCU Clocks & Memory**: Refer to `reference project/radtel/AT32F403AxG_FLASH.ld` for flash/RAM layout. Stack/heap sizes match the OEM firmware.
- **Display**: TFT responds to DCS commands 0x2A/0x2B/0x2C; pixel data is RGB565 sourced from 0x20000BD0. See `docs/display.md` for timing.
- **RF Chain**: BK4829 configuration uses a mixture of 4k/32k/64k SPI flash erase commands (`Software_SPI_FlashErase*`), so pay attention before modifying channel memories.

## Toward Custom Firmware

1. **Toolchain**: The reference project builds with Arm GCC; confirm version alignment (the AT32 SDK typically targets GCC 10.x). A future custom firmware can either start from the vendor BSP or from an open stack (e.g. OpenRTX).
2. **Boot / Flash Process**: Map the bootloader entry points before flashing. OEM firmware performs signature checks; ensure the bootloader accepts unsigned images or prepare a bypass.
3. **Drivers to Implement**:
   - GPIO, timers, ADC/DAC: Already documented—can be ported directly.
   - Display: Implement an 8080 parallel driver using the routines identified in `docs/display.md`.
   - RF: Reverse or replace BK4829 register sequences. Current init blocks reside at `FUN_80007f04` (hardware SPI) and `Software_SPI_Write_Block` flows.

## Contributing / Next Steps

- Fill in missing function names and upload diffs to `docs/Function_Names.csv`.
- Capture logic traces for the display and SPI flash to confirm controller IDs.
- Document the bootloader and flash layout boundaries.
- Once core drivers are ready, scaffold a minimal blinking firmware and LCD test pattern before attempting full-feature firmware.

## Credits

- OEM firmware and reference BSP belong to Radtel / Artery. All RE work here is for educational and interoperability purposes.

