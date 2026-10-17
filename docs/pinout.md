Radtel Peripheral & Pin Summary

| Module | Peripheral / Base | Pins & Signals | Key Firmware Functions | Notes |
| --- | --- | --- | --- | --- |
| BK4829 #1 (HW SPI) | SPI1 @ 0x40013000<br>Data: 0x4001300C | PB12 CS via GPIOB BSRR 0x40010C10 / BRR 0x40010C14 | spi_xfer_byte(FUN_8002112c), BK4829_init(FUN_80007f04), RF_Set_Frequency_Synthesizer(FUN_8000b62c) | Historical capture showed PB4 (mask 0x10); current firmware drives PB12 (mask 0x1000). |
| BK4829 #2 (SW SPI) | GPIOA bit-bang @ 0x40010800<br>SCR 0x40010810 / CLR 0x40010814 | PA5 CS, PA6 SCLK, PA7 MOSI | Software_SPI_Write_Byte(FUN_80021234), Software_SPI_Write_Block(FUN_80021290), Software_SPI_FlashErase4K (FUN_800210c0), Software_SPI_FlashErase32KBlock (FUN_80020f80), Software_SPI_FlashErase64KBlock (FUN_80020ff0) | Second RF front-end path written over GPIO toggles. |
| SI4732 (Bit-bang I2C / TBD) | GPIOA bit-bang | PA8 SCL, PA9 SDA | (pending rename) | Lines shared with discrete I2C routine; no hardware I2C peripheral observed. |
| GPS Receiver | USART3 @ 0x40004800 | PC10 TX?, PC11 RX (shared with UART4) | GPS_USART3_Init (FUN_80013f90), FUN_8001403c | Configures 9600 baud NMEA stream; high confidence. PC10/PC11 also remapped by UART4 when Bluetooth accessory is active. |
| Bluetooth Link (Main) | USART1 @ 0x40013800 | PA9 TX, PA10 RX | Bluetooth_UART1_Init (FUN_8000834c), FUN_80008518 | 115200 baud to on-board BT module (probable). Captured strings include 'BLUETOOTH'. |
| Accessory Port (Low confidence) | UART4 @ 0x40004C00 | PC10 TX?, PC11 RX (shared with GPS) | Bluetooth_UART4_Init (FUN_800226b0) | 115200 baud, likely routed to side connector for provisioning or external serial; conflicts with GPS pins when enabled. |
| Keypad Matrix | GPIOC/PD/PE/PA | PC0-3 column drive, PD4-7 row sense, PE5 scan enable, PA12 latch sense | Keypad_ScanMatrix (FUN_80013618), FUN_80012c20, FUN_80012c26 | Rows read from DAT_800136dc (GPIOD IDR) and decoded for 0x70/0xB0/0xD0/0xE0 patterns. |
| Rotary Encoder | GPIOB | PB4 phase A, PB5 phase B | Encoder_HandleQuadrature (FUN_8000e2e0), FUN_8000e42c | State machine samples masks 0x10 / 0x20 and debounces transitions. |
| PTT / RF Path Control | GPIOE / GPIOB | PE13 primary PTT, PE12 external PTT, PE14 PA gate, PE7 mic toggle, PB0/PB1 accessory | PTT_Relay_Select (FUN_8001ab04) | Selects TX/RX relay combinations based on parameters; clears PB0/PB1 when idle. |
| Audio Tone Output (probable) | DAC1 @ 0x40007400 + DMA2 | PA4 DAC_OUT1 (confirmed), PA5 DAC_OUT2 (low confidence) | AudioDMA_Trigger (FUN_8000dca0), ToneMenu_ShowEntry (FUN_8000bd58), ToneGraphic_Draw (FUN_8000cfd4) | Generates CTCSS/AFSK tones via DMA; verify PA5 usage on hardware. |
| Band / Relay SPI | GPIOE bit-bang | PE15 latch, PE10 clock, PE11 data in, PE8 secondary latch | Band_Relay_ShiftIn (FUN_8001c04c), FUN_8001c68c, FUN_8001c758 | Serial shift interface for RF filter relays; reads back status then programs new codes. |
| Analog Front-End (ADC) | ADC2 @ 0x40012800 | PA1 channel 1, PA0 channel 0 | ADC_Read_PA1 (FUN_80013c98), ADC_Read_PA0 (FUN_80013cd4), FUN_80003108, FUN_800030a4, FUN_8000309e | Single-shot conversions with 7-cycle sample time; used for audio/AGC sensing. |
| Analog Front-End (DAC) | DAC @ 0x40007400 | Internal outputs (routed to audio chain) | DAC_Channel_Enable (FUN_8000b368), FUN_8000b388, FUN_8000b3a8 | Enables channels and loads sample registers before playback. |
| Display Interface | DMA2 + GPIOD[15:8] 8080 bus | PD8-15 data, PD0 WR, PD1 CS, PD3 D/C; PC12 strobe/backlight | Display_BufferFlush (FUN_800037b0), LCD_WriteCommand (FUN_800271c0), LCD_WriteData (FUN_80027220), LCD_SetWindow (FUN_8001cb52), LCD_BlitRectangle (FUN_800156dc) | 16-bit RGB565; 320x240 frame buffer @ 0x20000BD0; DMA2 (0x40020430) streams rows to LCD. |


  
PA0 = VOX DETECT  
PA1 = BATTERY DETECT  
PA2 = SINGLE IN  
PA3 = BT UART IN  
PA4 = BEEP OUT  
PA5 = APC  
PA6 = FM RESET  
PA7 = V3FM EN  
PA8 = GPS ENABLE  
PA9 = BLUETOOTH RX  
PA10 = BLUETOOTH TX  
PA11 = DEVICE POWER OFF  
PA12 = SK 4?  
PA13 = SW SDA  
PA14 = SW CLK  
PA15 = REPLAY  
  

PB0 = V3R ENABLE  
PB1 = V3T ENABLE  
PB2 = V3RX ENNABLE  
PB3 = KEYPAD LIGHT  
PB4 = EC1  
PB5 = EC2  
PB6 = SI4732 SCK  
PB7 = SI4732 SDA  
PB8 = MICROPHONE ENABLE  
PB9 = LB POWER ENABLE  
PB10 = GPS RX  
PB11 = GPS TX  
PB12 = FLASH CS  
PB13 = FLASH SCK  
PB14 = FLASH MISO  
PB15 = FLASH MOSI  

  
PC0 = KEYPAD RAS 0  
PC1 = KEYPAD RAS 1  
PC2 = KEYPAD RAS 2  
PC3 = KEYPAD RAS 3  
PC4 = RF BAND RELAY??  
PC5 = VSW ENABLE  
PC6 = LCD BACKLIGHT  
PC7 = PTT DETECT  
PC8 = SIDEPORT RX DETECT ??  
PC9 = SIDEPORT PTT ??  
PC10 = UART TX  
PC11 = UART RX  
PX12 = BEEP SW  
PC13 = RED LED  
PC14 = GREEN LED  
PC15 = SIDEPORT EXTERNAL SPEAKER DETECTED ??  

  

PD0 = LCD SDA  
PD1 = LCD CS  
PD2 = LCD RESET  
PD3 = LCD RS  
PD4 = KEYPAD CAS 3  
PD5 = KEYPAD CAS 2  
PD6 = KEYPAD CAS 1  
PD7 = KEYPAD CAS 0  
PD8 TO PD15 = PARALLEL DATA LINES TO LCD  
  


PE0 = POWER SWITCH  
PE1 = SPEAKER MUTE  
PE2 = PTT 2  
PE3 = PTT  
PE4 = POWER AMP ENABLE  
PE5 = SIDE KEY 1  
PE6 = EXTERNAL PTT  
PE7 = U3T EN  
PE8 = BK4819 SEN1  
PE9 = SW TO BT ??  
PE10 = BK4819 SCK  
PE11 = BK4819 SDA  
PE12 = U3R ENABLE  
PE13 = U6R ENABLE  
PE14 = SW3T ENABLE  
PE15 = BK4819 SEN2  
