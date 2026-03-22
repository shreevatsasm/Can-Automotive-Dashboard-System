
#include <xc.h>
#include "adc.h"

#define _XTAL_FREQ 20000000
void init_adc(void) //configures adc
{
    
    ADFM=1; //adc result format bit, 1=right justified
    
    ACQT0=0; //acquisition time 010 = 4TAD
    ACQT1=1;
    ACQT2=0;
    
    ADCS0=0; //adc conversion clock, 010 = fosc/32
    ADCS1=1;
    ADCS2=0;
    
    ADRESH=0; //clear high
    ADRESL=0; //clear low
    
    ADON=1; //turn on adc module
}

unsigned int read_adc(unsigned char channel) //to read channel
{
    ADCON0 &= 0xC3; //to clear previous channel
    ADCON0 |= (channel<<2); //select channel
    __delay_us(20); //acquisition delay - to charge capacitor after changing channel
    GO=1; //start conversion
    while(GO); //it becomes 0 when conversion completes
    return ((unsigned int)ADRESH<<8) | ADRESL; //combine right justified result
}
