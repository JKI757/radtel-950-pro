decrypted.bin is the firmware decrypted from the btf file from the 0.18 firmware update.  the fwcrypt_io.py under scripts was used to decrypt it.   

RE/decrypted-firmware is the ghidra project I've been working on.
RE/firmware dumps is a USB capture of the firmware update process -- part 1 is the update, part 2 is the complete flash.

## Serial tool setup

The [serial tool](scripts/radtel_flash.py) needs PySerial. Install it with
`python -m pip install pyserial`, then run
`python firmware/scripts/radtel_flash.py --help` from the repository root to
see the available commands.

