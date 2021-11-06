#include <xc.h>
#include "interrupts.h"

/*****************************************************************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE
*****************************************************************************************/
void Interrupts_init(void) {
	// Turn on global interrupts, peripheral interrupts and the interrupt source
    // It's a good idea to turn on global interrupts last, once all other interrupt configuration is done
    PIE0bits.TMR0IE = 1;    // Enable TMR0 interrupt source
    INTCONbits.PEIE = 1;    // Turn on peripheral interrupts
    INTCONbits.GIE = 1;     // Turn on interrupts globally (when this is off, all interrupts are deactivated)
}

/*****************************************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
*****************************************************************/
void __interrupt(high_priority) HighISR() {
	// Add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if (PIR0bits.TMR0IF) {                  // Check the interrupt source
        LATHbits.LATH3 = !LATHbits.LATH3;   // Toggle the LED on RH3 when the LDR goes from light to dark
        TMR0H = 0b1011;                     // Write to TMR0H first (start the timer at 3035 to improve accuracy, see timers.c for explanation)
        TMR0L = 0b11011011;                 // Then write to TMR0L (start the timer at 3035 to improve accuracy, see timers.c for explanation)
        PIR0bits.TMR0IF = 0;                // Clear the interrupt flag
    }
}