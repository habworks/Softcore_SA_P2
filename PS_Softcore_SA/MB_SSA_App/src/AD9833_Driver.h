/******************************************************************************************************
 * @file            AD9833_Driver.h
 * @brief           Header file to support AD9833_Driver.c
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

#ifndef AD9833_DRIVER_H_
#define AD9833_DRIVER_H_
#ifdef __cplusplus
extern"C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "xspi.h"


// DEFINES
// INPUT
#define AD9833_MCLK         (25000000U)
// CONTROL REGISTER MAPPING
#define AD9833_CONTROL_REG  (uint16_t)(0x0000)
#define AD9833_FREQ0_REG    (uint16_t)(0x4000)
#define AD9833_FREQ1_REG    (uint16_t)(0x8000)
#define AD9833_PHASE0_REG   (uint16_t)(0xC000)
#define AD9833_PHASE1_REG   (uint16_t)(0xE000)
// CONTROL REGISTER BITS
#define AD9833_MODE_MASK    (uint16_t)(0x0001 << 1)
#define AD9833_DIV2_MASK    (uint16_t)(0x0001 << 3)
#define AD9833_OPBITEN_MASK (uint16_t)(0x0001 << 5)
#define AD9833_SLEEP12_MASK (uint16_t)(0x0001 << 6)
#define AD9833_SLEEP1_MASK  (uint16_t)(0x0001 << 7)
#define AD9833_RESET_MASK   (uint16_t)(0x0001 << 8)
#define AD9833_PSELECT_MASK (uint16_t)(0x0001 << 10)
#define AD9833_FSELECT_MASK (uint16_t)(0x0001 << 11)
#define AD9833_HLB_MASK     (uint16_t)(0x0001 << 12)
#define AD9833_B28_MASK     (uint16_t)(0x0001 << 13)
// CONSTANTS
// Phase:
#define AD9833_PHASE_BITS           (12U)
#define AD9833_MAX_PHASE_COUNT      (4095U)                 // 12b MAX COUNT VALUE
#define AD9833_MAX_PHASE_DEGREES    (360U)
#define AD9833_PHASE_RESOLUTION     ((double)(0.087890625)) // 360 / 2^12
// Frequency:
#define AD9833_FREQ_BITS            (28U)
#define AD9833_MAX_FREQ_COUNT       (268435455U)            // 28b MAX COUNT VALUE
#define AD9833_MAX_FREQ_HZ          (AD9833_MCLK / 2)   
#define AD9833_FREQ_RESOLUTION      ((double)(0.093132257)) // MCLK / 2^28


// TYPEDEFS AND ENUMS
typedef enum
{
    AD9833_CH0 = 0,
    AD9833_CH1
} Type_AD9833_Channel;

typedef enum
{
    AD9833_SINE = 0,
    AD9833_TRIANGLE,
    AD9833_SQUARE
} Type_Waveform;

typedef bool (*AD9833_CS_FunctionPointer)(bool);
typedef bool (*AD9833_TxRxFunctionPointer)(XSpi *, uint8_t, uint8_t *, uint8_t *, uint32_t, bool, AD9833_CS_FunctionPointer);
typedef struct
{
    AD9833_TxRxFunctionPointer  transmitReceive;
    AD9833_CS_FunctionPointer   chipSelect;
    XSpi                        *SPI_Handle;
    uint8_t                     CS_Number;
    uint32_t                    MCLK_Hz;
    uint16_t                    ControlReg;
    uint32_t                    FrequencyWord[2];
    uint16_t                    PhaseWord[2];
} Type_AD9833_Driver;


// FUNCTION  PROTOTYPES
bool init_AD9833(Type_AD9833_Driver *AD9833_Handle, XSpi *SPI_Handle, uint8_t CS_Number, uint32_t MCLK_Frequency, AD9833_TxRxFunctionPointer AD9833_TxRxFunction, AD9833_CS_FunctionPointer AD9833_CS_Function);
bool AD9833_Reset(Type_AD9833_Driver *AD9833_Handle, bool ResetEnable);
bool AD9833_SetFrequency(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, double Frequency_Hz);
bool AD9833_SetPhase(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, double Phase_Degrees);
bool AD9833_Enable(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel);
bool AD9833_Disable(Type_AD9833_Driver *AD9833_Handle);


#ifdef __cplusplus
}
#endif
#endif /* AD9833_DRIVER_H_ */