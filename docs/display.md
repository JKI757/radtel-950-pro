Display Interface Notes
========================

Panel & Controller
------------------
- Interface style: 8080-style parallel TFT using MIPI-DCS commands (0x2A column, 0x2B row, 0x2C RAM write).
- Resolution: 320 x 240 pixels (limits in FUN_800156dc clamp to 0x140 x 0xF0).
- Pixel format: RGB565 (two bytes per pixel pushed from buffer at 0x20000BD0).
- Likely controller family: ILI93xx / ST77xx class (command set matches).

GPIO Mapping
------------
- PD8-PD15: Data bus (8-bit chunks shifted out via FUN_800271c0 / FUN_80027220).
- PD0: WR# (write strobe toggled inside FUN_800271c0 / FUN_80027220 loops).
- PD1: CS# (chip select driven around command/data bursts).
- PD3: D/C# (command = 0, data = 1).
- PD2: RD# (not toggled; tied high).
- PC12: Panel enable/backlight gate (asserted during FUN_800037b0 DMA bursts).
- PC14: Observed held low via FUN_80015794; likely panel reset.

Memory & DMA
------------
- Frame buffer: 0x20000BD0 (DAT_80015714) with 0x9600 bytes used per update.
- DMA2 base: 0x40020430 (DAT_8000396c) for streaming rows in FUN_800037b0.
- Command staging buffer: 0x2000A1D0 (DAT_80015410).

Key Firmware Routines
---------------------
- FUN_800037b0: High-level flush; prepares DMA2, asserts PC12, pushes frame buffer, releases PC12.
- FUN_800271c0 / FUN_80027220: Low-level GPIO write helper for command/data bytes.
- FUN_8001cb52: Sends 0x2A/0x2B window commands before pixel writes.
- FUN_800156dc / FUN_8002717c / FUN_80027300: Various blit helpers that fill rectangles/pixel runs using the low-level routines.
- FUN_8002700c: Generic command packet builder used for initialization sequences (invoked by backlight/power setups).

Workflow Summary
----------------
1. Window set: FUN_8001cb52 writes 0x2A (column) and 0x2B (row) using FUN_800271c0.
2. GRAM write: FUN_80027354 triggers command 0x2C and pixel stream via FUN_800156dc.
3. Pixel data copied from frame buffer at 0x20000BD0, written through PD8-PD15.
4. FUN_800037b0 coordinates the DMA transfer, gating PC12 during active refresh.

Reverse-Engineering Follow-ups
------------------------------
- Issue a read (0x04/0xD3) using the existing helpers to confirm the exact LCD controller.
- Validate PC14 as reset and check for additional backlight PWM control elsewhere.
- Scope PD0/PD1/PD3 to map timing and verify 8080 protocol matches.
