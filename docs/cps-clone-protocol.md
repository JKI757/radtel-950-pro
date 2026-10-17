# RT-950 Pro CPS Clone Protocol

This note summarizes the [CHIRP RT-950 Pro protocol work](https://github.com/NathanBarguss/Chirp_Radtel-RT-950-Pro/blob/main/RADIO_PROTOCOL_WIP.md), last marked 2025-09-25 in that project. Its author reports checks with the vendor CPS and a radio. We have not repeated those tests. The [OEM update protocol](../firmware/RE/bootloader/bootloader_protocol_notes.md) is a separate path and uses different frames.

## Serial session

- The CHIRP project reports a USB CDC serial connection at 115200 baud, 8 data bits, no parity, and 1 stop bit.
- The host sends `PROGRAMBT9000U` and expects `0x06`. It then sends `F` for a 16-byte probe response and `M` for a 12-byte model response. The reported model check looks for `RT-950`.
- The host sends a frame that starts with `SEND` to select a four-byte XOR key. The radio acknowledges with `0x06`. The key applies to later 128-byte data blocks. The source gives inconsistent counts for the random bytes in this frame; check its implementation before building a sender. It also describes XOR exceptions for `0x00`, `0xFF`, and bytes that match the key or its inverse.
- Requests and replies are sequential. The source reports a one-second CPS retry timer and at most three attempts, but says it did not force timeout cases to test that behavior.

## Clone blocks

The source reports a four-byte read request: command, address high byte, address low byte, and `0x80` length. The radio returns the header and a 128-byte encoded data block. It reports `0x52` (`R`) for the main sections and `0x54` for APRS. Writes use `0x57` (`W`) for the main sections and `0x55` for APRS; the host waits for `0x06` after each write. The host sends `E` to end the session.

| Address range | Reported content | Size |
| --- | --- | ---: |
| `0x0000` to `0x77FF` | 960 channel records, 32 bytes each | `0x7800` |
| `0x8000` to `0x80FF` | VFO data | `0x0100` |
| `0x9000` to `0x90FF` | Function settings | `0x0100` |
| `0xA000` to `0xA1FF` | DTMF | `0x0200` |
| `0xB000` to `0xB1FF` | Modulation parameters | `0x0200` |
| `0xD000` to `0xD2FF` | Modulation names | `0x0300` |
| APRS address `0x0000` to `0x007F` | APRS block on the separate `0x54`/`0x55` command pair | `0x0080` |

The listed transfer sizes total 33,152 bytes. The source says that the radio can change bytes outside the defined sections after a write. It also gives different limits, `0x7A80` and `0x8080`, for comparisons of the returned image. Do not treat either limit as confirmed here. Compare only sections that a test has shown to be stable.

The source reports that DTMF digit zero uses byte `0x00`, while `0xFF` ends a DTMF sequence. It also says that an unchanged record should keep its original bytes. These details matter when a tool writes a codeplug, but this repository has not checked them on a radio.

## What this does and does not confirm

The source reports use of an `R` command to read **CPS clone data**. It does not show that the CPS **image-upload** `W` command from [JKI757 issue #1](https://github.com/JKI757/radtel-950-pro/issues/1) has a matching read command. It also does not locate the `0x90000` image data in internal or external flash. The CHIRP clone addresses above belong to a reported logical clone map; do not treat them as physical flash addresses without another check.

The [Hertzz58 project](https://github.com/Hertzz58/Radtel-RT950-Pro-Firmware) also describes `PROGRAMBT9000U` and raw `R`/`W` blocks. Its proposed external-flash map has different channel counts and positions. These maps need a read-only capture from one known radio before they can be joined.
