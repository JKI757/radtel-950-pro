# -*- coding: utf-8 -*-
#@category AT32/Setup
# Setup memory map for AT32F403A/407 (Cortex-M4), and set entry from vector table.
# Run after importing the raw firmware at 0x08000000 with language ARM:LE:32:Cortex.

from ghidra.program.model.address import Address
from ghidra.program.model.symbol import SourceType
from ghidra.program.model.mem import MemoryConflictException, MemoryAccessException

prog = currentProgram
mem  = prog.getMemory()
symt = prog.getSymbolTable()
af   = prog.getAddressFactory().getDefaultAddressSpace()

def A(x): return af.getAddress(x)

def overlaps_existing(start, end):
    """True if any existing memory block overlaps [start,end]."""
    s = start; e = end
    for blk in mem.getBlocks():
        bs = blk.getStart().getOffset()
        be = blk.getEnd().getOffset()
        if not (e < bs or s > be):
            return True
    return False

def ensure_block_safe(name, start, end, perms="RW"):
    """
    Create an uninitialized block [start,end] if nothing overlaps.
    If overlap exists, print and skip (idempotent / safe).
    """
    if start > end:
        return
    startA = A(start)
    if overlaps_existing(start, end):
        print("[=] Skipped {:<14} {:08X}-{:08X} {} (overlaps existing)".format(name, start, end, perms))
        return
    try:
        blk = mem.createUninitializedBlock(name, startA, (end - start + 1), False)
        blk.setRead(True)
        blk.setWrite("W" in perms)
        blk.setExecute("X" in perms)
        print("[+] Created {:<14} {:08X}-{:08X} {}".format(name, start, end, perms))
    except MemoryConflictException as e:
        print("[-] Conflict creating {}: {}".format(name, e))
    except Exception as e:
        print("[-] Error creating {}: {}".format(name, e))

# --- Sanity check language (warn only)
langID = str(prog.getLanguageID())
if "ARM:LE:32:Cortex" not in langID:
    print("[!] Program language is '{}'; expected contains 'ARM:LE:32:Cortex'.".format(langID))
else:
    print("[+] Language OK:", langID)

# --- Core regions
# FLASH container (1 MiB declared in your .ld; raw binary should already be mapped starting at 0x08000000)
ensure_block_safe("FLASH",     0x08000000, 0x080FFFFF, perms="RX")
# SRAM 96 KiB per your .ld
ensure_block_safe("SRAM",      0x20000000, 0x20017FFF, perms="RW")
# Boot ROM (uninitialized placeholder unless you dump it)
ensure_block_safe("BOOTROM",   0x1FFFB000, 0x1FFFEFFF, perms="R")
# External memory (optional, helpful for xrefs); safe if auto-mapped already
ensure_block_safe("XMC_MEM",   0x60000000, 0x9FFFFFFF, perms="RW")
ensure_block_safe("XMC_REG",   0xA0000000, 0xA0000FFF, perms="RW")

# --- AHB
ensure_block_safe("DMA1",          0x40020000, 0x400203FF)
ensure_block_safe("DMA2",          0x40020400, 0x400207FF)
ensure_block_safe("RCC_CRM",       0x40021000, 0x400213FF)
ensure_block_safe("FLASH_IF",      0x40022000, 0x400223FF)
ensure_block_safe("CRC",           0x40023000, 0x400233FF)
ensure_block_safe("SDIO2",         0x40023400, 0x40027FFF)
ensure_block_safe("EMAC",          0x40028000, 0x40029FFF)   # (only on some 407s)
ensure_block_safe("AHB_GAP1",      0x40020800, 0x40020FFF)
ensure_block_safe("AHB_GAP2",      0x40021400, 0x40021FFF)
ensure_block_safe("AHB_GAP3",      0x4002A000, 0x4005FFFF)

# --- APB2
ensure_block_safe("IOMUX",         0x40010000, 0x400103FF)
ensure_block_safe("EXINT",         0x40010400, 0x400107FF)
ensure_block_safe("GPIOA",         0x40010800, 0x40010BFF)
ensure_block_safe("GPIOB",         0x40010C00, 0x40010FFF)
ensure_block_safe("GPIOC",         0x40011000, 0x400113FF)
ensure_block_safe("GPIOD",         0x40011400, 0x400117FF)
ensure_block_safe("GPIOE",         0x40011800, 0x40011BFF)
ensure_block_safe("ADC1",          0x40012400, 0x400127FF)
ensure_block_safe("ADC2",          0x40012800, 0x40012BFF)
ensure_block_safe("TMR1",          0x40012C00, 0x40012FFF)
ensure_block_safe("SPI1_I2S1",     0x40013000, 0x400133FF)
ensure_block_safe("TMR8",          0x40013400, 0x400137FF)
ensure_block_safe("USART1",        0x40013800, 0x40013BFF)
ensure_block_safe("ADC3",          0x40013C00, 0x40013FFF)
ensure_block_safe("TMR9",          0x40014C00, 0x40014FFF)
ensure_block_safe("TMR10",         0x40015000, 0x400153FF)
ensure_block_safe("TMR11",         0x40015400, 0x400157FF)
ensure_block_safe("ACC",           0x40015800, 0x40015BFF)
ensure_block_safe("I2C3",          0x40015C00, 0x40015FFF)
ensure_block_safe("USART6",        0x40016000, 0x400163FF)
ensure_block_safe("USART7",        0x40016400, 0x400167FF)
ensure_block_safe("USART8",        0x40016800, 0x40016BFF)
ensure_block_safe("SDIO",          0x40018000, 0x400183FF)
ensure_block_safe("APB2_GAP1",     0x40011C00, 0x40011FFF)
ensure_block_safe("APB2_GAP2",     0x40012000, 0x400123FF)
ensure_block_safe("APB2_GAP3",     0x40014000, 0x40014BFF)
ensure_block_safe("APB2_GAP4",     0x40016C00, 0x40017FFF)
ensure_block_safe("APB2_GAP5",     0x40018400, 0x4001FFFF)

# --- APB1
ensure_block_safe("TMR2",          0x40000000, 0x400003FF)
ensure_block_safe("TMR3",          0x40000400, 0x400007FF)
ensure_block_safe("TMR4",          0x40000800, 0x40000BFF)
ensure_block_safe("TMR5",          0x40000C00, 0x40000FFF)
ensure_block_safe("TMR6",          0x40001000, 0x400013FF)
ensure_block_safe("TMR7",          0x40001400, 0x400017FF)
ensure_block_safe("TMR12",         0x40001800, 0x40001BFF)
ensure_block_safe("TMR13",         0x40001C00, 0x40001FFF)
ensure_block_safe("TMR14",         0x40002000, 0x400023FF)
ensure_block_safe("RTC",           0x40002800, 0x40002BFF)
ensure_block_safe("WWDT",          0x40002C00, 0x40002FFF)
ensure_block_safe("WDT",           0x40003000, 0x400033FF)
ensure_block_safe("SPI2_I2S2",     0x40003800, 0x40003BFF)
ensure_block_safe("USART2",        0x40004400, 0x400047FF)
ensure_block_safe("USART3",        0x40004800, 0x40004BFF)
ensure_block_safe("UART4",         0x40004C00, 0x40004FFF)
ensure_block_safe("UART5",         0x40005000, 0x400053FF)
ensure_block_safe("I2C1",          0x40005400, 0x400057FF)
ensure_block_safe("I2C2",          0x40005800, 0x40005BFF)
ensure_block_safe("USBFS",         0x40005C00, 0x40005FFF)
ensure_block_safe("CAN1",          0x40006400, 0x400067FF)
ensure_block_safe("CAN2",          0x40006800, 0x40006BFF)
ensure_block_safe("BPR",           0x40006C00, 0x40006FFF)
ensure_block_safe("PWC",           0x40007000, 0x400073FF)
ensure_block_safe("DAC",           0x40007400, 0x400077FF)
ensure_block_safe("USBFS_BUF_512", 0x40006000, 0x400063FF)
ensure_block_safe("USBFS_BUF_768", 0x40007800, 0x400083FF)
ensure_block_safe("APB1_GAP1",     0x40002400, 0x400027FF)
ensure_block_safe("APB1_GAP2",     0x40003400, 0x400037FF)
ensure_block_safe("APB1_GAP3",     0x40003C00, 0x40003FFF)
ensure_block_safe("APB1_GAP4",     0x40008400, 0x4000FFFF)

# --- Set entry point from vector table
try:
    reset_val = getLong(A(0x08000004)) & 0xFFFFFFFF
    resetA = A(reset_val)
    try:
        createLabel(resetA, "Reset_Handler", True, SourceType.USER_DEFINED)
    except:
        pass
    symt.addExternalEntryPoint(resetA)
    print("[+] Reset_Handler entry set at 0x%08X" % reset_val)
except MemoryAccessException:
    print("[-] Could not read reset vector at 0x08000004. Check load address/base.")