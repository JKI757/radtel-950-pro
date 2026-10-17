# RT-950 Pro → OpenRTX handover

Audience: a coding model that has not seen this session. Build a new OpenRTX target for the Radtel RT-950 Pro with an LVGL UI. Do not continue the old `RADTEL-950P` tree, and do not treat the September 2025 pin map as hardware truth.

This document separates **observed** (bytes, Ghidra, Capstone), **inferred**, and **claimed by an existing tree but not re-proven here**.

## Decision

1. Start from a fresh OpenRTX checkout at or after **v0.4.4** (`github.com/OpenRTX/OpenRTX`, `master`). Do not rebase `openrtx_radtel`.
2. Target the Artery **AT32F403A xG**: 1024 KB flash, **96 KB SRAM**, Cortex-M4F. It is not an STM32F405 and it does not have 320 KB SRAM.
3. Use the OEM image as a hardware spec, not as source to paste into OpenRTX.
4. Hertzz58's GPL-3 tree is the best written driver sketch and the only hardware-bring-up log. Reuse it where this document marks a fact verified. Several of its function addresses are wrong.
5. The CS7000 LVGL tree is a 160×128 visual spike. Reuse the control-event contract and the partial-buffer idea. Do not port that screen size or pretend it is wired to the radio FSM.

## Do not use

| Source | Why |
| --- | --- |
| `openrtx_radtel/OpenRTX/platform/targets/RADTEL-950P/pinmap.h` | Built from the old notes. Puts BK4829 on SPI1/PA5/PA7, flash on GPIOA, GPS on PC10/PC11, LCD reset on PC11, backlight on PC12, keypad scan-enable on PE5. Those assignments conflict with the V0.27 image. `PD4` is both LCD read and a keypad row. `PA12` is both the keypad latch and USB D+, and it is also a bootloader-entry pin. |
| `build_arm/openrtx_radtel_950p` | An ARM ELF was linked on 2025-11-11. Meson loads it at `0x08000000`, which is the OEM bootloader. Do not flash it. |
| `platform/mcu/STM32F4xx/stm32_radtel_1m+320k.ld` | Invents a 64 KB CCM plus 192 KB SRAM and a framebuffer section. Vendor xG linker is 96 KB at `0x20000000` only. A 320×240 RGB565 frame is 153,600 bytes and does not fit. |
| `docs/pinout.md`, `docs/display.md`, `docs/spi_flash.md` | Early notes. They still describe SPI1 + GPIOA bit-bang and a DMA framebuffer. Superseded by the V0.27 evidence below. |
| Hertzz58 `include/rt950_pinmap.h` comments that name `LCD_WriteCmd @ 0x080267B8` and `LCD_Init @ 0x08026954` | At load base `0x08003000` those addresses are floating-point code, not an LCD driver. |
| `firmware/RE/firmware dumps/fullflash_reconstructed.bin` | 386,048 bytes. Not a full-chip dump. Vector table matches the V0.21 shape, not V0.27. |
| `docs/handover-evidence/v027-key.c` as a whole | Ghidra pseudocode of selected addresses. Some requested addresses fell inside the wrong function. Use only the functions called out below. |

## Evidence method

| Tool | Result |
| --- | --- |
| REA `inspect_firmware_regions` | Refused. Needs Linux and `REA_BINWALK_COMMAND`. This image is a flat Cortex-M blob, not a container. Binwalk was not required. |
| REA `open_binary` on `decrypted.bin` | Refused: unsupported binary format. REA has no raw Cortex-M target format. |
| Ghidra 12.1.4 headless, `ARM:LE:32:Cortex`, `BinaryLoader`, base `0x08003000` | Imported V0.27. **643 functions, 487 strings.** Project: `/Users/minerva/rt950-ghidra` (also copied under `docs/handover-evidence/`). `JAVA_HOME` must be `/opt/homebrew/opt/openjdk@21`. Ghidra rejects a project path that contains a dot-component, so it cannot live under `~/.hermes`. |
| Capstone 5.0.7 Thumb-2 | Used to prove the load address, resolve literal pools, and check addresses Ghidra grouped badly. |
| Bootloader | Same Ghidra project, base `0x08000000`, plus Capstone on the button-check literals. |

Nothing here was executed on a radio. Static analysis did not observe TX, RX, or a panel ID read.

## Image identity

App images are linked to be programmed at **`0x08003000`**, not `0x08000000`. Proof: file offset `0x3E0` is the ASCII signature `RT-950      `. The bootloader compares that signature at flash `0x080033E0`. `0x080033E0 - 0x3E0 = 0x08003000`. Under that base, the reset vector `0x080032A1` is a Thumb trampoline at file offset `0x2A0`. Loading the same file at `0x08000000` turns the real GPIO helper into garbage and must not be used. Hertzz58's pinmap comment "Load base: 0x08000000" is wrong; their absolute function addresses that survived checking were still correct because they are flash addresses, not file offsets.

| Image | Size | SHA-256 | Vector[0] SP | Vector[1] Reset |
| --- | --- | --- | --- | --- |
| V0.27 decrypted (analyze this) | 380,316 | `fc223cfd2fa5456b8c8858e32d4cc193e91e383c74f2bd2e7725a3ff87993e91` | `0x20015D10` | `0x080032A1` |
| V0.18 decrypted (`firmware/decrypted.bin`) | 381,736 | `b619f1984d8b767bcc710fd1546a94f3a85cbdf7724960282a34f3c46775a48a` | `0x20017BB0` | `0x080032A1` |
| OEM bootloader `rt950pro_bootloader.bin` | 12,288 | `109e599a17f65035e5c8c42d436452f3dce393cbbb51652adb3dd05a59e7b1e0` | `0x200008B8` | `0x08000191` |

V0.27 path:

`firmware/Radtel-RT950-Pro-Firmware -- gpl firmware project from hertzz58/binary/RT_950Pro_V0.27_260203/RT_950Pro_V0.27_decrypted.bin`

Ghidra image span: `0x08003000`–`0x0805FD9B`. Code functions are concentrated in `0x08003184`–`0x0802BA88`. Strings continue to about `0x0805EE3C`. The rest of the 1 MB flash is not in this file.

BTF files are the update container. A version-specific key lives at file offset `0x400`. Decrypt with Hertzz58 `tools/encrypt_btf.py`. Do not copy key bytes into OpenRTX.

## Memory map

Observed from the Artery xG linker in `reference project/radtel/AT32F403AxG_FLASH.ld` and from the images:

```
0x08000000  OEM bootloader, 12 KB, permanent
0x08003000  Application vector table (first word must be a SRAM stack pointer)
0x080033E0  12-byte model signature "RT-950      "  (file offset 0x3E0)
0x0805FD9C  First byte after the V0.27 app image (0x08003000 + 380316)
0x08100000  End of 1024 KB internal flash

0x20000000  SRAM start
0x20015D10  V0.27 initial SP
0x20017BB0  V0.18 initial SP; Hertzz58 linker _estack
0x20018000  End of the 96 KB SRAM the vendor linker describes

0x08400000  SPIM window in the vendor linker (16 MB). Not shown used by V0.27.
```

Boot contract, mixed evidence:

- **Observed:** bootloader strings at `0x080003A4` and `0x08000698` are `RT-950      `. App file+`0x3E0` is the same 12 bytes. Bootloader also contains `UPDATE`, ` Upgrading... `, ` Update Error! `, `Update Success!`, `Firmware Error!`, `Ver 1.0`.
- **Observed:** update-button check at `0x0800049C` loads `0x40010800` (GPIOA) with mask `0x1000` (PA12) and `0x40011800` (GPIOE) with mask `0x0020` (PE5). Both reads must be zero or the function skips update mode. It then calls `0x08001588` and repeats both reads. Hertzz58 `docs/bootloader.md` mislabels `0x40010800` as GPIOB. The address is GPIOA pin 12.
- **Observed in the bootloader pool:** literal `0x00300700`, which matches the SPI version-check address their bootloader note describes. That does not by itself prove the whole SPI-update state machine.
- **Claimed, and contradicted inside that same tree:** README and one section of `docs/bootloader.md` say the only app check is `(sp & 0x2FFE0000) == 0x20000000`. The linker and `startup.c` also plant a second `"RT-950      "` at file+`0x7E0` and say a failed check there erases the app. V0.27's SP `0x20015D10` would pass the mask. Confirm both checks in Ghidra program `bootloader.bin` before flashing a custom image that omits the second signature.
- **Claimed, required by their `main`:** hold PB9 high immediately or the radio drops power. Feed the independent watchdog. Turn JTAG off and leave SWD on, or PB3 (backlight) and PB4 (encoder) stay in the debug mux.
- **Clock contradiction:** `system.c` programs 8 MHz HEXT × 15 = 120 MHz. The Makefile comment says 240 MHz. Use 120 MHz until a scope says otherwise.
- **Claimed:** UART update is UART4 at 115200, and the host handshake string `PROGRAMBT9000U` is required. The string is **observed** in the app at `0x0802FB4E` next to `UPDATE` at `0x0802FB47`. Baud was not re-derived.
- Custom firmware must keep the model signature at file offset `0x3E0`. Hertzz58 also places a second copy at file offset `0x7E0` because their upload path does not write the BTF key block into flash. The OEM V0.27 image does **not** have `RT-950` at `0x7E0`. Follow the Hertzz58 linker comments if you boot through their upload tool; do not assume the OEM image layout and the custom-firmware layout are the same after byte `0x400`.

Internal flash sector size is claimed as 2 KB. Not re-measured here.

## Clock and MCU

Claimed by Hertzz58 and the vendor SDK, not re-derived from the PLL registers in this pass: AT32F403A Cortex-M4F, 120 MHz, hardware FPU. The bootloader note says the bootloader enables CP10/CP11 and writes VTOR `0x08000000`. An app must set VTOR to `0x08003000` or interrupts will still fetch the bootloader table.

Vendor linker and headers: `reference project/radtel/` and `artery_cortex-m4/`. Datasheet and reference manual: `Datasheets/DS_AT32F403A_V2.04_EN.pdf`, `Datasheets/RM_AT32F403A_407_V2.07_EN.pdf`.

## Vector table (V0.27, observed)

Default peripheral handler is `0x080032BB` (`b .` stubs). Unique handlers:

| Index | Name | Address |
| --- | --- | --- |
| 1 | Reset | `0x080032A1` |
| 2 | NMI | `0x0801B2F5` |
| 3 | HardFault | `0x08016BC9` |
| 4 | MemManage | `0x08019889` |
| 5 | BusFault | `0x0800A29F` |
| 6 | UsageFault | `0x080252D5` |
| 11 | SVCall | `0x0801F0A7` |
| 12 | DebugMon | `0x0800D545` |
| 14 | PendSV | `0x0801BD11` |
| 15 | SysTick | `0x080241E1` |
| 27 | IRQ11 DMA1 channel 1 | `0x0800D191` |
| 53 | IRQ37 USART1 | `0x08024EBD` |
| 54 | IRQ38 USART2 | `0x08024EC1` |
| 55 | IRQ39 USART3 | `0x08024EC3` |
| 68 | IRQ52 UART4 | `0x08024E95` |
| 74 | IRQ58 DMA2 channel 3 | `0x0800D1D5` |

USART2/USART3 entries point a few bytes after the USART1 entry, inside one instruction stream. Do not treat that as a USART2 driver. An aligned-word scan found **one** literal equal to USART2 base `0x40004400` and **zero** literals for SPI1, ADC1, I2C1, I2C2, AFIO base, or AFIO MAPR.

Hertzz58 `startup.c` only mentions DMA1_CH1 and DMA2_CH3. The UART IRQs are also populated. DMA1_CH1 and DMA2_CH3 addresses match their comments.

## GPIO helpers (observed)

These match only at base `0x08003000`:

| Address | Ghidra | Behavior |
| --- | --- | --- |
| `0x080155B2` | `FUN_080155b2` | `*(port + 0x10) = mask` — STM32/AT32 BSRR, pin goes high |
| `0x080155AE` | `FUN_080155ae` | `*(port + 0x14) = mask` — BRR, pin goes low |
| `0x0801559A` | `FUN_0801559a` | `(IDR & mask) != 0` |

AT32 GPIO bases, standard and used as literals:

| Port | Base |
| --- | --- |
| GPIOA | `0x40010800` |
| GPIOB | `0x40010C00` |
| GPIOC | `0x40011000` |
| GPIOD | `0x40011400` |
| GPIOE | `0x40011800` |

## Pins and peripherals verified from V0.27

"Verified" means a literal pool or a decompiled store used that base and mask. It does not mean the silkscreen name was checked on a board.

### Dual BK4829 — GPIOE bit-bang, not SPI1

| Function | Address | What it does |
| --- | --- | --- |
| Shift byte | `0x0801E260` | Port `GPIOE`. Clock mask `0x0400` (PE10). Data mask `0x0800` (PE11). 8 bits, MSB first, delay call between edges. |
| Chip 0 write | `0x0801F090` | Asserts PE8 (`0x0100`) low, shifts three bytes, releases PE8, then clears PE10. |
| Chip 1 write | `0x0801F0E4` | Same sequence with PE15 (`0x8000`). |

SPI1 base `0x40013000` occurs **zero** times as an aligned literal. The old "BK4829 on SPI1" note is false for V0.27.

This is the bus contract an OpenRTX driver needs. Do not copy a register-programming sequence out of the OEM image into a transmitter until the user is on a dummy load and the legal band limits are explicit. Register maps belong in the BK4829 datasheet under `sample_code/BK4829_EMBARGOED` and in Hertzz58 `include/drivers/bk4829.h`.

### SPI flash — SPI2, CS on PB12

`0x08017000` sets GPIOB mask `0x1000` (PB12) through the set-pin helper, then later code references SPI2 `0x40003800` (literal site `0x08017046`). Hertzz58 says PB13/PB14/PB15 are the AF pins, mode 3, software NSS. The CS pin and the SPI2 base are observed; the AF pin numbers are their claim plus the AT32 alternate-function map.

Erase/read opcodes in their header (`0x06`, `0x03`, `0x02`, `0x52`, `0x05`, `0x35`) were not re-disassembled in this pass. Their older local note that said the flash shared PA5/PA6/PA7 with a second BK4829 is wrong for V0.27.

### GPS — USART3, 9600, PB10

`FUN_08016b20` at `0x08016B20` configures GPIOB mask `0x0400` (PB10) and USART3 `0x40004800` with a baud field of `0x2580` (9600). PB11 as RX is the usual USART3 pair and is their claim; this decompile showed the TX-side mask clearly.

### Bluetooth — USART1 on GPIOA

A literal pool at `0x0800A578` / `0x0800A57C` is GPIOA and USART1 `0x40013800`. Nearby code stores mask `0x0200` (PA9). PA10 as RX is their claim. String `bluetooth open!` is at `0x0801A240`. Menu strings `Bluetooth` / `BLUETOOTH` are in the string table. 115200 baud is their claim, not re-derived.

### CPS / programming — UART4 on GPIOC

Literals at `0x080257A4` and `0x080257A8` are GPIOC `0x40011000` and UART4 `0x40004C00`. Init fragment at `0x08025738` handles pin masks `0x0400` and `0x0800` (PC10 and PC11). IRQ52 points at `0x08024E95`. This is the programming port. PC10/PC11 are not the GPS port. Protocol write-up: Hertzz58 `docs/cps-uart.md` and `tools/cps_flash.py`. Do not invent a second programming protocol.

### Backlight — PC6 and PB3

`FUN_08017c40` at `0x08017C40`: nonzero argument sets GPIOC mask `0x0040` (PC6) and GPIOB mask `0x0008` (PB3); zero clears both. Hertzz58's hardware note matches this. The old "backlight = PC12" assignment does not.

### Relay / RF path — GPIOE + GPIOB

`FUN_0801d268` at `0x0801D268` takes two integers and drives:

| Mask | Pin | Port |
| --- | --- | --- |
| `0x2000` | PE13 | GPIOE |
| `0x1000` | PE12 | GPIOE |
| `0x4000` | PE14 | GPIOE |
| `0x0080` | PE7 | GPIOE |
| `0x0001` | PB0 | GPIOB |
| `0x0002` | PB1 | GPIOB |

**TX safety:** Hertzz58 says these relays are active-low and that the idle state is driven high; driving them the wrong way can bias the PA. This decompile shows both set and clear calls, including a fall-through that clears all six masks. Do not pick a polarity from that fall-through. Before any TX test, find the call site used for RX idle, measure the pins, and keep PA enable off until the user confirms a dummy load. Do not key the transmitter from an LVGL callback.

### ADC2, DAC, DMA, timers

| Peripheral | Evidence |
| --- | --- |
| ADC2 `0x40012800` | Literals at `0x08016858` and `0x08016894`. `FUN_0801685c` returns `(sample & 0xFFF) >> 4`. ADC1 base occurs zero times. |
| DAC `0x40007400` | Literal hits from `0x0800D50A`. Ghidra also has xrefs from `0x0800D512`. |
| TIM6 `0x40001000` | Literals at `0x0801BECC`, `0x0800680C`. |
| DMA2 channel 3 `0x40020430` | Literals at `0x0801BF02` (next to the DAC init region), `0x080100B0`, `0x080068EE`. IRQ58 enters `0x0800D1D5`. |
| DMA1 `0x40020008` | IFCR-area literals. IRQ11 enters `0x0800D191`. |
| RCC `0x40021000` | Clock setup cluster around `0x0801CD52`. |
| Flash controller `0x40022000` | Cluster around `0x080116E2`. |

Which ADC channel is battery versus VOX is **unresolved**. Hertzz58's own header says the silkscreen and the firmware disagree (PA0 vs PA1). Measure before displaying a battery icon as truth.

### LCD data bus — GPIOD bits 8–15, but not at the published addresses

Capstone found `bfi Rt, Rn, #8, #24` (insert a byte into bits 8–15) at:

- `0x08029714`
- `0x080297DC`
- `0x0802983C`

The same region has GPIOD literals (`0x080296CC`, `0x080297BC`, `0x0802981C`). That is the 8-bit 8080 data bus on PD8–PD15. Hertzz58's published writer addresses `0x080266C4`, `0x080267B8`, and `0x08026954` disassemble as VFP/libm code. Re-find WR/CS/DC from the `0x080297xx` functions in the Ghidra project before writing the display driver. Their hardware log says the panel is an ST7789V, 240×320, software bit-bang, no LCD DMA. Panel ID was not read in this session. Old notes said 320×240 ILI93xx and a DMA2 framebuffer at `0x20000BD0`. The framebuffer address cannot hold a full RGB565 frame in 96 KB SRAM. Use a row or tile buffer.

### Keypad and encoder — partial

- Encoder function region `0x08010710` is real code (push, compare a byte). Hertzz58 says PB4/PB5 quadrature, polled, not TIM3. That pin pair was not re-proven from a literal in this pass. Their April 2026 GPIO scan is the hardware source for PB4/PB5.
- Address `0x08012FF8`, which their header calls the keypad scanner, is in the middle of `FUN_0800b1fc`, and that function writes SPI-looking addresses `0xE000` / `0xE020`. Do not treat `0x08012FF8` as the scanner entry. The matrix they hardware-confirmed (columns PC0–PC3, rows PD4–PD7, implicit fifth column) is a hardware claim. PC4 is their band-relay output; do not toggle it while scanning.
- PA12 and PE5 are the bootloader update buttons (observed) and their top/bottom program keys (claimed). A new UI must not require those two keys to be the only way out of a crash, because holding both at power-up enters the OEM bootloader.

### Peripheral absence (aligned literals in V0.27)

Zero hits: SPI1 `0x40013000`, ADC1 `0x40012400`, I2C1 `0x40005400`, I2C2 `0x40005800`, AFIO `0x40010000`, AFIO_MAPR `0x40010004`. SI4732 I2C is therefore bit-banged if present. Hertzz58 says PB6/PB7, 7-bit address `0x11`. That was not re-proven here. One USART2 literal is not a driver.

## External flash map

Source: Hertzz58 `include/drivers/flash_layout.h`, which says it was checked against V0.27 and a KDH server JSON. This session did not re-decode the records. Treat the map as the working hypothesis and verify one channel record against a CPS read before writing a codeplug.

| Region | Address | Claim |
| --- | --- | --- |
| 990 channels × 32 bytes | `0x000000` | Packed-BCD frequencies, 12-byte name at offset 20 |
| VFO A/B/C | `0x008000` | Different digit encoding from channel records |
| Settings | `0x009000` | 80 bytes across 5 blocks |
| Calibration | `0x00F000` | Factory, not CPS-visible, about 0x300 bytes |
| Fonts | `0x15C000` and up | ASCII/CJK glyphs used by the OEM UI |
| Splash | `0x090000` | 15360-byte blocks |
| SPI firmware update header | `0x300000` / `0x300700` | Bootloader note; `0x00300700` literal observed |

Chip identity is not settled. Headers say Winbond W25Q16. Their `main.c` accepts only JEDEC `0xEF40xx`. The changelog recorded Macronix `0xC22016` on a real radio. Read the ID before writing a driver.

Do not freeze a channel struct from this tree. Three writeups disagree, and the running firmware does not use `flash_layout.h` for VFO persistence:

| Source | Channels | VFO / settings | Zones |
| --- | --- | --- | --- |
| `include/drivers/flash_layout.h` | 990 × 32 at `0x000000`, name at offset 20, BCD little-endian digits | 3 × 32 at `0x008000`; settings at `0x009000` | 10 names at `0x00C000` |
| `include/drivers/spi_flash.h` + wear-leveling code | 990 × 32, end `0x008000` | 96-byte wear-leveled slots at `0x009000` (same address as settings in the other header) | 16 names at `0x00A200` |
| `docs/kdh-server.md` | name at bytes 16–27 | server map | different again |

`vfo.c` saves the wear-leveled 96-byte records, not the 32-byte image at `0x008000`. Dump one radio with `tools/cps_flash.py` before defining a codeplug struct. Do not erase `0x00F000`; both headers call that the factory calibration block.

## OEM feature strings

The app is a conventional analog handheld plus APRS, GPS, Bluetooth, NOAA weather, FM broadcast, CTCSS/DCS, VOX, zones, and a CPS/update path. There is no M17 or DMR string in the Ghidra string list. M17 would be new OpenRTX behavior on top of the BK4829 FM path, not a port of an OEM mode.

Useful anchors (address in the V0.27 image):

- `0x0802FB47` `UPDATE`
- `0x0802FB4E` `PROGRAMBT9000U`
- `0x0801A240` `bluetooth open!`
- `0x0801A6D3` `APRS activation of`
- `0x0805E804` `GPS SWITCH`
- `0x0805E810` `APRS SWITCH`
- `0x0805E828` `BLUETOOTH`
- `0x0805DEA1` `NOAA Alert`
- `0x0805E1F5` `VOX Switch`
- `0x0805DCFD` `CTCSS DCS`, plus nearby scan, zone, and frequency strings

Full string table: `docs/handover-evidence/v027-strings.tsv` (468 rows).

## What the three trees actually are

### Hertzz58 GPL firmware

- Path: `firmware/Radtel-RT950-Pro-Firmware -- gpl firmware project from hertzz58`
- Remote: `https://github.com/Hertzz58/Radtel-RT950-Pro-Firmware.git`
- HEAD: `b49804bafcf86271fdb8b20f9a32b7d30d5ea1be` (2026-04-06), "First working speaker output"
- License: GPL-3. Compatible with OpenRTX's GPL-3.0-or-later. Keep copyright notices if you copy driver code.
- Working tree is dirty against `origin/main` in file mode only (`100644` → `100755`, 0 lines changed). Content matches commit `b49804ba`. Do not treat the SMB copy as a source fork.
- README status is stale. CHANGELOG 0.0.2–0.0.3 is the hardware log: keypad, encoder, PTT pins, DAC speaker, and a Macronix JEDEC ID were confirmed. SI4732 init is still skipped in `main`. GPS was not in that confirmed list.
- The application is not "code complete." `radio_handle_key` has no caller, so the PTT task does not key the PA. `uart_acc_init` has no caller, so `cps.c` never sees bytes. Scanner, spectrum, APRS, crossband, VOX, NOAA, and Bluetooth poll functions are not scheduled.
- CPS conflict: `tools/cps_flash.py` and `docs/kdh-server.md` implement `PROGRAMBT9000U`, then raw `R`/`W` blocks and a 4-byte XOR key. `src/app/cps.c` and `docs/cps-uart.md` describe `0xA5` frames with CRC, and that code is unwired. Trust the Python tool, not `cps.c`.
- BK4829 role conflict: README says chip 0 is the main VFO. `vfo.c` assigns VFO A and C to chip 1 (PE15) and VFO B to chip 0 (PE8).
- `assembly/00_full_linear.asm` was disassembled with a base that makes its addresses `+0x3000` too high (`assembly/README.md`). Do not cite those addresses. Bootloader asm is based at `0x08000000` and is usable.
- Linker origin `0x08003000`, 96 KB SRAM, stack `0x20017BB0`. That matches V0.18's SP, not V0.27's `0x20015D10`. Either stack top is inside the 96 KB SRAM.
- Use this tree for: GPIO helpers, the verified pin table, flash-layout hypothesis, upload tool, and the test menu (`make test TEST=1..11`). Do not import it as the OpenRTX application layer. It is a bare-metal firmware with its own UI.

### Local OpenRTX Radtel fork — discard as a base

- Path: `openrtx_radtel/OpenRTX`
- Branch `jki757/radtel-950-pro` at `4b9f75fe81092947eaef586d304cf10662cbb6ff` (2025-09-22). That commit is also `origin/master` in this clone. It is a year behind current OpenRTX and behind v0.4.4.
- The clone is mode-dirty (`100644` → `100755` on hundreds of files) plus a small Meson edit. The Radtel target itself was never committed. Do not rebase it.
- A `radtel_950p` Meson target exists and is wired to `stm32f405_src`, with load address `0x08000000`. That address is the bootloader. The 2025-11-11 ELF under `build_arm/openrtx_radtel_950p` must not be flashed.
- Platform files exist (`hwconfig`, `pinmap.h`, `platform.c`, `bk4829.c`, `display_radtel_ili9341.cpp`, `keyboard_radtel.c`, `audio_radtel.cpp`). The pinmap is the wrong one, and several pads are aliased to two functions. The display driver is named ILI9341 and uses FSMC. The backlight code is the MD-3x0 PWM driver. `bk4829.c` is a single-chip bit-bang, not SPI1, but it is not the GPIOE bus verified above and it has no second chip. Read it only for public-datasheet register names.

### CS7000 LVGL spike — reuse the contract, not the panel

- Path: `Radio Projects/OpenRTX-CS7000-M17-Plus-LVGL/firmware/OpenRTX`
- Branch `cs7000p-lvgl-ui` at `a00655383787ab944438964d679fa4ce2ea252cd` (2026-03-21). Uncommitted: `lib/lvgl/`, `openrtx/ui/`, `.gitmodules`.
- LVGL **v9.6.0**, commit `80ca777e37a2b176770726a02e07a6fb79ef0b39` (`docs/LVGL-REVISION.md`).
- The plan file cites a different baseline (`37461958`). The checkout is `a006553`. Trust git, not that sentence.
- What exists: `openrtx/ui/lvgl/cs7000p_ui.c` (154 lines), a demo, a simulator, and a 16-row RGB565 partial buffer (5,120 bytes). README says it is **not** connected to the radio FSM and not flashable. PTT in the spike is a UI intent only.
- Upstream OpenRTX master is already at **v0.4.4** (QR, M17 packet TX/RX, internal-flash NVM driver, `AGENTS.md`). Pull that, then replay the LVGL spike. Do not fork March 2026.
- RT-950 UI needs a 240×320 (or 320×240 — confirm rotation on hardware) partial renderer and a 3-VFO model. The CS7000 plan's rule still holds: LVGL presents state; it does not own PTT, CPS writes, or RF sequencing.

## Build sequence for the next model

Do these in order. Do not start RF TX in the same change as the display.

1. **Fresh tree.** Clone OpenRTX at v0.4.4 or current `master` into a local APFS path, not the SMB share. SMB is an archive. Record the commit in the target README.
2. **Toolchain.** `arm-none-eabi-gcc`. Hertzz58 builds with GCC 12+. OpenRTX already has `cross_cm4.txt`. Add an AT32 target; do not compile it as STM32F405.
3. **Linker.** Flash origin `0x08003000`, length 1012 KB. SRAM origin `0x20000000`, length 96 KB. `_estack = 0x20017BB0`. Place `RT-950      ` at file offset `0x3E0` and the second copy at `0x7E0` if using the Hertzz58 uploader. Set VTOR to `0x08003000` in `SystemInit`.
4. **Boot test.** Build a firmware that sets VTOR to `0x08003000`, holds PB9 high, feeds the watchdog, disables JTAG while leaving SWD, and toggles PC6/PB3. Print on UART4. Do not flash `build_arm/openrtx_radtel_950p`. Upload with Hertzz58 `tools/firmware_upload.py` only after the user confirms the radio is the intended unit. Recovery image: `binary/RT_950Pro_V0.27_260203/RT_950Pro_V0.27_260203.BTF`. Enter the bootloader by holding PA12 and PE5 low at power-up. Keep both `"RT-950      "` signatures until the bootloader check is re-decompiled; the tree contradicts itself on whether the second one is enforced.
5. **Display.** Find WR, CS, DC, and reset from the `0x080297xx` BFI functions in the Ghidra project. Implement an 8-bit 8080 write and a partial RGB565 flush. Read panel ID (`0x04` / `0xD3`) once on hardware before locking the controller driver to ST7789. No full framebuffer.
6. **Inputs.** Matrix as Hertzz58 hardware-confirmed, then verify against a GPIO scan. Encoder PB4/PB5. PTT inputs are PE3 and PE2 in their header (hardware scan claim). Keep PTT out of LVGL.
7. **SPI flash read-only.** Read the JEDEC ID before assuming Winbond. Their changelog saw Macronix `0xC22016`. Then read one channel at `0x000000` and compare it to a `cps_flash.py` dump. Do not erase `0x00F000`. Do not adopt either in-tree flash header as the codeplug format until that dump exists.
8. **BK4829 identify.** Bit-bang on PE8/PE15/PE10/PE11. Read the chip ID. Do not program TX frequency or open the PA in this step.
9. **Audio path.** DAC on PA4, TIM6, DMA2 channel 3, after the ID read. Speaker mute and amp enable are still mostly claimed (PE1, PB8). Confirm before driving them.
10. **LVGL.** Vendor v9.6.0. Partial buffer. Presenter reads OpenRTX `state`. Three VFO slots, one TX path. Port the CS7000 control-event struct, not the 160×128 widgets.
11. **RX, then TX.** RX on a known amateur frequency first. TX only with the user's explicit go-ahead, a dummy load, and the relay polarity measured against RX idle.

## Open questions

- Panel controller ID and whether the visible orientation is 240×320 or 320×240.
- Exact WR/CS/DC/reset pins. Backlight is known. The data bus port is known. The control pins are not re-proven.
- ADC channel assignment for battery vs VOX.
- SI4732 pin and address. Claimed PB6/PB7, address `0x11`. Not re-proven.
- Encoder pins. Claimed and hardware-scanned by Hertzz58, not re-proven from a literal here.
- Keypad matrix entry point. The published address is wrong.
- Relay idle polarity. Pins are known. Safe level is not proven by the fall-through in `FUN_0801d268`.
- Whether SRAM above `0x20018000` exists on this package. Vendor xG linker says no. Do not use it.
- USART2's single literal. Ignore until a caller is found.
- OEM calibration record layout at `0x00F000`. Do not guess it.

## Artifact index

| Path | What it is |
| --- | --- |
| `/Users/minerva/rt950-ghidra` | Ghidra 12.1.4 project. Programs `v027.bin` at `0x08003000` and `bootloader.bin` at `0x08000000`. Open with `JAVA_HOME=/opt/homebrew/opt/openjdk@21`. |
| `docs/handover-evidence/v027-ghidra.json` | 643 functions, 487 strings, peripheral base xrefs. |
| `docs/handover-evidence/v027-strings.tsv` | Printable strings with image addresses. |
| `docs/handover-evidence/v027-key.c` | Selected decompilation. Trust only the functions named in this document. |
| `docs/handover-evidence/rt950.rep` | Copy of the Ghidra database. The `.gpr` file is empty in both copies; the local project is the one that was opened successfully. |
| Hertzz58 `docs/bootloader.md`, `docs/cps-uart.md`, `tools/README.md` | Protocol notes. Correct the GPIO base names against this document before using them. |
| `Radio Projects/OpenRTX-CS7000-M17-Plus-LVGL/docs/OpenRTX-LVGL-RT950-UI-PLAN.md` | UI architecture. RT-950 board bring-up was explicitly out of scope there. |
