#include <xc.h>
#include "ADC.h"

/******************************************************
/ ADC_init
/ Function used to initialise ADC module and set it up
/ to sample on pin RA3.
******************************************************/
void ADC_init(void)
{
    TRISFbits.TRISF7=1;     // Select pin RF7 as input
    ANSELFbits.ANSELF7=1;   // Ensure analogue circuitry is active (it is by default - watch out for this later in the course!)

    // Set up the ADC module - check section 32 of the datasheet for more details
    ADREFbits.ADNREF = 0;   // Use Vss (0V) as negative reference
    ADREFbits.ADPREF = 0b00;// Use Vdd (3.3V) as positive reference
    ADPCH=0b101111;         // Select channel RF7/ANF7 for ADC (Refer pg 631 of datasheet)
    ADCON0bits.ADFM = 0;    // Left-justified result (i.e. no leading 0s)
    ADCON0bits.ADCS = 1;    // Use internal Fast RC (FRC) oscillator as clock source for conversion
    ADCON0bits.ADON = 1;    // Enable ADC
}

unsigned int ADC_getval(void)
{
    unsigned int tmpval;    // Variable to store voltage readings in bits
    
    ADCON0bits.GO = 1;      // Start ADC conversion

    while (ADCON0bits.GO);  // Wait until conversion done (bit is cleared automatically when done)
        
    tmpval = ADRESH;        // Get 8 most significant bits of the ADC result - if we wanted the 
	// full 10bit result we would also need to read the 2 LSBs from ADRESL and combine the result.
	// An 8bit result is sufficient for our needs here

    tmpval = 255 - tmpval;  // Invert readings for LDR so darker is a smaller number and brighter is a bigger number
    
    return tmpval;          // Return this value when the function is called
}