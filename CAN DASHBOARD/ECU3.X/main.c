#include "main.h"
#include "clcd.h"
#include "can.h"
#include<stdio.h>

const char gears[][5]={"N","1","2","3","4","5","R"}; //gear values
const char indicators[][5]={"<-","->","<-->","OFF"}; //indicator values
unsigned int blink=1; // toggle indicator
unsigned int speed=0;
unsigned int rpm=0;
unsigned int wait=0; //blink counter

/* delay 1ms function */
void delay(unsigned short factor)
{
	unsigned short i, j;

	for (i = 0; i < factor; i++)
	{
		for (j = 500; j--; );
	}
}


void init_config(void)
{
    
    init_clcd(); //initialise clcd
	
	init_can(); //initialise can
    
    TRISB=TRISB&0x0C; // set RB0,RB1,RB6,RB7 as ops
    
    delay(200); 
    clcd_print("SPD",LINE1(0));
    clcd_print("GR",LINE1(4));
    clcd_print("RPM",LINE1(7));
    clcd_print("IND",LINE1(12));  
    
}

void can_demo(void)
{
         
    if (can_receive()) //if mssg received
	{		
        unsigned int rx_id;
        rx_id=((can_payload[SIDH]<<3) |  //11 bit std id formed
                (can_payload[SIDL]>>5)); 
        
        if(rx_id == 0x120) //speed + gear
        {
            char res[4];
            speed =can_payload[D0]; //extract speed
            sprintf(res,"%3d",speed); //display speed
            clcd_print(res,LINE2(0)); //print speed position
            int idx=can_payload[D1]; //load gear to idx
            clcd_print(gears[idx],LINE2(5)); //print gear   
        }
        else if(rx_id == 0x124) //rpm + indicator
        {
            char temp[5];
            static unsigned int prev_rpm=0;
            rpm = ((unsigned int)can_payload[D0] << 8) | can_payload[D1]; //extract rpm to two 8 bit, 0X0B << 8 | 0XB8
            sprintf(temp,"%4d",rpm); //load to temp and print
            rpm=0;
            clcd_print(temp,LINE2(7)); //print rpm value
            

            
            if(wait++==50) //blink logic for every 50 sec
            {
                wait=0;
                blink=!blink;
            }
            
            if(blink) //led on
            {
                switch(can_payload[D2]) //indicator key
                {
                    case 0x0E : //switch1 left indicator
                                clcd_print("    ",LINE2(12));
                                clcd_print("<-",LINE2(12));
                                RB0=RB1=1; //left LEDs on
                                break;
                                
                    case 0x0D : //switch2 right indicator
                                clcd_print("    ",LINE2(12));
                                clcd_print("->",LINE2(12));
                                RB6=RB7=1; //right LEDs on
                                break;  
                                
                    case 0x0B : //switch3 hazard 
                                clcd_print("    ",LINE2(12));
                                clcd_print("<-->",LINE2(12));
                                RB0=RB1=RB6=RB7=1; //left and right LEDs on
                                break;             
                }
            }
            else
            {
                
                PORTB=PORTB&0x0C; //clears RB0,RB1,RB6,RB7
                if(can_payload[D2]==0x07) //indicator off
                {
                    
                    clcd_print("OFF ",LINE2(12));
                }
                else
                {
                    clcd_print("      ",LINE2(11)); //clears display
                }
                
            } 

        }  
	}
    
    delay(10);
}

void main(void)
{    
	init_config();

	while (1)
	{
		can_demo();	
	}
}