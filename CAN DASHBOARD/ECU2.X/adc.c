
#include <xc.h>
#include "adc.h"

#define _XTAL_FREQ 20000000
void init_adc()
{
    ADFM=1; //adc result format bit 1=right
    
    ACQT0=0; //acquisition time 010 = 4TAD
    ACQT1=1;
    ACQT2=0;
    
    ADCS0=0; //adc conversion clock 010 = fosc/32
    ADCS1=1;
    ADCS2=0;
    
    ADRESH=0;
    ADRESL=0;
    
    ADON=1; //turn on adc
}

unsigned int read_adc(unsigned char channel) //reads channel
{
    ADCON0&=0xC3; //previous channel cleared
    ADCON0 |= (channel<<2); //select channel
    __delay_us(20); //acquisition delay
    GO=1; //start conversion 
    while(GO); //completes when it is 0
    return ((unsigned int)ADRESH<<8) | ADRESL; //right justified
}