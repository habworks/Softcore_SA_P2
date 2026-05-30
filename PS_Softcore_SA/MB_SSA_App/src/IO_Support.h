/******************************************************************************************************
 * @file            IO_Support.h
 * @brief           Header file to support IO_Support.c (if part of code base or can be used alone)
 * ****************************************************************************************************
 * @author          Hab Collector (habco)\n
 *
 * @version         See Main_Support.h: FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV
 *
 * @param Development_Environment \n
 * Hardware:        <Xilinx Artix A7> \n
 * IDE:             Vitis 2024.2 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            The associated header file provides MACRO functions for IO control
 *
 *                  This is an embedded application
 *                  It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *                    Schematic: 
 *                    IMR Engineering
 *                    IMR Engineering
 *
 * @copyright       IMR Engineering, LLC
 ********************************************************************************************************/

#ifndef IO_SUPPORT_H_
#define IO_SUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "xgpio.h"
#include "SSD1309_Driver.h"


// USED IN IO ACCESS
#define GPIO_INPUT_CHANNEL      1          
#define GPIO_OUTPUT_CHANNEL     2  
// DEFINES GPIO BIT MASK: INPUTS
#define SW_0                ((uint32_t)(0x01 << 0))
#define SW_1                ((uint32_t)(0x01 << 1))
#define PB_1                ((uint32_t)(0x01 << 2))
#define PB_2                ((uint32_t)(0x01 << 3))
#define PB_3                ((uint32_t)(0x01 << 4))
#define USD_CD              ((uint32_t)(0x01 << 5))
#define DDR_CALIB_COMPLETE  ((uint32_t)(0x01 << 6))
#define IOX_2_IRQ           ((uint32_t)(0x01 << 7))
#define HW_PL_VER_MASK      ((uint32_t)(0x0F << 8))
#define HW_PL_VER_OFFSET    8U
#define ARTY_A7_UI          (SW_0 | SW_1 | PB_1 | PB_2 | PB_3)

// DEFINES GPIO BIT MASK: OUTPUTS                       // A7    - SSA
#define TIMER_1_OUTPUT      ((uint32_t)(0x01 << 0))     // J4.2
#define TIMER_2_OUTPUT      ((uint32_t)(0x01 << 1))     // J4.4
#define DISPLAY_RESET_RUN   ((uint32_t)(0x01 << 2))     // JD.3  - J4.3
#define DISPLAY_CMD_DATA    ((uint32_t)(0x01 << 3))     // JD.2  - J4.2
#define DISPLAY_CS          ((uint32_t)(0x01 << 4))     // JD.1  - J4.1
#define ADC_IRQ_N_DONE      ((uint32_t)(0x01 << 5))     // JD.9  - J4.9
#define AUDIO_EN            ((uint32_t)(0x01 << 6))     // JC.7  - J1.7
#define SIG_SEL             ((uint32_t)(0x01 << 7))     // JA.8  - J6.8  
#define IOX_RESET           ((uint32_t)(0x01 << 8))     // JC.4  - J1.4
#define TEST_IO_0           ((uint32_t)(0x01 << 9))     // J4.6
#define WAVEFORM_GEN_CS     ((uint32_t)(0x01 << 10))    // JB.10 - J3.10


// EXTERN
extern XGpio AXI_GPIO_Handle;

// FUNCTION PROTOTYPES
void init_GPIO(XGpio *GPIO_Handle);
bool is_MicroSD_Inserted(void);
void IOX_Reset(bool Status);
void IOX_ChipSelect(bool Enable);
void WaveformGenChipSelect(bool Enable);
void displayResetOrRun(Type_DisplayResetRun ResetRunAction);
void displayCommandOrData(Type_DisplayCommandData CommandDataAction);
void displayChipSelect(Type_Display_CS DisplaySelect);

#ifdef __cplusplus
}
#endif
#endif /* IO_SUPPORT_H_ */