/******************************************************************************************************
 * @file            AD9833_Driver.c
 * @brief           A collection of functions relevant to supporting the DDS Programmable Waveform Generator
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


static bool AD9833_WriteControl(Type_AD9833_Driver *AD9833_Handle, uint16_t ControlRegister)
{
    uint8_t TxBuffer[sizeof(uint16_t)];
    uint8_t RxBuffer[sizeof(uint16_t)] = {0};

    // Send data MSB First
    TxBuffer[1] = (uint8_t)(ControlRegister & 0x00FF);
    TxBuffer[0] = (uint8_t)(ControlRegister >> 8);

    if (AD9833_Handle->transmitReceive(AD9833_Handle->SPI_Handle, AD9833_Handle->CS_Number, TxBuffer, RxBuffer, (uint32_t)sizeof(TxBuffer)))
    {
        AD9833_Handle->ControlReg = ControlRegister;
        return(true);
    }
    else
    {
        return(false);
    }

} // END OF AD9833_WriteControl


static bool AD9833_WriteFrequencyCount(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, const uint32_t FrequencyCount)
{
    uint8_t TxBuffer[sizeof(uint16_t)];
    uint8_t RxBuffer[sizeof(uint16_t)] = {0};
    uint16_t FrequencyRegister_MSB;
    uint16_t FrequencyRegister_LSB;

    // Simple test
    if (((Channel != AD9833_CH0) && (Channel != AD9833_CH1)) ||  (AD9833_Handle == NULL))
        return(false);
    if (FrequencyCount > AD9833_MAX_FREQ_COUNT)
        return(false);

    // Set device in reset for update to desired Channel
    uint16_t ControlRegister = AD9833_Handle->ControlReg;
    ControlRegister |= AD9833_B28_MASK | AD9833_RESET_MASK;
    if (!AD9833_WriteControl(AD9833_Handle, ControlRegister))
        return(false);

    // Build 28b FrequencyCount word with channel destination
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

    // Load and transmit the Tx Buffer LSB part of Frequency Register First
    TxBuffer[0] = (uint8_t)(FrequencyRegister_LSB >> 8);
    TxBuffer[1] = (uint8_t)(FrequencyRegister_LSB & 0x00FF);
    if (!AD9833_Handle->transmitReceive(AD9833_Handle->SPI_Handle, AD9833_Handle->CS_Number, TxBuffer, RxBuffer, (uint32_t)sizeof(TxBuffer)))
        return(false);
    
    // Load and transmit the Tx Buffer MSB part of Frequency Register Second
    TxBuffer[0] = (uint8_t)(FrequencyRegister_MSB >> 8);
    TxBuffer[1] = (uint8_t)(FrequencyRegister_MSB & 0x00FF);
    if (AD9833_Handle->transmitReceive(AD9833_Handle->SPI_Handle, AD9833_Handle->CS_Number, TxBuffer, RxBuffer, (uint32_t)sizeof(TxBuffer)))
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


static bool AD9833_WritePhaseCount(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, const uint16_t PhaseCount)
{
    uint8_t TxBuffer[sizeof(uint16_t)];
    uint8_t RxBuffer[sizeof(uint16_t)] = {0};
    uint16_t PhaseCountRegister;

    // Simple test
    if (((Channel != AD9833_CH0) && (Channel != AD9833_CH1)) ||  (AD9833_Handle == NULL))
        return(false);
    if (PhaseCount > AD9833_MAX_PHASE_COUNT)
        return(false);

    // Set device in reset for update to desired Channel
    uint16_t ControlRegister = AD9833_Handle->ControlReg;
    ControlRegister |= AD9833_B28_MASK | AD9833_RESET_MASK;
    if (!AD9833_WriteControl(AD9833_Handle, ControlRegister))
        return(false);

    // Build the 12b PhaseCountWord with channel destination
    if (Channel == AD9833_CH0)
        PhaseCountRegister = AD9833_PHASE0_REG | (PhaseCount & 0x0FFF);
    else
        PhaseCountRegister = AD9833_PHASE1_REG | (PhaseCount & 0x0FFF);

    // Load Tx buffer MSB first
    TxBuffer[0] = (uint8_t)(PhaseCountRegister >> 8);
    TxBuffer[1] = (uint8_t)(PhaseCountRegister & 0x00FF);

    // Transmit 16b value to AD9833 MSB first
    if (AD9833_Handle->transmitReceive(AD9833_Handle->SPI_Handle, AD9833_Handle->CS_Number, TxBuffer, RxBuffer, (uint32_t)sizeof(TxBuffer)))
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


bool init_AD9833(Type_AD9833_Driver *AD9833_Handle, XSpi *SPI_Handle, uint8_t CS_Number, uint32_t MCLK_Frequency, TxRxFunctionPointer AD9833_TxRxFunction)
{
    // STEP 1: Assign struct member values
    AD9833_Handle->SPI_Handle = SPI_Handle;
    AD9833_Handle->CS_Number = CS_Number;
    AD9833_Handle->MCLK_Hz = MCLK_Frequency;
    AD9833_Handle->transmitReceive = AD9833_TxRxFunction;
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


bool AD9833_Reset(Type_AD9833_Driver *AD9833_Handle, bool ResetEnable)
{
    uint16_t RegisterValue;

    if (ResetEnable)
        RegisterValue = AD9833_B28_MASK | AD9833_RESET_MASK;
    else
        RegisterValue = AD9833_B28_MASK;
    
    bool Status = AD9833_WriteControl(AD9833_Handle, RegisterValue);
    return(Status);

} // END OF AD9833_Reset


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
    
    // STEP 5: Write control register to device
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



bool AD9833_SetFrequency(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, double Frequency_Hz)
{
    // Simple test
    if (((Channel != AD9833_CH0) && (Channel != AD9833_CH1)) ||  (AD9833_Handle == NULL))
        return(false);
    if (Frequency_Hz > AD9833_MAX_FREQ_HZ)
        return(false);
    
    // Calculate frequency in counts from Hz
    double FrequencyCounts = Frequency_Hz / AD9833_FREQ_RESOLUTION;

    // Set the frequency
    bool Status = AD9833_WriteFrequencyCount(AD9833_Handle, Channel, (uint32_t)FrequencyCounts);
    return(Status);

} // END OF AD9833_SetFrequency


bool AD9833_SetPhase(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel, double Phase_Degrees)
{
    // Simple test
    if (((Channel != AD9833_CH0) && (Channel != AD9833_CH1)) ||  (AD9833_Handle == NULL))
        return(false);
    if (Phase_Degrees > AD9833_MAX_PHASE_DEGREES)
        return(false);

    // Calculate PhaseCount in counts from degrees
    double Phase_Count = Phase_Degrees / AD9833_PHASE_RESOLUTION;

    // Set the phase
    bool Status = AD9833_WritePhaseCount(AD9833_Handle, Channel, (uint16_t)Phase_Count);
    return(Status);
    
} // END OF AD9833_SetPhase


bool AD9833_Enable(Type_AD9833_Driver *AD9833_Handle, Type_AD9833_Channel Channel)
{
    // Simple test
    if (((Channel != AD9833_CH0) && (Channel != AD9833_CH1)) ||  (AD9833_Handle == NULL))
        return(false);

    // Configure Control Register according to channel
    uint16_t ControlRegister = AD9833_Handle->ControlReg;
    ControlRegister &= ~(AD9833_SLEEP1_MASK | AD9833_SLEEP12_MASK); // Disables Sleep Mode
    ControlRegister &= ~AD9833_RESET_MASK; // Enable output
    if (Channel == AD9833_CH0)     
        ControlRegister &= ~(AD9833_PSELECT_MASK | AD9833_FSELECT_MASK); // Set for channel 0 frequency and phase
    else 
        ControlRegister |= (AD9833_PSELECT_MASK | AD9833_FSELECT_MASK); // Set for channel 1 frequency and phase

    // Enable action
    bool Status = AD9833_WriteControl(AD9833_Handle, ControlRegister);
    return(Status);

} // END OF AD9833_Enable


bool AD9833_Disable(Type_AD9833_Driver *AD9833_Handle)
{
    // Simple test
    if (AD9833_Handle == NULL)
        return(false);

    // Configure Control Register to put device into reset and  deep sleep - this disables the output
    uint16_t ControlRegister = AD9833_Handle->ControlReg;
    ControlRegister |= AD9833_RESET_MASK | AD9833_SLEEP1_MASK | AD9833_SLEEP12_MASK;

    // Disable action
    bool Status = AD9833_WriteControl(AD9833_Handle, ControlRegister);
    return(Status);

} // END OF AD9833_Disable