/**************************************************************************//**
 * \file FilamentTurner.h
 * \author Roman Holderried
 *
 * \brief Declaration and Type definitions for the filament turner.
 *
 * <b> History </b>
 * 	- 2024-03-14 - File creation 
 *****************************************************************************/

#ifndef _FILAMENTTURNER_H_
#define _FILAMENTTURNER_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/******************************************************************************
 * Defines
 *****************************************************************************/
// Parameters
#define USE_HALF_STEPPING           0   // 0: Apply full stepping table, 1: Apply half stepping table
#define ROUNDS_PER_MINUTE           4
#define DIRECTION                   0   // 0: CW, 1: CCW (direction at motor output)

// Step to Output Round 
#define MOTOR_NUMBER_OF_STEPS       64
#define GEAR_RATIO                  96  // Motor internal gear: 1:64, External gear 1:1,5
#define OUTPUT_STEP_COUNT           (uint16_t)MOTOR_NUMBER_OF_STEPS * (1 + USE_HALF_STEPPING) * GEAR_RATIO

#define TIMER_RESOLUTION_us         4
#define TIMER_PERIOD                (uint16_t)(((uint32_t)60 * 1000000) / ((uint32_t)TIMER_RESOLUTION_us * OUTPUT_STEP_COUNT * ROUNDS_PER_MINUTE))

#define NUM_OF_COMM_TABLE_ENTRIES   4 * (1 + USE_HALF_STEPPING)


// Commutation Tables
// Values that can directly be written to the output port register
#define HALFSTEP_COMMUTATION_TABLE \
{ \
    0b0100000000000000,\
    0b1100000000000000,\
    0b1000000000000000,\
    0b1001000000000000,\
    0b0001000000000000,\
    0b0011000000000000,\
    0b0010000000000000,\
    0b0110000000000000,\
}

#define FULLSTEP_COMMUTATION_TABLE \
{ \
    0b1100000000000000,\
    0b1001000000000000,\
    0b0011000000000000,\
    0b0110000000000000,\
}

#define NUMBER_OF_HALFSTEPS_PER_REVOLUTION 8
#define NUMBER_OF_FULLSTEPS_PER_REVOLUTION 4


/******************************************************************************
 * Type definitions
 *****************************************************************************/
typedef enum
{
    eDIR_CW     = 0,
    eDIR_CCW    = 1
}teDIRECTION;

/** \brief Main data struct for the filament turner
 * 
*/
typedef struct
{
    struct
    {
        teDIRECTION eDirection;
        uint8_t     ui8NumOfTableEntries;
        uint16_t*   pui16CommutationTable;
    }sCommutationPar;

    int16_t i8ActStep;
}tsFILAMENTTURNER;

#define tsFILAMENTTURNER_DEFAULTS { {eDIR_CW, 0, NULL},\
                                    -1}

/******************************************************************************
 * Function declarations
 *****************************************************************************/

/** \brief Initializes the Flament turner
 * 
*/
void FTInit (tsFILAMENTTURNER* psFT);

/** \brief Callback to set outputs
 * 
 * 
*/
void StepIncrease (tsFILAMENTTURNER* psFT);


#endif // _FILAMENTTURNER_H_