/*
 Input/Output with protocol lines
 Exercise from slides
 */ 


#include <avr/io.h>

#define max 128

int main(void)
{

	unsigned char v,d;
	DDRB=0x00;
	DDRC=(1<<PINC1) | (1<<PINC2) | (1<< PINC3);
	PORTC=(1<<PINC2) | (1<< PINC3);
    while (1) 
    {
		d=PINC & 0x01;
		if (d)
		{
			v=PINB;
			PORTC|=(1<<PINC1);
			PORTC&=~(1<<PINC1);
			if (v>max) 
				//hard stop, no way to restart the motor
				{
					PORTC&=~ (1<<PINC2 | 1<<PINC3); 
					break;
				}
			   
		}
	}
				
}

