

#include <xc.h>
#include "main.h"
#include "adc.h"
#include "keypad.h"
#include "can.h"

unsigned int rpm=0; //global variable for rpm 16 bit value need 2 CAN bytes
unsigned char key=0x07; //indicator in off condn

void init_config()
{
    init_adc(); //initialise adc
    init_can(); //initialise can
    init_digital_keypad(); //initialise switch
    
}
void main(void) 
{
    init_config();
    
    unsigned int prev=0;
    
    while(1)
    {
        unsigned char k=read_digital_keypad(STATE_CHANGE); //switch pressed
        if((k!=0xFF) && (k!=key)) //update only when new key pressed or else default no key pressed
        {
            key=k;
        }        
        unsigned int adc_val=read_adc(4); //read adc channel 4
        rpm=(adc_val/10.23)*60; //0-6000 range
        
        if(prev != rpm) //transmit only if changed
        {
            prev=rpm;   //prev value changed to current rpm
        }
        can_transmit(); //transmit fn called , ID = 0X124
    } 
}
