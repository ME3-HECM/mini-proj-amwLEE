#include <xc.h>
#include "interrupts.h"
#include "dateandtime.h"

/*****************************************************************************************
 * Function to turn on interrupts and set if priority is used
 * Note you also need to enable peripheral interrupts in the INTCON register to use CM1IE
*****************************************************************************************/
void Interrupts_init(dateandtime current) {
	// Turn on global interrupts, peripheral interrupts and the interrupt source
    // It's a good idea to turn on global interrupts last, once all other interrupt configuration is done
    if (current.hour<1 || current.hour>=5) {PIE2bits.C1IE = 1;}      // Enable comparator interrupt source
    else {PIE2bits.C1IE = 0;}
    PIE0bits.TMR0IE = 1;    // Enable TMR0 interrupt source
    IPR2bits.C1IP = 1;
    IPR0bits.TMR0IP = 0;
    INTCONbits.IPEN = 1;
    INTCONbits.PEIE = 1;    // Turn on peripheral interrupts
    INTCONbits.GIE = 1;     // Turn on interrupts globally (when this is off, all interrupts are deactivated)
}


/*****************************************************************
 * High priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
*****************************************************************/
volatile unsigned char sunrise_flag;
volatile unsigned char sunset_flag;

void __interrupt(high_priority) HighISR() {
	// Add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if (PIR2bits.C1IF) {                        // Check the interrupt source
        LATDbits.LATD7 = !LATDbits.LATD7;       // Toggle the LED on RD7 when the LDR goes from light to dark
        if (LATDbits.LATD7==0) {sunrise_flag=1;}
        else {sunset_flag=1;}
        PIR2bits.C1IF = 0;                      // Clear the interrupt flag
    }
}


/*****************************************************************
 * Low priority interrupt service routine
 * Make sure all enabled interrupts are checked and flags cleared
*****************************************************************/
volatile unsigned char time_flag;

void __interrupt(low_priority) LowISR() {
	// Add your ISR code here i.e. check the flag, do something (i.e. toggle an LED), clear the flag...
    if (PIR0bits.TMR0IF) {                  // Check the interrupt source
        TMR0H = TMR0H_bits;                 // Write to TMR0H first (start the timer at 3035 to improve accuracy, see timers.c for explanation)
        TMR0L = TMR0L_bits;                 // Then write to TMR0L (start the timer at 3035 to improve accuracy, see timers.c for explanation)
        
        time_flag = 1;
        PIR0bits.TMR0IF = 0;                // Clear the interrupt flag
    }
}