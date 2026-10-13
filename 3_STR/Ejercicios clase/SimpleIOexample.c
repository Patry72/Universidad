/*
Simple Input/Output example
Exercise from slides
 */ 

#include <avr/io.h>

int main(void)
{
  	unsigned short volatile b;
  	
  	//PB input for keyboard, PC output for 7 segments
  	DDRB=0x00;  //Input
  	DDRC=0xFF;  //Output
  	//D0,D1,D2 as output for keyboards leds
  	DDRD|= (1<<PIND0) | (1<<PIND1) | (1<<PIND2)	;
  	
  	while(1)
  	{
	  	b=PINB;
	  	PORTC=b;
	  	switch (b)
	  	{
		  	case 58: PORTD ^= 1<<PIND0;break;
		  	case 69: PORTD ^= 1<<PIND1;break;
		  	case 70: PORTD ^= 1<<PIND2;break;
		  	
		  	default: PORTD&= ~ ( 1<<PIND0 | 1<<PIND1 | 1<<PIND2);
		  	//by default switch them off
	  	}
  	}
}

