/******************************************************************************************************
 * @file            AD9833_Driver.c
 * @brief           A collection of functions relevant to supporting the AD9833 DDS Programmable Waveform Generator
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

 #include "AD9833_Driver.h"


static bool AD9833_WriteControl(Type_AD9833_Driver *AD9833_Handle, uint16_t ControlRegister);
static bool AD9833_WriteFrequencyCount(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, const uint32_t FrequencyCount);
static bool AD9833_WritePhaseCount(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, const uint16_t PhaseCount);


/********************************************************************************************************
* @brief Write to the control register.  On success struct member for the control register is updated. The
* control register is 16b
*
* @author original: Hab Collector \n
* 
* @note: AD9833 must be init before use
* @note: Only 16b transfers are allowed
*
* @param AD9833_Handle: Pointer to the AD9833 handle
* @param ControlRegister: Register value to write
*
* @return True if write OK
*
* STEP 1: Simple test
* STEP 2: Writes to the control register - D15 and D14 must be 0 - make sure it is so
* STEP 3: Send data MSB First
********************************************************************************************************/
static bool AD9833_WriteControl(Type_AD9833_Driver *AD9833_Handle, uint16_t ControlRegister)
{
    uint8_t TxBuffer[sizeof(uint16_t)];
    uint8_t RxBuffer[sizeof(uint16_t)] = {0};

    // STEP 1: Simple test
    if (AD9833_Handle == NULL)
        return(false);
    
    // STEP 2: Writes to the control register - D15 and D14 must be 0 - make sure it is so
    if ((ControlRegister & 0xC000) != 0x0000)
        return(false);

    // STEP 2: Send data MSB First
    TxBuffer[1] = (uint8_t)(ControlRegister & 0x00FF);
    TxBuffer[0] = (uint8_t)(ControlRegister >> 8);

    if (AD9833_Handle->transmitReceive(AD9833_Handle->SPI_Handle, AD9833_Handle->CS_Number, TxBuffer, RxBuffer, (uint32_t)sizeof(TxBuffer), true, AD9833_Handle->chipSelect))
    {
        AD9833_Handle->ControlReg = ControlRegister;
        return(true);
    }
    else
    {
        return(false);
    }

} // END OF AD9833_WriteControl



/********************************************************************************************************
* @brief Write the frequency count to the selected channel.  There are two frequency channels and the frequency
* count is 28b, therefore it takes 2 writes to update the frequency count.  Each write consist of 14bits lower
* LSb and with the upper two bits being the channel select.  On success the strut member for that frequency
* count channel is updated.  This function leaves the device in reset mode (reset mode only means no output)
*
* @author original: Hab Collector \n
* 
* @note: AD9833 must be init before use
*
* @param AD9833_Handle: Pointer to the AD9833 handle
* @param Channel: One of two channels
* @param FrequencyCount: Frequency count - 28b value max value represents AD9833_MAX_FREQ_HZ
*
* @return True OK
*
* STEP 1: Simple test
* STEP 2: Set device in reset for update to desired Channel
* STEP 3: Build 28b FrequencyCount word with channel destination
* STEP 4: Load and transmit the Tx Buffer LSB part of Frequency Register First
* STEP 5: Load and transmit the Tx Buffer MSB part of Frequency Register Second
********************************************************************************************************/
static bool AD9833_WriteFrequencyCount(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, const uint32_t FrequencyCount)
{
    uint8_t TxBuffer[sizeof(uint16_t)];
    uint8_t RxBuffer[sizeof(uint16_t)] = {0};
    uint16_t FrequencyRegister_MSB;
    uint16_t FrequencyRegister_LSB;

    // STEP 1: Simple test
    if (((Channel != AD9833_CH0) && (Channel != AD9833_CH1)) ||  (AD9833_Handle == NULL))
        return(false);
    if (FrequencyCount > AD9833_MAX_FREQ_COUNT)
        return(false);

    // STEP 2: Set device in reset for update to desired Channel
    uint16_t ControlRegister = AD9833_Handle->ControlReg;
    ControlRegister |= AD9833_B28_MASK | AD9833_RESET_MASK;
    if (!AD9833_WriteControl(AD9833_Handle, ControlRegister))
        return(false);

    // STEP 3: Build 28b FrequencyCount word with channel destination
    if (Channel == AD9833_CH0)
    {
        FrequencyRegister_MSB = AD9833_FREQ0_REG | (uint16_t)((FrequencyCount >> 14) & 0x3FFF); // Take the top 14bits of FrequencyCount - FrequencyCount Value is 28b
        FrequencyRegister_LSB = AD9833_FREQ0_REG | (uint16_t)(FrequencyCount & 0x00003FFF); // Take lower 14bits - FrequencyCount value is 28b
    }
    else
    {
        FrequencyRegister_MSB = AD9833_FREQ1_REG | (uint16_t)((FrequencyCount >> 14) & 0x3FFF); // Take the top 14bits of FrequencyCount - FrequencyCount Value is 28b
        FrequencyRegister_LSB = AD9833_FREQ1_REG | (uint16_t)(FrequencyCount & 0x00003FFF); // Take lower 14bits - FrequencyCount value is 28b
    }

    // STEP 4: Load and transmit the Tx Buffer LSB part of Frequency Register First
    TxBuffer[0] = (uint8_t)(FrequencyRegister_LSB >> 8);
    TxBuffer[1] = (uint8_t)(FrequencyRegister_LSB & 0x00FF);
    if (!AD9833_Handle->transmitReceive(AD9833_Handle->SPI_Handle, AD9833_Handle->CS_Number, TxBuffer, RxBuffer, (uint32_t)sizeof(TxBuffer), true, AD9833_Handle->chipSelect))
        return(false);
    
    // STEP 5: Load and transmit the Tx Buffer MSB part of Frequency Register Second
    TxBuffer[0] = (uint8_t)(FrequencyRegister_MSB >> 8);
    TxBuffer[1] = (uint8_t)(FrequencyRegister_MSB & 0x00FF);
    if (AD9833_Handle->transmitReceive(AD9833_Handle->SPI_Handle, AD9833_Handle->CS_Number, TxBuffer, RxBuffer, (uint32_t)sizeof(TxBuffer), true, AD9833_Handle->chipSelect))
    {
        AD9833_Handle->FrequencyWord[Channel] = FrequencyCount;
        AD9833_Handle->ControlReg = ControlRegister;
        return(true);
    }
    else
    {
        return(false);
    }

} // END OF AD9833_WriteFrequencyCount



/********************************************************************************************************
* @brief Write the phase count to the selected channel.  There are two phase channels and the phase
* count is 12b, therefore it takes 1 write to update the phase count.  The write consist of 12bits lower
* LSb and with the upper three bits being the channel select.  On success the strut member for that phase
* count channel is updated.  This function leaves the device in reset mode (reset mode only means no output)
*
* @author original: Hab Collector \n
* 
* @note: AD9833 must be init before use
*
* @param AD9833_Handle: Pointer to the AD9833 handle
* @param Channel: One of two channels
* @param PhaseCount: Phase count - 12b value max value represents 360 degrees
*
* @return True OK
*
* STEP 1: Simple test
* STEP 2: Set device in reset for update to desired Channel
* STEP 3: Build the 12b PhaseCountWord with channel destination
* STEP 4: Load Tx buffer MSB first
* STEP 5: Transmit 16b value to AD9833 MSB first
********************************************************************************************************/
static bool AD9833_WritePhaseCount(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, const uint16_t PhaseCount)
{
    uint8_t TxBuffer[sizeof(uint16_t)];
    uint8_t RxBuffer[sizeof(uint16_t)] = {0};
    uint16_t PhaseCountRegister;

    // STEP 1: Simple test
    if (((Channel != AD9833_CH0) && (Channel != AD9833_CH1)) ||  (AD9833_Handle == NULL))
        return(false);
    if (PhaseCount > AD9833_MAX_PHASE_COUNT)
        return(false);

    // STEP 2: Set device in reset for update to desired Channel
    uint16_t ControlRegister = AD9833_Handle->ControlReg;
    ControlRegister |= AD9833_B28_MASK | AD9833_RESET_MASK;
    if (!AD9833_WriteControl(AD9833_Handle, ControlRegister))
        return(false);

    // STEP 3: Build the 12b PhaseCountWord with channel destination
    if (Channel == AD9833_CH0)
        PhaseCountRegister = AD9833_PHASE0_REG | (PhaseCount & 0x0FFF);
    else
        PhaseCountRegister = AD9833_PHASE1_REG | (PhaseCount & 0x0FFF);

    // STEP 4: Load Tx buffer MSB first
    TxBuffer[0] = (uint8_t)(PhaseCountRegister >> 8);
    TxBuffer[1] = (uint8_t)(PhaseCountRegister & 0x00FF);

    // STEP 5: Transmit 16b value to AD9833 MSB first
    if (AD9833_Handle->transmitReceive(AD9833_Handle->SPI_Handle, AD9833_Handle->CS_Number, TxBuffer, RxBuffer, (uint32_t)sizeof(TxBuffer), true, AD9833_Handle->chipSelect))
    {
        AD9833_Handle->PhaseWord[Channel] = PhaseCount;
        AD9833_Handle->ControlReg = ControlRegister;
        return(true);
    }
    else
    {
        return(false);
    }

} // END OF AD9833_WritePhaseCount



/********************************************************************************************************
* @brief Init of the AD9833 for use.  This funciton must be called before any of the drivers APIs.  This
* function leaves the device in reset mode (reset mode only means no output) with all registers set to the
* default start values: Control in reset, Phase Count = 0, Frequency Count = 0 both channels.
*
* @author original: Hab Collector \n
* 
* @note: AD9833 must be init before use
*
* @param AD9833_Handle: Pointer to the AD9833 handle
* @param SPI_Handle: Pointer to the bus handle 
* @param CS_Number: When a shared SPI controller is used this is the chip select number associated with the AD9833 device
* @param MCLK_Frequency: Master Clock frequency input to AD9833
* @param AD9833_TxRxFunction: Function pointer to a transmit receive function
*
* @return True OK
*
* STEP 1: Assign struct member values
* STEP 2: Reset the device
* STEP 3: Update the frequency and PhaseCount registers
********************************************************************************************************/
bool init_AD9833(Type_AD9833_Driver *AD9833_Handle, XSpi *SPI_Handle, uint8_t CS_Number, uint32_t MCLK_Frequency, AD9833_TxRxFunctionPointer AD9833_TxRxFunction, AD9833_CS_FunctionPointer AD9833_CS_Function)
{
    // STEP 1: Assign struct member values
    AD9833_Handle->SPI_Handle = SPI_Handle;
    AD9833_Handle->CS_Number = CS_Number;
    AD9833_Handle->MCLK_Hz = MCLK_Frequency;
    AD9833_Handle->transmitReceive = AD9833_TxRxFunction;
    AD9833_Handle->chipSelect = AD9833_CS_Function;
    // Set frequency and PhaseCount to 0
    AD9833_Handle->FrequencyWord[AD9833_CH0] = 0;
    AD9833_Handle->FrequencyWord[AD9833_CH1] = 0;
    AD9833_Handle->PhaseWord[AD9833_CH0] = 0;
    AD9833_Handle->PhaseWord[AD9833_CH1] = 0;

    // STEP 2: Reset the device
    AD9833_Reset(AD9833_Handle, true);

    // STEP 3: Update the frequency and PhaseCount registers
    if (!AD9833_WriteFrequencyCount(AD9833_Handle, AD9833_CH0, 0))
        return(false);
    if (!AD9833_WriteFrequencyCount(AD9833_Handle, AD9833_CH1, 0))
        return(false);
    if (!AD9833_WritePhaseCount(AD9833_Handle, AD9833_CH0, 0))
        return(false);
    if (!AD9833_WritePhaseCount(AD9833_Handle, AD9833_CH1, 0))
        return(false);
    else
        return(true);

} // END OF init_AD9833



/********************************************************************************************************
* @brief This function leaves the device in reset mode (reset mode only means no output) or active.  On
* success the strct member for the Control Register is updated.
*
* @author original: Hab Collector \n
* 
* @note: AD9833 must be init before use
*
* @param AD9833_Handle: Pointer to the AD9833 handle
* @param ResetEnable: If true device will be in reset mode, if not it will be active
*
* @return True OK
*
* STEP 1: Assign struct member values
* STEP 2: Reset the device
* STEP 3: Update the frequency and PhaseCount registers
********************************************************************************************************/
bool AD9833_Reset(Type_AD9833_Driver *AD9833_Handle, bool ResetEnable)
{
    uint16_t RegisterValue;

    // STEP 1: 
    if (ResetEnable)
        RegisterValue = AD9833_B28_MASK | AD9833_RESET_MASK;
    else
        RegisterValue = AD9833_B28_MASK;
    
    if (AD9833_WriteControl(AD9833_Handle, RegisterValue))
    {
        AD9833_Handle->ControlReg = RegisterValue;
        return(true);
    }
    else
        return(false);

} // END OF AD9833_Reset



/********************************************************************************************************
* @brief The AD9833 is capble of outputing one of 3 different waveforms: sine, triangle, or square.  This 
* selects the waveform output.  On sucess the control register is updated.  This function leaves the device 
* in reset mode (reset mode only means no output).
*
* @author original: Hab Collector \n
* 
* @note: AD9833 must be init before use
*
* @param AD9833_Handle: Pointer to the AD9833 handle
* @param Waveform: Waveform type desired
* @param SquareWaveDivideByTwo: If square wave will the frequency be 1/2
*
* @return True OK
*
* STEP 1: Assign struct member values
* STEP 2: Build baseline control register
* STEP 3: Configure desired waveform
* STEP 4: Write control register to device
********************************************************************************************************/
bool AD9833_SetWaveformType(Type_AD9833_Driver *AD9833_Handle, Type_Waveform Waveform, bool SquareWaveDivideByTwo)
{
    uint16_t ControlRegister;

    // STEP 1: Simple test
    if (AD9833_Handle == NULL)
        return(false);
    if (Waveform > AD9833_SQUARE)
        return(false);

    // STEP 2: Build baseline control register
    ControlRegister = AD9833_Handle->ControlReg & ~(AD9833_MODE_MASK | AD9833_OPBITEN_MASK | AD9833_DIV2_MASK);

    // STEP 3: Configure desired waveform
    switch (Waveform)
    {
        case AD9833_SINE:
        {
            // Default mode
            // MODE = 0
            // OPBITEN = 0
        }
        break;

        case AD9833_TRIANGLE:
        {
            ControlRegister |= AD9833_MODE_MASK;
        }
        break;

        case AD9833_SQUARE:
        {
            if (SquareWaveDivideByTwo)
                ControlRegister |= AD9833_OPBITEN_MASK | AD9833_DIV2_MASK;
            else
                ControlRegister |= AD9833_OPBITEN_MASK;
        }
        break;

        default:
        {
            return(false);
        }
    } // END CASE
    
    // STEP 4: Write control register to device
    if (AD9833_WriteControl(AD9833_Handle, ControlRegister))
    {
        AD9833_Handle->ControlReg = ControlRegister;
        return(true);
    }
    else
    {
        return(false);
    }

} // END OF AD9833_SetWaveformType



/********************************************************************************************************
* @brief Set the desired frequency in Hz for the desired channel
*
* @author original: Hab Collector \n
* 
* @note: AD9833 must be init before use
*
* @param AD9833_Handle: Pointer to the AD9833 handle
* @param Channel: Desired channel to update
* @param Frequency_Hz: Frequency in Hz
*
* @return True OK
*
* STEP 1: Simple test
* STEP 2: Calculate frequency in counts from Hz
* STEP 3: Set the frequency
********************************************************************************************************/
bool AD9833_SetFrequency(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, double Frequency_Hz)
{
    // STEP 1: Simple test
    if (((Channel != AD9833_CH0) && (Channel != AD9833_CH1)) ||  (AD9833_Handle == NULL))
        return(false);
    if (Frequency_Hz > AD9833_MAX_FREQ_HZ)
        return(false);
    
    // STEP 2: Calculate frequency in counts from Hz
    double FrequencyCounts = Frequency_Hz / AD9833_FREQ_RESOLUTION;

    // STEP 3: Set the frequency
    bool Status = AD9833_WriteFrequencyCount(AD9833_Handle, Channel, (uint32_t)FrequencyCounts);
    return(Status);

} // END OF AD9833_SetFrequency



/********************************************************************************************************
* @brief Set the desired phase in degress for the desired channel
*
* @author original: Hab Collector \n
* 
* @note: AD9833 must be init before use
*
* @param AD9833_Handle: Pointer to the AD9833 handle
* @param Channel: Desired channel to update
* @param Phase_Degrees: Phase in degrees 0 to 360
*
* @return True OK
*
* STEP 1: Simple test
* STEP 2: Calculate PhaseCount in counts from degrees
* STEP 3: Set the phase
********************************************************************************************************/
bool AD9833_SetPhase(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, double Phase_Degrees)
{
    // Simple test
    if (((Channel != AD9833_CH0) && (Channel != AD9833_CH1)) ||  (AD9833_Handle == NULL))
        return(false);
    if (Phase_Degrees > AD9833_MAX_PHASE_DEGREES)
        return(false);

    // STEP 2: Calculate PhaseCount in counts from degrees
    double Phase_Count = Phase_Degrees / AD9833_PHASE_RESOLUTION;

    // STEP 3: Set the phase
    bool Status = AD9833_WritePhaseCount(AD9833_Handle, Channel, (uint16_t)Phase_Count);
    return(Status);
    
} // END OF AD9833_SetPhase



/********************************************************************************************************
* @brief Enable the desired output channel  
*
* @author original: Hab Collector \n
* 
* @note: AD9833 must be init before use
*
* @param AD9833_Handle: Pointer to the AD9833 handle
* @param Channel: Desired channel to enable 
*
* @return True OK
*
* STEP 1: Simple test
* STEP 2: Configure Control Register according to channel
* STEP 3: Enable action
********************************************************************************************************/
bool AD9833_Enable(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel)
{
    // STEP 1: Simple test
    if (((Channel != AD9833_CH0) && (Channel != AD9833_CH1)) ||  (AD9833_Handle == NULL))
        return(false);

    // STEP 2: Configure Control Register according to channel
    uint16_t ControlRegister = AD9833_Handle->ControlReg;
    ControlRegister &= ~(AD9833_SLEEP1_MASK | AD9833_SLEEP12_MASK); // Disables Sleep Mode
    ControlRegister &= ~AD9833_RESET_MASK; // Enable output
    if (Channel == AD9833_CH0)     
        ControlRegister &= ~(AD9833_PSELECT_MASK | AD9833_FSELECT_MASK); // Set for channel 0 frequency and phase
    else 
        ControlRegister |= (AD9833_PSELECT_MASK | AD9833_FSELECT_MASK); // Set for channel 1 frequency and phase

    // STEP 3: Enable action
    if (AD9833_WriteControl(AD9833_Handle, ControlRegister))
    {
        AD9833_Handle->ControlReg = ControlRegister;
        return(true);
    }
    else 
    {
        return(false);
    }

} // END OF AD9833_Enable



/********************************************************************************************************
* @brief Disable the desired output channel.  Disable is a form of device deep sleep. 
*
* @author original: Hab Collector \n
* 
* @note: AD9833 must be init before use
*
* @param AD9833_Handle: Pointer to the AD9833 handle
* @param Channel: Desired channel to disable 
*
* @return True OK
*
* STEP 1: Simple test
* STEP 2: Configure Control Register to put device into reset and  deep sleep - this disables the output
* STEP 3: Disable action
********************************************************************************************************/
bool AD9833_Disable(Type_AD9833_Driver *AD9833_Handle)
{
    // STEP 1: Simple test
    if (AD9833_Handle == NULL)
        return(false);

    // STEP 2: Configure Control Register to put device into reset and  deep sleep - this disables the output
    uint16_t ControlRegister = AD9833_Handle->ControlReg;
    ControlRegister |= AD9833_RESET_MASK | AD9833_SLEEP1_MASK | AD9833_SLEEP12_MASK;

    // STEP 3: Disable action
    bool Status = AD9833_WriteControl(AD9833_Handle, ControlRegister);
    return(Status);

} // END OF AD9833_Disable