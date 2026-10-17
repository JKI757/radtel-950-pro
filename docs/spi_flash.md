# RT-950 Pro SPI Flash Notes

These notes describe the OEM V0.27 application. See the [V0.27 analysis note](rt950-v027-analysis.md) for the image hash and method.

## Bus

The function at `0x08017000` sets GPIOB mask `0x1000` (PB12) and configures the SPI2 peripheral at `0x40003800`. The checked image contains three aligned words with the SPI2 base and no aligned word with the SPI1 base `0x40013000`.

The [Hertzz58 board pin map](https://github.com/Hertzz58/Radtel-RT950-Pro-Firmware/blob/main/include/rt950_pinmap.h) assigns PB13 to clock, PB14 to MISO, and PB15 to MOSI. This review did not trace each alternate function setup. Read the flash chip ID on the radio before choosing a driver. The project changelog reports a Macronix ID on one tested unit; other project files name a Winbond chip.

## Data and write limits

The separate firmware project proposes a channel and settings layout in external flash. Its headers disagree about some record positions and counts. Treat that layout as a working hypothesis. Compare a read-only flash dump with a known channel before writing records.

The project places factory calibration data near `0x00F000`. Keep that area unchanged during experiments until its layout is confirmed. The older [SPI flash notes](spi_flash-legacy.md) record erase opcodes and software SPI function names from earlier analysis; their GPIOA bus assignment does not match the checked V0.27 setup.

[JKI757 issue #1](https://github.com/JKI757/radtel-950-pro/issues/1) reports that the CPS image-upload feature sends a `W` command at offset `0x90000`. The reporter did not know whether that offset refers to external or internal flash. The [original handover](handover-evidence/source-handover.md) also lists a splash block at `0x090000`; this match is a lead, not proof of the target device. The [CHIRP report](cps-clone-protocol.md) documents an `R` read command for CPS clone data. It does not confirm a read command for image uploads.
