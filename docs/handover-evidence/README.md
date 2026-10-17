# V0.27 Analysis Evidence

These files came from `/Volumes/Shared_Drive/Projects/950-pro/docs/` on 2026-10-17. The source report is saved as `source-handover.md`. It records the original analysis and its limits.

The V0.27 application image used for the analysis has SHA-256 `fc223cfd2fa5456b8c8858e32d4cc193e91e383c74f2bd2e7725a3ff87993e91`. Its load address is `0x08003000`. A copy of that image is available in [Hertzz58's firmware project](https://github.com/Hertzz58/Radtel-RT950-Pro-Firmware).

| File | Contents |
| --- | --- |
| `v027-ghidra.json` | Function list, string list, and peripheral address references from Ghidra analysis. |
| `v027-strings.tsv` | Printable strings and their application addresses. |
| `v027-key.c` | Selected Ghidra decompiler output from the OEM image. It is analysis output, not buildable source code. |
| `rt950.gpr` and `rt950.rep/` | Ghidra 12.1.4 project files for the V0.27 application and bootloader. The `.gpr` file is empty by design; the data is in `.rep/`. |
| `source-handover.md` | Original handover report, kept for provenance. Some paths and development plans in it refer to a different computer. |

The [V0.27 analysis note](../rt950-v027-analysis.md) gives the findings that are useful to this repository. Check each pin claim against its stated evidence before using it in hardware code.
