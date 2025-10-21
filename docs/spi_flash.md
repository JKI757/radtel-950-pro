SPI Flash Notes
================

The Radtel 950 Pro uses an external SPI NOR device accessible via the bit-banged SPI bus on GPIOA (same lines used for the second BK4829).

Key entry points:

- `Software_SPI_Write_Byte` / `Software_SPI_Write_Block`: fundamental byte/stream writers that toggle PA5 (CS), PA6 (SCLK), and PA7 (MOSI).
- `Software_SPI_FlashErase4K` (`FUN_800210c0`): issues opcode `0x20` plus a 24-bit address, then polls the status register until the write-in-progress bit clears.
- `Software_SPI_FlashErase32KBlock` (`FUN_80020f80`): same as above but sends opcode `0x52` (32 KB erase).
- `Software_SPI_FlashErase64KBlock` (`FUN_80020ff0`): sends opcode `0xD8` (64 KB erase).
- `FUN_80021180`: performs opcode `0x03` and reads blocks into RAM (`software SPI read`), used for channel memories and BT firmware transfers.

Where they are used:

- The Bluetooth command sequencer (`FUN_80008518`) calls the erase helpers when flashing the Bluetooth module, so captured traffic shows a mix of 4K/32K/64K erases. -- I bet this is CPS/channel/settings storage as I think this radio has the ability to reprogram over BT.
- Channel/programming routines elsewhere in the firmware (e.g., around 0x80018504–0x80019540) reference these helpers with addresses like `0x11000`, `0x21000`, etc., indicating sector layouts.

Hardware mapping:

- Bus pins: PA5 (CS), PA6 (CLK), PA7 (MOSI); MISO is read when `spi_xfer_byte()` is invoked inside `FUN_80021180`.
- This bus is shared with the second BK4829 RF front-end, so chip-select discipline is important before sending flash commands.

Notes:

- All erase routines poll `FUN_80020f6c()` for completion; if you port these, keep the delay loops or replace them with a proper status read.
- If you plan to dump the flash, reuse `FUN_80021180` and ensure PA5 is asserted for the duration of the read (the OEM code toggles in `Software_SPI_Write_Block`).
