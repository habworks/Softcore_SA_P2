/******************************************************************************************************
 * @file            IO_Support.c
 * @brief           A collection of functions relevant IO use
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

#include "IO_Support.h"
#include "Hab_Types.h"

// Switches and push buttons as input
/********************************************************************************************************
* @brief Init of GPIO intputs and outputs.  All outputs are set to their default POR state.  Switches and 
* push buttons inputs SW_0, SW_1, PB_1, PB_2, PB_3 are all on the Arty A7 PCB
*
* @author original: Hab Collector \n
* 
* @note: This function must be called before any GPIO set and clear activity
*
* @param GPIO_Handle: Pointer to the GPIO handle
*
* STEP 1: Define input output direction
* STEP 2: Set the default POR output conditions
********************************************************************************************************/
void init_GPIO(XGpio *GPIO_Handle)
{
    // STEP 1: Define input output direction
    XGpio_SetDataDirection(GPIO_Handle, GPIO_INPUT_CHANNEL, 0xFFFF);  
    XGpio_SetDataDirection(GPIO_Handle, GPIO_OUTPUT_CHANNEL, 0x0000);

    // STEP 2: Set the default POR output conditions
    XGpio_DiscreteClear(GPIO_Handle, GPIO_OUTPUT_CHANNEL, TIMER_1_OUTPUT);
    XGpio_DiscreteClear(GPIO_Handle, GPIO_OUTPUT_CHANNEL, TIMER_2_OUTPUT);
    XGpio_DiscreteClear(GPIO_Handle, GPIO_OUTPUT_CHANNEL, DISPLAY_RESET_RUN);
    XGpio_DiscreteClear(GPIO_Handle, GPIO_OUTPUT_CHANNEL, DISPLAY_CMD_DATA);
    XGpio_DiscreteSet(GPIO_Handle, GPIO_OUTPUT_CHANNEL, DISPLAY_CS);
    XGpio_DiscreteClear(GPIO_Handle, GPIO_OUTPUT_CHANNEL, ADC_IRQ_N_DONE);
    XGpio_DiscreteClear(GPIO_Handle, GPIO_OUTPUT_CHANNEL, AUDIO_EN);
    XGpio_DiscreteClear(GPIO_Handle, GPIO_OUTPUT_CHANNEL, SIG_SEL);
    XGpio_DiscreteClear(GPIO_Handle, GPIO_OUTPUT_CHANNEL, IOX_RESET);
    XGpio_DiscreteClear(GPIO_Handle, GPIO_OUTPUT_CHANNEL, TEST_IO_0);
    XGpio_DiscreteSet(GPIO_Handle, GPIO_OUTPUT_CHANNEL, WAVEFORM_GEN_CS);

} // END OF init_GPIO



/********************************************************************************************************
* @brief Check if the uSD card is inserted in the uSD card holder
*
* @author original: Hab Collector \n
* 
* @param return: True if card is inserted
********************************************************************************************************/
bool is_MicroSD_Inserted(void)
{
    uint32_t SwitchState = XGpio_DiscreteRead(&AXI_GPIO_Handle, GPIO_INPUT_CHANNEL);
    return (SwitchState & USD_CD);

} // END OF is_MicroSD_Inserted



/********************************************************************************************************
* @brief rest the IO expanders.  This will reset both IO Exapander as they are tied together in HW.
*
* @author original: Hab Collector \n
* 
* @param Reset: Chip reset status
********************************************************************************************************/
void IOX_Reset(bool Reset)
{
    if (Reset)
        XGpio_DiscreteClear(&AXI_GPIO_Handle, GPIO_OUTPUT_CHANNEL, IOX_RESET);
    else
        XGpio_DiscreteSet(&AXI_GPIO_Handle, GPIO_OUTPUT_CHANNEL, IOX_RESET);

} // END OF IOX_Reset



void IOX_ChipSelect(bool Enable)
{
    NOT_USED(Enable);
    DO_NOTHING();
}



/********************************************************************************************************
* @brief Set or clear the output pin to disable or enable the AD9833 for SPI communication
*
* @author original: Hab Collector \n
* 
* @param Enable: Chip select enable status
********************************************************************************************************/
void WaveformGenChipSelect(bool Enable)
{
    if (Enable)
        XGpio_DiscreteClear(&AXI_GPIO_Handle, GPIO_OUTPUT_CHANNEL, WAVEFORM_GEN_CS);
    else
        XGpio_DiscreteSet(&AXI_GPIO_Handle, GPIO_OUTPUT_CHANNEL, WAVEFORM_GEN_CS);

} // END OF WaveformGenChipSelect



/********************************************************************************************************
* @brief Set or clear the output pin to Run or reset the display for SSD1309 Display
*
* @author original: Hab Collector \n
* 
* @param ResetRunAction: Reset or run action to take
********************************************************************************************************/
void displayResetOrRun(Type_DisplayResetRun ResetRunAction)
{
    if (ResetRunAction == DISPLAY_RUN)
        XGpio_DiscreteSet(&AXI_GPIO_Handle, GPIO_OUTPUT_CHANNEL, DISPLAY_RESET_RUN);
    else
        XGpio_DiscreteClear(&AXI_GPIO_Handle, GPIO_OUTPUT_CHANNEL, DISPLAY_RESET_RUN);

} // END OF displayResetOrRun



/********************************************************************************************************
* @brief Set or clear the output pin to Data or Command mode for SSD1309 Display
*
* @author original: Hab Collector \n
* 
* @param CommandDataAction: Data or command action to take
********************************************************************************************************/
void displayCommandOrData(Type_DisplayCommandData CommandDataAction)
{
    if (CommandDataAction == DISPLAY_DATA)
        XGpio_DiscreteSet(&AXI_GPIO_Handle, GPIO_OUTPUT_CHANNEL, DISPLAY_CMD_DATA);
    else
        XGpio_DiscreteClear(&AXI_GPIO_Handle, GPIO_OUTPUT_CHANNEL, DISPLAY_CMD_DATA);

} // END OF displayCommandOrData



/********************************************************************************************************
* @brief Set or clear the output pin to disable or enable the SSD1309 Display for SPI communication
*
* @author original: Hab Collector \n
* 
* @param DisplaySelect: Display select action to take
********************************************************************************************************/
void displayChipSelect(Type_Display_CS DisplaySelect)
{
    if (DisplaySelect == CS_ENABLE)
        XGpio_DiscreteClear(&AXI_GPIO_Handle, GPIO_OUTPUT_CHANNEL, DISPLAY_CS);
    else
        XGpio_DiscreteSet(&AXI_GPIO_Handle, GPIO_OUTPUT_CHANNEL, DISPLAY_CS);

} // END OF displayChipSelect