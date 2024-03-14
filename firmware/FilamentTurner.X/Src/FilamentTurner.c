/**************************************************************************//**
 * \file FilamentTurner.c
 * \author Roman Holderried
 *
 * \brief Filament Turner main functionality
 *
 * <b> History </b>
 * 	- 2024-03-14 - File creation
 *****************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include "../Inc/FilamentTurner.h"
#include "../mcc_generated_files/tmr1.h"


/******************************************************************************
 * Defines
 *****************************************************************************/

/******************************************************************************
 * Global variable definition
 *****************************************************************************/
static const uint16_t ui16FullStepTable[NUMBER_OF_FULLSTEPS_PER_REVOLUTION] = FULLSTEP_COMMUTATION_TABLE;
static const uint16_t ui16HalfStepTable[NUMBER_OF_HALFSTEPS_PER_REVOLUTION] = HALFSTEP_COMMUTATION_TABLE;

/******************************************************************************
 * Function definitions
 *****************************************************************************/
void FTInit (tsFILAMENTTURNER* psFT)
{
    uint16_t Test = 0;
    // Reset the outputs
    PORTB &= 0x0FFF;

    // Initialize the timer register
    TMR1    = 0;
    PR1     = TIMER_PERIOD;

    psFT->sCommutationPar.eDirection = DIRECTION;

    #if USEHALFSTEPPING == 0
    psFT->sCommutationPar.pui16CommutationTable = &ui16FullStepTable;
    psFT->sCommutationPar.ui8NumOfTableEntries = NUMBER_OF_FULLSTEPS_PER_REVOLUTION;
    #else
    psFT->sCommutationPar.pui16CommutationTable = &ui16HalfStepTable;
    psFT->sCommutationPar.ui8NumOfTableEntries = NUMBER_OF_HALFSTEPS_PER_REVOLUTION;
    #endif
    
    // Switch on timer 1
    T1CONbits.TON = 1;
}
//=============================================================================

void StepIncrease (tsFILAMENTTURNER* psFT)
{
    uint16_t ui16NewValue;
    
    if (psFT->sCommutationPar.eDirection == eDIR_CW)
    {
        if (!(++psFT->i8ActStep < psFT->sCommutationPar.ui8NumOfTableEntries))
            psFT->i8ActStep = 0;
    }
    else
    {
        if (--psFT->i8ActStep < 0)
            psFT->i8ActStep = psFT->sCommutationPar.ui8NumOfTableEntries - 1;
    }
    

    ui16NewValue = psFT->sCommutationPar.pui16CommutationTable[psFT->i8ActStep];
    // Apply the outputs all at once
    PORTB = ((PORTB & 0x0FFF) | ui16NewValue);
}

//=============================================================================

//=============================================================================

//=============================================================================

//=============================================================================
