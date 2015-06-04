#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes
#include <xc.h>

int main ( void )
{
    /* Initialize all MPLAB Harmony modules, including application(s). */
    SYS_Initialize ( NULL );
    
    TRISBbits.TRISB7 = 0; // sets pin B7 dig out used for A
    RPB5Rbits.RPB5R = 0b0101; // used as OC2
    ANSELBbits.ANSB15 = 0;  // analog off for B15
    RPB15Rbits.RPB15R = 0b0101;   // Sets B15 to be used for OC1
    ANSELBbits.ANSB14 = 0;  // analog off for B14
    TRISBbits.TRISB14 = 0;  // digital out

    PR2 = 39999;            // Timer2 is the base for OC1, PR3 defines PWM frequency, 1 kHz
    TMR2 = 0;               // initialize value of Timer2
    T2CONbits.ON = 1;       // turn Timer2 on, all defaults are fine
    OC1CONbits.OCTSEL = 0;  // use Timer2 for OC1
    OC2CONbits.OCTSEL = 0;
    OC1CONbits.OCM = 0b110; // PWM mode with fault pin disabled
    OC2CONbits.OCM = 0b110;
    OC1RS = 00000;          // duty cycle = OC1RS/(PR3+1) = 0%
    OC1R = 00000;           // initialize before turning OC1 on; afterward it is read-only
    OC2RS = 00000;          // duty cycle = OC1RS/(PR3+1) = 0%
    OC2R = 00000;
    OC1CONbits.ON = 1;      // Turn OC1 on
    OC2CONbits.ON = 1;

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

