#!/usr/bin/env python3
"""
fwcrypt_io.py — Python equivalent of the C# FwCrypt routine,
with explicit input and output filenames.

Usage:
    python fwcrypt_io.py --infile firmware_encrypted.bin --outfile firmware_decrypted.bin [--verbose]

Behavior:
 - Reads the input binary file.
 - Extracts 16-byte base key from offset 0x400.
 - Builds full 128-byte key (8 × 16-byte segments) using left/right bit rotations.
 - Starting from offset 0x800, XORs each non-00/FF byte with key[i % 128].
 - If the result becomes 00 or FF, keeps the original byte.
 - Leaves the first two 1024-byte blocks (0x0000–0x07FF) untouched.
 - Writes output to the specified --outfile.
"""

import argparse
import os
import sys

BLOCK_SKIP_SIZE = 0x800   # skip first two 1KB blocks (2×1024)
BASE_KEY_OFFSET = 0x400
BASE_KEY_LEN = 16
FULL_KEY_LEN = 128


def rol8(value: int, shift: int) -> int:
    """8-bit rotate left."""
    return ((value << shift) & 0xFF) | (value >> (8 - shift))


def ror8(value: int, shift: int) -> int:
    """8-bit rotate right."""
    return ((value >> shift) | ((value << (8 - shift)) & 0xFF)) & 0xFF


def build_key(fw: bytes) -> bytes:
    """Construct 128-byte key from firmware bytes."""
    if len(fw) < BASE_KEY_OFFSET + BASE_KEY_LEN:
        raise ValueError("Firmware too short to contain base key at 0x400")

    key = bytearray(FULL_KEY_LEN)
    key[0:BASE_KEY_LEN] = fw[BASE_KEY_OFFSET:BASE_KEY_OFFSET + BASE_KEY_LEN]

    for j in range(16, 128, 16):
        # first 8 bytes: rotate-left(previous section’s byte)
        for i in range(j, j + 8):
            key[i] = rol8(key[i - 16], 1)
        # last 8 bytes: rotate-right(previous section’s byte)
        for i in range(j + 8, j + 16):
            key[i] = ror8(key[i - 16], 1)

    return bytes(key)


def fwcrypt(fw_bytes: bytearray, key: bytes, verbose: bool = False) -> bytearray:
    """Encrypt/decrypt firmware in place."""
    total = len(fw_bytes)
    processed = 0

    for i in range(BLOCK_SKIP_SIZE, total):
        cr = fw_bytes[i]
        if cr in (0x00, 0xFF):
            continue
        k = key[i % FULL_KEY_LEN]
        outb = cr ^ k
        if outb in (0x00, 0xFF):
            continue
        fw_bytes[i] = outb
        processed += 1

    if verbose:
        print(f"Processed {processed} bytes (out of {total - BLOCK_SKIP_SIZE} after skip).")

    return fw_bytes


def main():
    parser = argparse.ArgumentParser(description="Firmware encrypt/decrypt tool (Python version of FwCrypt).")
    parser.add_argument("--infile", required=True, help="Input firmware binary file")
    parser.add_argument("--outfile", required=True, help="Output binary filename")
    parser.add_argument("--verbose", action="store_true", help="Show detailed progress and key info")

    args = parser.parse_args()
    infile = args.infile
    outfile = args.outfile

    if not os.path.exists(infile):
        sys.exit(f"Input file not found: {infile}")

    with open(infile, "rb") as f:
        fw = bytearray(f.read())

    if args.verbose:
        print(f"Loaded {len(fw)} bytes from {infile}")

    key = build_key(fw)

    if args.verbose:
        base_key = fw[BASE_KEY_OFFSET:BASE_KEY_OFFSET + BASE_KEY_LEN]
        print("Base key (16 bytes @0x400):", " ".join(f"{b:02X}" for b in base_key))
        print("Expanded key (128 bytes):", " ".join(f"{b:02X}" for b in key))

    fw_out = fwcrypt(fw, key, verbose=args.verbose)

    with open(outfile, "wb") as fo:
        fo.write(fw_out)

    print(f"Output written to {outfile}")
    if args.verbose:
        print("Operation complete. Run again on same file to reverse the operation (symmetric).")


if __name__ == "__main__":
    main()
