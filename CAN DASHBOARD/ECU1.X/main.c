#include "main.h"
#include "keypad.h"
#include "can.h"
#include "adc.h"
#include<stdio.h>

unsigned int speed=0; //global decalaration of speed
unsigned char gear=0; //global decalartion of gear


void init_config(void)
{
	
    init_digital_keypad(); //to access switches
    
    init_can(); //access can, sets baud rate, enables transmit buffer
    
    init_adc(); //configures adc, selects ref v, enables adc conversion
}

void gear_change() //fn to inc and dec gear
{ 
    unsigned char key = read_digital_keypad(STATE_CHANGE); //detects when key pressed
    if(key==SWITCH1)
    {
        if(gear<6) 
            gear++; //increases gear 
    }
    else if(key==SWITCH2)
    {
        if(gear>0)
            gear--; //decreases gear
    } 
}

void can_demo(void)
{
    
    speed = read_adc(4); //reads channel 4,1 - 1023
    
    speed=speed/10.23; //range 0 - 100
    
    gear_change(); //fn called
    
	can_transmit(); //speed D0 and gear D1, also sends CAN frame id 0x120
    
    /*int delay=0;
    if(delay++==200)
    {
        delay=0;
    }*/
    
}

void main(void)
{    
	init_config();

	while (1)
	{
		can_demo();	
	}
}