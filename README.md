# Radtel RT-950 Pro Reverse Engineering Workspace

This repository contains hardware notes, firmware data, and tools for the Radtel RT-950 Pro radio. The work helps document the radio and supports new firmware development.

## Hardware Data

- [Pinout notes](docs/pinout.md) separate V0.27 firmware checks from reported board tests. The [earlier pin list](docs/pinout-legacy.md) remains available.
- [Pinout slides](docs/pinout.pptx) show the pin data in a visual format.
- [RT950Pro RF schematic](docs/RT950Pro.pdf) shows RF circuits and signal names.
- [Display notes](docs/display.md), [SPI flash notes](docs/spi_flash.md), and [audio notes](docs/audio_tones.md) record findings from firmware analysis.
- [V0.27 analysis](docs/rt950-v027-analysis.md) records image checks, pin evidence, and open questions. The [evidence directory](docs/handover-evidence/README.md) contains analysis data, selected decompiled C, and a Ghidra project.
- [CPS clone protocol notes](docs/cps-clone-protocol.md) record reported clone commands and keep them separate from the OEM firmware update protocol.
- [Screen photos](docs/screenshots/README.md) show the radio user interface during several operations.
- [AT32F403A data sheet](Datasheets/DS_AT32F403A_V2.04_EN.pdf) and [AT32F403A/407 reference manual](Datasheets/RM_AT32F403A_407_V2.07_EN.pdf) give MCU data.

## Firmware Data and Tools

- `firmware/decrypted.bin` is a decrypted OEM V0.18 image. See [firmware/README.md](firmware/README.md) for its source.
- `firmware/dissassembled_v3_mem_map.c` and `.h` contain output from firmware analysis. [Function_Names.csv](docs/Function_Names.csv) records function names.
- `firmware/RE/decrypted-firmware/` contains a Ghidra project. `firmware/RE/firmware dumps/` contains USB captures from the update process.
- [Bootloader notes](firmware/RE/bootloader/bootloader_protocol_notes.md) describe the update protocol. `bootloader/rt-950pro_bootloader.bin` contains a bootloader image.
- `firmware/reference firmware/` contains OEM update files, instructions, and code extracted from the updater.
- `firmware/scripts/` contains firmware and flash tools. `reference project/` and `artery_cortex-m4/` contain AT32 reference code and libraries.

## External References

- [Hertzz58's RT-950 Pro firmware project](https://github.com/Hertzz58/Radtel-RT950-Pro-Firmware) has bare-metal firmware, GPIO notes, and flash and CPS tools.
- [NathanBarguss's CHIRP RT-950 Pro project](https://github.com/NathanBarguss/Chirp_Radtel-RT-950-Pro) has a [draft CPS clone protocol report](https://github.com/NathanBarguss/Chirp_Radtel-RT-950-Pro/blob/main/RADIO_PROTOCOL_WIP.md). Our [protocol notes](docs/cps-clone-protocol.md) record the parts used here.
- [JKI757's RT-950 Pro repository](https://github.com/JKI757/radtel-950-pro) has earlier firmware analysis notes. Its [protocol discussion](https://github.com/JKI757/radtel-950-pro/issues/1) and [reported power-on modes](https://github.com/JKI757/radtel-950-pro/issues/2) are covered in the [V0.27 analysis](docs/rt950-v027-analysis.md). Some earlier pin and bus claims differ from the V0.27 checks.
- [nicsure's GPIO diagnostic post](https://www.patreon.com/nicsure/posts/nicfw950-gpio-143205329) reports pin names used in our [pinout notes](docs/pinout.md).

## Next Work

- Check uncertain pin assignments on the hardware. Update the [pinout notes](docs/pinout.md) with the results.
- Compare the RF schematic with the firmware signal names.
- Add confirmed function names to [Function_Names.csv](docs/Function_Names.csv).

## Credits

Radtel and Artery supplied the OEM firmware and AT32 reference material. Hertzz58 maintains the separate firmware project linked above.
