# RT-950 Bootloader Reverse-Engineering Notes

_Last updated: $(date)_

## Captured Protocol Overview

The USB updater that ships with the radio speaks a very small CDC/ACM binary protocol. The same framing is visible in both the `upgrade` and `fullflash` captures. The [CPS clone protocol](../../../docs/cps-clone-protocol.md) uses a different session and block format; its `R` command does not establish a read command in this updater:

- Each frame starts with `0xAA`, ends with `0x55`.
- Bytes `[1..2]` carry the command identifier; the device mirrors the same byte in ACKs.
- Bytes `[3..4]` are a big-endian payload length.
- Payload length for data blocks is 1024 bytes; other commands carry short ASCII strings or metadata.
- A CRC16/XMODEM (polynomial `0x1021`, init `0x0000`) covers `cmd + param1 + len + payload`.
- Device ACKs always set `param1 = 0x0006` and send an empty payload.

| Cmd | Direction | Meaning (from capture)                | Implementation |
|-----|-----------|---------------------------------------|----------------|
|0x42 | host→dev  | Enter binary/update mode              | `FUN_0800e500 → FUN_0800df4c`
|0x0A | host→dev  | Send bootloader version string        | `FUN_0800df4c`
|0x02 | host→dev  | Send model metadata block             | `FUN_0800df4c`
|0x04 | host→dev  | Send short config blob (0x0178, etc.) | `FUN_0800df4c`
|0x03 | host→dev  | Data chunk (chunk# in `param1`)       | `FUN_0801fdb8`
|0x45 | host→dev  | Finalise and reboot                   | `FUN_0800ebbc` state machine |

The matching ACKs (device→host) are emitted by the same helpers but always express success (`param1 = 0x0006`). No codepath produces a payload-bearing reply.

## Key Functions (Firmware Address → Purpose)

| Function        | Address      | Summary |
|-----------------|--------------|---------|
|`FUN_0800e500`   | `0x0800E500` | CDC RX state machine. Pulls bytes from USB buffer (`FUN_08012eae`), matches start-of-frame sequences, and triggers command handlers once a complete frame has been reconstructed. |
|`FUN_0800df4c`   | `0x0800DF4C` | Maps parsed command opcodes into the outbound transmit queue. Writes the actual `0x41/0x42/0x43/0x44/0x2A…` bytes we see on the wire. |
|`FUN_0800eef4`   | `0x0800EEF4` | Drains the transmit queue (`DAT_0800ef8c`/`DAT_0800ef90`) and feeds the CDC IN endpoint through `FUN_08014928`. Handles UI updates when chunks complete. |
|`FUN_0800ebbc`   | `0x0800EBBC` | Main bootloader loop. Implements the state machine for “update”, “write”, “erase”, and “reboot” modes. Controls the on-radio UI state while an update is in progress. |
|`FUN_0801fdb8`   | `0x0801FDB8` | Chunk manager. Consumes metadata from `DAT_080200e0`, writes each 1KB block into flash (`FUN_0801a4d4`/`FUN_0801a4e8`), and drives the UI progress states. |
|`FUN_0801fd00`   | `0x0801FD00` | Builds response frames for command class 0x05/0x07; stamps header `0xAA`, fills len/cmd, and queues structure for transmit. |
|`FUN_0801fd24`   | `0x0801FD24` | Same as above for command class 0x09/0x0A. |
|`FUN_0801fd48`   | `0x0801FD48` | Same as above for command class 0x03/0x04 (data chunk ACKs). |
|`FUN_0800b410`   | `0x0800B410` | CRC16-XMODEM implementation used before emitting ACKs. |
|`FUN_0800f324`   | `0x0800F324` | Resets UI state and exits update mode when the upload completes or the user cancels. |

Support routines of note: `FUN_0800f43c`, `FUN_0800f408`, `FUN_0800f3dc` (UI clean-up and message display), `FUN_0801fba4`/`FUN_0801fcb4` (string formatting for progress popups), and `FUN_0801a4d4` (flash erase/program, invoked indirectly by the chunk manager).

### Data Structures / Globals

- `DAT_0800e644`: RX state block. Fields `[0]` current command, `[1]` sequence, `[4]` countdown timer for multi-frame commands, `[5]/[6]` receiver sub-state.
- `DAT_0800ef8c` & `DAT_0800ef90`: TX FIFO bookkeeping (write index, read index, pointer to current chunk).
- `DAT_2000aec4` / `DAT_2000aed4`: Bootloader state variables used by `FUN_0800ebbc` to track the ASCII “F/U/D/R/W/M” commands.
- `DAT_0801fdb4`: UI mode indicator. When >2 the bootloader pops up status dialogs via `FUN_0801fcb4` and blocks user input.

A typical data-frame ACK path looks like this:

1. `FUN_0800e500` assembles a `0x03` frame and stores chunk metadata.
2. `FUN_0801fdb8` writes the block to flash and flips `DAT_080200e0[0]` to the next state.
3. `FUN_0801fd48` prepares the `0x03` ACK (header `AA 03 00 06 00 00` + CRC `0x5C72`).
4. `FUN_0800eef4` pushes that frame to the USB IN endpoint.

## Observed Behaviour

- Bootloader only writes – no host-visible readback paths exist. Every device-to-host frame is length 0, and the command switch in `FUN_0801fdb8` never routes data into the IN FIFO. This matches the OEM updater: it offers “Program”, “Erase”, “Reboot” but no “Read” option.
- CRC validation happens before every ACK, so our Python uploader must compute the same CRC16 to satisfy the bootloader.
- UI feedback ("NONE", "D:on", etc.) is entirely tied to the chunk counter and result flags; there are no hidden protocol commands embedded in those strings.

## Function Reference (by Address)

```
0x0800B410  FUN_0800b410           // CRC16-XMODEM over memory buffer
0x0800DF4C  FUN_0800df4c           // Queue outbound frame (set cmd byte)
0x0800E500  FUN_0800e500           // CDC RX parser / frame assembler
0x0800EBBC  FUN_0800ebbc           // Bootloader main loop/state machine
0x0800EEF4  FUN_0800eef4           // CDC TX queue handler
0x0800F324  FUN_0800f324           // UI teardown when leaving update mode
0x0801FBA4  FUN_0801fba4           // Draw “chunk” status popup (UI only)
0x0801FD00  FUN_0801fd00           // Build ACK frame for command class 0x05/0x07
0x0801FD24  FUN_0801fd24           // Build ACK frame for command class 0x09/0x0A
0x0801FD48  FUN_0801fd48           // Build ACK frame for command class 0x03/0x04
0x0801FDB8  FUN_0801fdb8           // Chunk processing & flash programming
```

(Replace the addresses with the symbolic name once you rename them in Ghidra; the existing names come straight from the default export.)

## Next Steps

1. **Confirm the updater GUI** – Decompile the vendor’s C# updater (e.g., `RT_950_EnUPDATE.exe`). Look for UART/USB command constants (0x42, 0x03, etc.) to see whether any hidden “READ” verb exists. If it doesn’t, the bootloader probably never had readback wired up.
2. **Improve script integration** – We can translate the structures above into a typed Python representation so `radtel_flash.py` no longer relies on magic offsets.
3. **Optional verification** – If you want belt-and-suspenders, sniff the CDC stream while running the OEM updater to ensure there’s no special behaviour before/after the upload (e.g., vendor-specific signature check).

Let me know once you have the ILSpy dump – we can cross-reference the C# opcodes with the firmware functions listed here.
