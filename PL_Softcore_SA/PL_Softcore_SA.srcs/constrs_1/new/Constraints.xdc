# CONNECTIONS ON DIGILENT ARTY A7 BOARD THAT DO NOT FEED THROUGH TO THE SSA
# CLOCK AND EXTERNAL RESET
# Clock input (100 MHz single-ended clock on E3)
set_property PACKAGE_PIN E3 [get_ports CLK_100MHZ]
set_property IOSTANDARD LVCMOS33 [get_ports CLK_100MHZ]
# Reset input (PUSH BTN0 on D9)
set_property PACKAGE_PIN D9 [get_ports RST_PB]
set_property IOSTANDARD LVCMOS33 [get_ports RST_PB]
set_property PULLTYPE PULLDOWN [get_ports RST_PB]

# ARTY A7 TEST SWITCHS x2 AND PUSH BUTTONS x3
# Switch input (SW0 on A8)
set_property PACKAGE_PIN A8 [get_ports SW_0]
set_property IOSTANDARD LVCMOS33 [get_ports SW_0]
# Switch input (SW1 on C11)
set_property PACKAGE_PIN C11 [get_ports SW_1]
set_property IOSTANDARD LVCMOS33 [get_ports SW_1]
# Switch input (SW2 on C10)
#set_property PACKAGE_PIN C10 [get_ports {SW_2[0]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {SW_2[0]}]
# TEST PUSH BUTTON INPUTS
# TEST 1 (PUSH BTN1 on C9)
set_property PACKAGE_PIN C9 [get_ports PB_1]
set_property IOSTANDARD LVCMOS33 [get_ports PB_1]
# TEST 2 (PUSH BTN2 on B9)
set_property PACKAGE_PIN B9 [get_ports PB_2]
set_property IOSTANDARD LVCMOS33 [get_ports PB_2]
# TEST 3 (PUSH BTN3 on B8)
set_property PACKAGE_PIN B8 [get_ports PB_3]
set_property IOSTANDARD LVCMOS33 [get_ports PB_3]
# Remove no_input_delay warnings - these inputs not tied to clock
set_false_path -from [get_ports PB_*]


# BOTH A7 DEDICATED AND CONNECTIONS THAT FEED THROUGH TO THE SSA: JA = J6, JB = J3, JC = J1, JD = J4 {A7_Connector = SSA_Connector)
# TIMER TEST SIGNALS OUTPUTS x2 ON ARTY A7 J4 CONNECTOR
# TIMER 1 output PIN TIM1 on A7 J4.2 U11
set_property PACKAGE_PIN U11 [get_ports {gpio2_io_o_0[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[0]}]
# TIMER 2 output PIN TIM2 on A7 J4.4 V16
set_property PACKAGE_PIN V16 [get_ports {gpio2_io_o_0[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[1]}]

# UART (AXI_UARTLITE_0)
# UART TX output PIN (A7 JD.7 E2) = SA J4.7
set_property PACKAGE_PIN E2 [get_ports UART_RX]
set_property IOSTANDARD LVCMOS33 [get_ports UART_RX]
# UART RX input PIN (A7 JD.8 D2) = SA J4.8
set_property PACKAGE_PIN D2 [get_ports UART_TX]
set_property IOSTANDARD LVCMOS33 [get_ports UART_TX]

#SSD1309 OLED DISPLAY (SHARES AXI_QUAD_SPI_0)
#CS OUPTUT PIN (A7 JD.1 D4) = SA J4.1 FW: DISPLAY_CS
set_property PACKAGE_PIN D4 [get_ports {gpio2_io_o_0[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[4]}]
# Command(0) / Data(1) PIN (A7 JD.2 D3) = SA J4.2 FW: DISPLAY_CMD_DATA
set_property PACKAGE_PIN D3 [get_ports {gpio2_io_o_0[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[3]}]
# RESET (ACTIVE LOW) PIN (A7 JD.3 F4) = SA J4.3 FW: DISPLAY_RESET_RUN
set_property PACKAGE_PIN F4 [get_ports {gpio2_io_o_0[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[2]}]
#MOSI OUPTUT PIN (A7 JD.4 F3) = SA J4.4
set_property PACKAGE_PIN F3 [get_ports DISPLAY_MOSI]
set_property IOSTANDARD LVCMOS33 [get_ports DISPLAY_MOSI]
#SCLK OUTPUT PIN (A7 JD.10 G2) = SA J4.10
set_property PACKAGE_PIN G2 [get_ports DISPLAY_SCLK]
set_property IOSTANDARD LVCMOS33 [get_ports DISPLAY_SCLK]
#MISO INPUT PIN ***NOT USED***
# DISPLAY_CSn PIN (A7 CK_IO5 ON J4.11) ***NOT USED***
set_property PACKAGE_PIN T14 [get_ports {IOX_CSn[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {IOX_CSn[0]}]

#IO EXPANDER X2 AND WAVEFORM GENERATOR X1 (SHARES AXI_QUAD_SPI_0)
#COMMON SCLK OUTPUT PIN (A7 JC.1 U12) = SA J1.1
set_property PACKAGE_PIN U12 [get_ports IOX_SCLK]
set_property IOSTANDARD LVCMOS33 [get_ports IOX_SCLK]
#COMMON MOSI OUTPUT PIN (A7 JC.2 V12) = SA J1.2
set_property PACKAGE_PIN V12 [get_ports IOX_MOSI]
set_property IOSTANDARD LVCMOS33 [get_ports IOX_MOSI]
#IOX MISO INPUT PIN (A7 JC.3 V10 = SA J1.3)
set_property PACKAGE_PIN V10 [get_ports IOX_MISO]
set_property IOSTANDARD LVCMOS33 [get_ports IOX_MISO]
# IOX RESET OUTPUT PIN (A7 JC.4 V11) = SA J1.4
set_property PACKAGE_PIN V11 [get_ports {gpio2_io_o_0[8]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[8]}]
# IO EXPANDER 1 CS PIN (A7 JC.10 U13) = SA J1.10
set_property PACKAGE_PIN U13 [get_ports {IOX_CSn[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {IOX_CSn[1]}]
# IO EXPANDER 2 CS PIN (A7 JC.9 T13) = SA J1.9
set_property PACKAGE_PIN T13 [get_ports {IOX_CSn[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {IOX_CSn[2]}]
# IO EXPANDER 2 IRQ PIN (A7 JC.8 V14) = SA J1.8
set_property PACKAGE_PIN V14 [get_ports IOX_2_IRQ]
set_property IOSTANDARD LVCMOS33 [get_ports IOX_2_IRQ]
# WAVEFORM GENERATOR CS / FSYNC PIN (A7 JB.10 J15) = SA J3.10
set_property PACKAGE_PIN J15 [get_ports {IOX_CSn[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {IOX_CSn[3]}]

#MICRO-SD (AXi_QUAD_SPI_1)
#CS OUPTUT PIN (A7 JB.1 E15) = SA J3.1
set_property PACKAGE_PIN E15 [get_ports {USD_CSn[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {USD_CSn[0]}]
#MOSI OUPTUT PIN (A7 JB.2 E16) = SA J3.2
set_property PACKAGE_PIN E16 [get_ports USD_MOSI]
set_property IOSTANDARD LVCMOS33 [get_ports USD_MOSI]
#MISO INPUT PIN (A7 JB.3 D15) = SA J3.3
set_property PACKAGE_PIN D15 [get_ports USD_MISO]
set_property IOSTANDARD LVCMOS33 [get_ports USD_MISO]
#SCLK OUTPUT PIN (A7 JB.4 C15) = SA J3.4
set_property PACKAGE_PIN C15 [get_ports USD_SCLK]
set_property IOSTANDARD LVCMOS33 [get_ports USD_SCLK]
# CARD DETECT PIN (A7 JB.9 on K15) = SA J3.9
set_property PACKAGE_PIN K15 [get_ports USD_CD]
set_property IOSTANDARD LVCMOS33 [get_ports USD_CD]

## ADC DUAL 7476A
## ADC_CSn PIN (JA1 on G13) = J6.1
#set_property PACKAGE_PIN G13 [get_ports ADC_CSn]
#set_property IOSTANDARD LVCMOS33 [get_ports ADC_CSn]
## ADC_MISO_A PIN (JA2 on B11) = J6.2
#set_property PACKAGE_PIN B11 [get_ports ADC_MISO_A]
#set_property IOSTANDARD LVCMOS33 [get_ports ADC_MISO_A]
## ADC_MISO_B PIN (JA3 on A11) = J6.3
#set_property PACKAGE_PIN A11 [get_ports ADC_MISO_B]
#set_property IOSTANDARD LVCMOS33 [get_ports ADC_MISO_B]
## ADC_SCLK PIN (JA4 on D12) = J6.4
#set_property PACKAGE_PIN D12 [get_ports ADC_SCLK]
#set_property IOSTANDARD LVCMOS33 [get_ports ADC_SCLK]
## ADC_IP_IRQ PIN (CK_IO6 on T15)
#set_property PACKAGE_PIN T15 [get_ports ADC_IP_IRQ]
#set_property IOSTANDARD LVCMOS33 [get_ports ADC_IP_IRQ]
## ADC_IRQ_N_DONE PIN (CK_IO4 on R12)
#set_property PACKAGE_PIN R12 [get_ports {ADC_IRQ_N_DONE[0]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {ADC_IRQ_N_DONE[0]}]
## ADC_IRQ_N_DONE (SPILT ON GPIO) PIN (CK_IO7 on T16)
#set_property PACKAGE_PIN T16 [get_ports {gpio2_io_o_0[5]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[5]}]

#AUDIO
# AUDIO PWM PIN (A7 JD.9 on H2) = SA J4.9 FW: AUDIO_PWM
set_property PACKAGE_PIN H2 [get_ports AUDIO_PWM]
set_property IOSTANDARD LVCMOS33 [get_ports AUDIO_PWM]
# AUDIO EN PIN (A7 JC.7 ON U14) = SA J1.7 FW: AUDIO_EN
set_property PACKAGE_PIN U14 [get_ports {gpio2_io_o_0[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[6]}]
set_property PULLTYPE PULLDOWN [get_ports {gpio2_io_o_0[6]}]

# INPUT SIGNAL SELECT
# SIG_SEL PIN (A7 JA.8 ON B18) = SA J6.8 FW: SIG_SEL
set_property PACKAGE_PIN B18 [get_ports {gpio2_io_o_0[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[7]}]

#TEST SIGNALS
#TEST_IO_0 PIN (A7 J4.6 ON M13)
set_property PACKAGE_PIN M13 [get_ports {gpio2_io_o_0[9]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[9]}]
#TEST_IO_1 PIN (A7 ON J4.8 ON R10)
set_property PACKAGE_PIN R10 [get_ports {gpio2_io_o_0[10]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[10]}]
#MB_CLK PIN (A7 J4.3 ON U16)
set_property PACKAGE_PIN U16 [get_ports MB_CLK]
set_property IOSTANDARD LVCMOS33 [get_ports MB_CLK]
# TEST_IRQ PIN (A7 J4.5 ON P14)
set_property PACKAGE_PIN P14 [get_ports TEST_IRQ]
set_property IOSTANDARD LVCMOS33 [get_ports TEST_IRQ]
# MB_RESET PIN(A7 J4.7 ON T11)
set_property PACKAGE_PIN T11 [get_ports MB_RST]
set_property IOSTANDARD LVCMOS33 [get_ports MB_RST]
# gpio2_io_o_0[5] PIN (A7 J4.10 ON R11) TEMP PLACEMENT
set_property PACKAGE_PIN R11 [get_ports {gpio2_io_o_0[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {gpio2_io_o_0[5]}]

# ARTY A7 LEDS:
# DDR3 CALIBRATION COMPLETE ACTIVE HIGH (LED_4 on H5)
set_property PACKAGE_PIN H5 [get_ports LED_4]
set_property IOSTANDARD LVCMOS33 [get_ports LED_4]
# GENERIC LED USE (LED_5 on J5)
#set_property PACKAGE_PIN J5 [get_ports {LED_5}]
#set_property IOSTANDARD LVCMOS33 [get_ports {LED_5}]
# GENERIC LED USE (LED_6 on T9)
#set_property PACKAGE_PIN T9 [get_ports {gpio_io_o_0[1]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {gpio_io_o_0[1]}]
## GENERIC LED USE (LED_7 on T10)
#set_property PACKAGE_PIN T10 [get_ports {gpio_io_o_0[2]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {gpio_io_o_0[2]}]

# QUAD SPI FLASH USED BY BOOTLOADER ONLY - LOCATED ON ARTY AS IC3 / IC4
# DQ 0
set_property PACKAGE_PIN K17 [get_ports QSPI_FLASH_BOOT_io0_io]
set_property IOSTANDARD LVCMOS33 [get_ports QSPI_FLASH_BOOT_io0_io]
# DQ 1
set_property PACKAGE_PIN K18 [get_ports QSPI_FLASH_BOOT_io1_io]
set_property IOSTANDARD LVCMOS33 [get_ports QSPI_FLASH_BOOT_io1_io]
# DQ 2
set_property PACKAGE_PIN L14 [get_ports QSPI_FLASH_BOOT_io2_io]
set_property IOSTANDARD LVCMOS33 [get_ports QSPI_FLASH_BOOT_io2_io]
# DQ 3
set_property PACKAGE_PIN M14 [get_ports QSPI_FLASH_BOOT_io3_io]
set_property IOSTANDARD LVCMOS33 [get_ports QSPI_FLASH_BOOT_io3_io]
# CS
set_property PACKAGE_PIN L13 [get_ports {QSPI_FLASH_BOOT_ss_io[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {QSPI_FLASH_BOOT_ss_io[0]}]
# NEEDED BITSTREAM SETTINGS FOR USING QUAD SPI FLASH (IC3 /IC4) AS STORAGE
# Enable Quad Mode
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
# Sets stable boot speed
set_property BITSTREAM.CONFIG.CONFIGRATE 33 [current_design]
# Makes the image smaller for faster loading
set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]

set_property CONFIG_MODE SPIx4 [current_design]
