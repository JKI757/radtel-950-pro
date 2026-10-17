# RT-950 Pro V0.27 Firmware Analysis

This note records checks on the OEM V0.27 application. It separates values read from the image, Ghidra results, and pin names reported by other work. It does not replace a test on the radio.

## Image and method

- Image: `firmware/radtel-rt950-pro-firmware/binary/RT_950Pro_V0.27_260203/RT_950Pro_V0.27_decrypted.bin` in the separate [Hertzz58 project](https://github.com/Hertzz58/Radtel-RT950-Pro-Firmware). The local copy is ignored by this repository.
- Size: 380,316 bytes. SHA-256: `fc223cfd2fa5456b8c8858e32d4cc193e91e383c74f2bd2e7725a3ff87993e91`.
- Load address: `0x08003000`. The image has `RT-950      ` at file offset `0x3E0`. The initial stack pointer is `0x20015D10`; the reset vector is `0x080032A1`.
- Check made here: Ghidra 11.3 imported the raw image as `ARM:LE:32:Cortex` at `0x08003000`. We decompiled the functions listed below and checked their literal values against the image bytes.
- [Original handover](handover-evidence/source-handover.md) and [analysis files](handover-evidence/README.md) give more detail. The original report used Ghidra 12.1.4. Its paths and development plans need review before use.

The GPIO helpers are `0x080155B2` (set pin, register offset `+0x10`), `0x080155AE` (clear pin, `+0x14`), and `0x0801559A` (read input, `+0x08`). The port bases are GPIOA `0x40010800`, GPIOB `0x40010C00`, GPIOC `0x40011000`, GPIOD `0x40011400`, and GPIOE `0x40011800`.

## Bootloader check

The 12,288-byte bootloader in `bootloader/rt-950pro_bootloader.bin` has SHA-256 `109e599a17f65035e5c8c42d436452f3dce393cbbb51652adb3dd05a59e7b1e0`. Ghidra imported it at `0x08000000`. Its function at `0x0800049C` reads GPIOA mask `0x1000` (PA12) and GPIOE mask `0x0020` (PE5). Both reads must be low twice before it enters update mode. The bootloader contains `RT-950      ` at file offsets `0x3A4` and `0x698`.

This check identifies the update-button pins. It does not settle every application signature check or prove a recovery procedure. The [original handover](handover-evidence/source-handover.md) lists those open questions.

[JKI757 issue #1](https://github.com/JKI757/radtel-950-pro/issues/1) questioned a Ghidra project that loaded flash at `0x80000000`. The V0.27 image check above sets the application base to `0x08003000`; the bootloader has its own base at `0x08000000`. The issue also asks whether the CPS image-upload `W` command has an `R` read command. The [CHIRP clone protocol report](cps-clone-protocol.md) documents an `R` command for clone data, but does not answer the image-upload question. The issue reports image upload at offset `0x90000`, but does not establish which memory device holds that data.

[JKI757 issue #2](https://github.com/JKI757/radtel-950-pro/issues/2) reports four power-on key combinations: PTT with OK for factory mode, PTT with 8 for super mode, PTT with 2 for normal mode, and Down for an all-band test mode. The reporter did not establish what these modes do. These are user reports; we did not test them or map the keys to MCU pins. In particular, the PA12 and PE5 bootloader check above does not prove that any of these combinations enters update mode.

## Pin and bus results

| Function or circuit | Result from V0.27 | Evidence | Limit |
| --- | --- | --- | --- |
| Two BK4829 chips | PE10 clock, PE11 data, PE8 chip 0 select, PE15 chip 1 select. The code shifts bits through GPIOE. | `0x0801E260`, `0x0801F090`, `0x0801F0E4`; literal `0x40011800`. | The chip names come from the project pin map. The bus pins and two select masks are in the code. |
| External SPI flash | PB12 select and SPI2 base `0x40003800`. | `0x08017000`; literals at `0x0801705C` and `0x08017060`. | PB13 to PB15 are the reported SPI2 alternate pins. We did not trace each one here. |
| GPS serial port | USART3 base `0x40004800` and GPIOB mask `0x0400` (PB10). The setup value for baud is `0x2580` (9600). | `0x08016B20`; literals at `0x08016BA4` and `0x08016BA8`. | PB11 as the other serial pin is reported by the project pin map. |
| Bluetooth serial port | USART1 base `0x40013800` and GPIOA mask `0x0200` (PA9). | Literals at `0x0800A578` and `0x0800A57C`. | PA10 as the other serial pin is reported by the project pin map. |
| Programming port | UART4 base `0x40004C00` and GPIOC masks for PC10 and PC11. | `0x08025738`; literals at `0x080257A4` and `0x080257A8`. | This check does not prove the connector wiring. |
| Backlight | PC6 and PB3 change together. | `0x08017C40`; literals at `0x08017C84` and `0x08017C88`. | Their electrical roles still need a board check. |
| RF path relays | PE13, PE12, PE14, PE7, PB0, and PB1 are driven by one routine. | `0x0801D268`; literals at `0x0801D3E8` and `0x0801D3EC`. | A call site or a board measurement is needed for safe idle polarity. |
| Display | Data uses PD8 to PD15. PD0 is the write strobe, PD1 is chip select, and PD3 selects command or data. | `0x080297B8`, `0x08029818`, and the transfer loop reached at `0x08029714`; GPIOD literals at `0x08029764`, `0x08029814`, and `0x08029874`. | The code does not identify the panel controller. PD2 reset was not checked here. |

An aligned word scan of this image found no SPI1 base `0x40013000`. It found three SPI2 base words. This supports the SPI2 flash result. It does not prove that SPI1 can never be used by another image.

The V0.27 display transfer loop uses byte writes to GPIOD and limits one transfer to `0x9600` bytes. It reads from `0x20000C50` in the checked function. A full 240 by 320 RGB565 frame needs 153,600 bytes; the stated MCU SRAM is 98,304 bytes. The earlier full-frame DMA description does not fit this image. DMA2 appears elsewhere in the firmware.

## Earlier pin map checks

The [earlier pin list](pinout-legacy.md) has useful board labels. Its display control pins PD0, PD1, PD3 and data bus PD8 to PD15 agree with this Ghidra check. Its PB12 flash select, PC6 backlight, and GPIOE relay pins also agree.

Other parts of the earlier notes conflict with V0.27:

- BK4829 on SPI1 or GPIOA: the checked V0.27 functions use GPIOE.
- Flash on a GPIOA bit-bang bus: the checked V0.27 flash setup uses SPI2 and PB12 select.
- GPS on PC10 and PC11: the checked GPS setup uses USART3 and PB10. PC10 and PC11 appear in UART4 setup.
- PC12 as the display backlight: the checked backlight routine drives PC6 and PB3.
- The published LCD writer addresses near `0x080267B8`: Ghidra reads floating-point code there. The display writes checked here are near `0x080297B8`.

The project's hardware scan reports a keypad matrix on PC0 to PC3 and PD4 to PD7, encoder inputs on PB4 and PB5, and PTT inputs on PE2 and PE3. This review did not repeat those board tests. The prior address `0x08012FF8` did not decompile as a keypad scanner in our Ghidra import. The reported SI4732 pins PB6 and PB7, the display reset on PD2, and the battery and VOX ADC pin roles also need a check.

## External flash and other open work

The Hertzz58 project describes 990 channel records, VFO data, settings, fonts, and calibration in external flash. Its headers disagree on some record positions. Read one channel and compare it with a known CPS value before defining a data structure. The project places factory calibration near `0x00F000`; keep that area unchanged during experiments.

The panel ID, display reset, SI4732 pins, ADC input roles, and safe RF relay idle levels remain open. The imported Ghidra project and decompiled C provide data for further checks. They are analysis data from the OEM image, not new firmware source.

[nicsure's GPIO diagnostic post](https://www.patreon.com/nicsure/posts/nicfw950-gpio-143205329) reports pin roles from another firmware experiment. The [current pinout](pinout.md) records the useful agreements and the names that still conflict with V0.27 analysis. Its diagnostic binary is kept only in the ignored local `firmware/embargo/` directory.

## REA result

REA 6.5.0 is installed in a temporary directory. An earlier test found that its ordinary artifact check rejected this headerless image and that Ghidra 11.3 was too old for its Ghidra provider.

On 2026-10-17, a new check found Ghidra 12.1.4 at `/Applications/ghidra_12.1.4_PUBLIC`. REA accepted its version, headless launcher, and Java runtime. Its provider check still failed because this Ghidra installation has no executable native decompiler for macOS arm64. A raw-image `rea inspect` call stopped at that check. REA's [raw binary guide](https://github.com/morluto/rea/blob/main/docs/raw-binary.md) also lists Cortex-M and Thumb-only targets as unsupported. Adding the [native decompiler](https://github.com/morluto/rea/blob/main/docs/installation.md) would resolve one setup fault, but would not make this raw Cortex-M image a supported REA target. REA supplied no new pin evidence. Direct Ghidra analysis remains available.
