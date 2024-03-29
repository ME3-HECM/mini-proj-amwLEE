#include <xc.h>
#include "comparator.h"

/*********************************************************
 * DAC_init
 * Function to set up DAC for use by the comparator module
 *********************************************************/
void DAC_init(void) {
    DAC1CON0bits.PSS=0b00;      // 3.3V for positive source (Vdd)
    DAC1CON0bits.NSS=0b0;       // 0V for negative source (Vss)
    
	// 5-bit DAC value to set output voltage
    // Min is 0 = 0V
    // Max is 2^5-1 = 31 = 3.3V
    DAC1CON1bits.DAC1R=0b11011; // Adjust according to ambient light 27~2.87V
    DAC1CON0bits.DAC1EN=1;      // Turn on DAC
}

/************************************************************************
 * Comp1_init
 * Function to set up comparator to compare RF7 to the DAC output voltage
 * Note the CM1 interrupt is not enabled by this function
 ************************************************************************/
void Comp1_init(void) {
    TRISFbits.TRISF7=1;         // Set pin RF7 as input
    CM1NCHbits.NCH=0b011;       // Pin RF7 as input for comparator
    CM1PCHbits.PCH=0b101;       // Use DAC output for positive input
    CM1CON0bits.HYS=1;          // A little bit of hysteresis to stop multiple triggers
    CM1CON0bits.POL=1;          // Needed for interrupt to work
    CM1CON1bits.INTP=1;         // Set interrupt flag on positive going edge (switch on street light when transition from light-to-dark - at sunset)
    CM1CON1bits.INTN=1;         // Set interrupt flag on negative going edge (switch off street light when transition from dark-to-light - at sunrise)
    DAC_init();                 // Initialise the DAC
    CM1CON0bits.EN=1;           // Enable comparator 1
}