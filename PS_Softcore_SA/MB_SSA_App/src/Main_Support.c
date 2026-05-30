/******************************************************************************************************
 * @file            Main_Support.c
 * @brief           A collection of functions relevant to supporting both the main test and main application
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

#include "Main_Support.h"
#include "IO_Support.h"
#include "xgpio.h"
#include "xuartlite.h"
#include "xtmrctr.h"
#include "sleep.h"

extern XGpio AXI_GPIO_Handle;
#define GPIO_Handle AXI_GPIO_Handle


volatile uint32_t __attribute__ ((section (".Hab_Mixed_Data"))) ReceivedBytes = 0;
volatile uint8_t __attribute__ ((section (".Hab_Mixed_Data"))) RxDataBuffer[RX_BUFFER_SIZE] = {0};


/********************************************************************************************************
* @brief Blocking delay by tens of us
*
* @author original: Hab Collector \n
*
* @note: Requires BSP xiltimer to be active with dedicated timer for use
* 
* @param WaitTime: Number of 10s of us to wait: A value of 10 would wait 100us
*
* STEP 1: Wait 10s of us
********************************************************************************************************/
void sleep_10us_Wrapper(uint32_t WaitTime)
{
    // STEP 1: Wait 10s of us
    for (uint32_t Time = 0; Time < WaitTime; Time++)
    {
        usleep(10);
    }

} // END OF sleep_10us_Wrapper



/********************************************************************************************************
* @brief Blocking delay by milliseconds
*
* @author original: Hab Collector \n
*
* @note: Requires BSP xiltimer to be active with dedicated timer for use
* 
* @param WaitTime: Number of in milliseconds
********************************************************************************************************/
void sleep_ms_Wrapper(uint32_t WaitTime)
{
    msleep(WaitTime);

} // END OF sleep_ms_Wrapper



/********************************************************************************************************
* @brief This is communication for use with the QSPI interface that interfaces to the display (0 not used)
* IO expanders (1 and 2), and Waveform Generator (3).  As this is SPI it is full duplex transmit and receive.  
* It is passed by reference for use with the display and IO drivers.  This will generally be referred to as 
* the UI SPI Tx Rx.
*
* @author original: Hab Collector \n
*
* @note: SPI must be configured in polling mode
*
* @param SPI_UI_Handle: Pointer to QSPI handle
* @param ChipSelect_N: Chip Select associate with device first device is 1
* @param TxBuffer: The transmit buffer - information to send
* @param RxBuffer: The receive buffer - information to receive
* @param BytesToTransfer: Bytes to both transmit and receive (always equal for SPI)
* @param UseNegSpiClk: Sets the polarity of the SPI Clock = data active on positive or negative clock edge
* @param External_CS_Enable: If not using the CS of the AXI Quad SPI IP then this function pointer if not NULL should be used
*
* @return: True if transmission was successful
*
* STEP 1: Simple test
* STEP 2: Select the correct slave device
* STEP 3: Transfer the data
* STEP 4: Deselect all slave devices
********************************************************************************************************/
bool userInterfaceTrasmitReceive(XSpi *SPI_UI_Handle, uint8_t ChipSelect_N, uint8_t *TxBuffer, uint8_t *RxBuffer, uint32_t BytesToTransfer, bool UseNegSpiClk, CS_FunctionPointer External_CS_Enable)
{
    // STEP 1: Simple test
    if ((SPI_UI_Handle == NULL) || (ChipSelect_N == 0))
        return(false);

    // Set target options
    uint32_t TargetOptions = XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION;
    if (UseNegSpiClk)
        TargetOptions |= XSP_CLK_ACTIVE_LOW_OPTION;
    XSpi_SetOptions(SPI_UI_Handle, TargetOptions);

    // STEP 2: Select the correct slave device
    if (External_CS_Enable == NULL)
        XSpi_SetSlaveSelect(SPI_UI_Handle, ChipSelect_N);
    else
        External_CS_Enable(true);

    // STEP 3: Transfer the data
    int AXI_Status;
    uint8_t DummyRxBuffer[BytesToTransfer];
    uint8_t *RxBufferPtr;
    if (RxBuffer == NULL)
        RxBufferPtr = DummyRxBuffer;
    else
        RxBufferPtr = RxBuffer;


    AXI_Status = XSpi_Transfer(SPI_UI_Handle, TxBuffer, RxBufferPtr, BytesToTransfer);    
    if (AXI_Status != XST_SUCCESS)  // Something went wrong try again
    {
        // Reset the SPI and deselect the chip
        XSpi_Reset(SPI_UI_Handle);
        if (External_CS_Enable == NULL)
            XSpi_SetSlaveSelect(SPI_UI_Handle, ChipSelect_N);
        else
            External_CS_Enable(false);
        // Set the options and restart
        XSpi_SetOptions(SPI_UI_Handle,TargetOptions);
        XSpi_Start(SPI_UI_Handle);
        XSpi_IntrGlobalDisable(SPI_UI_Handle);
        // Enable chip select and repeat transfer
        if (External_CS_Enable == NULL)
            XSpi_SetSlaveSelect(SPI_UI_Handle, ChipSelect_N);
        else
            External_CS_Enable(true);
        AXI_Status = XSpi_Transfer(SPI_UI_Handle, TxBuffer, DummyRxBuffer, BytesToTransfer);        
    }

    // STEP 4: Deselect all slave devices
    if (External_CS_Enable == NULL)
        XSpi_SetSlaveSelect(SPI_UI_Handle, 0x00);
    else
        External_CS_Enable(false);

    return(AXI_Status == XST_SUCCESS);

} // END OF userInterfaceTrasmitReceive




