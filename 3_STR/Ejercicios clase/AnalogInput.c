/*
	Simple example to read from analog input
	Exercise from slides

*/
#include <avr/io.h>


void InitADC()
{
	ADMUX = (1 << REFS0) + (1<<ADLAR);	// Select external voltage reference + Left Adjust
	ADCSRA = (1 << ADEN) + (1<<ADPS0)+(1<<ADPS1)+(1<<ADPS2);	// Enable ADC + CLK Divider 128;
}

unsigned char ReadADC(unsigned char ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX |= (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	
	// wait until ADC conversion is complete
	// It could be better to use interruptions for this
	// Turn on/off led B5 to meassure conversion time. 114us for CLK divisor 128
	//digitalWrite(1);
	PORTB|=(1<<PORTB5);
	
	while( ADCSRA & (1<<ADSC) );
	//digitalWrite(0);
	PORTB&=~(1<<PORTB5);
	return ADCH;
}


void Config()
{
	InitADC();
		//caution PC5 is used for the ADC... 	//so it must be configured as input (0)	DDRC=0x00;		//bit B5 output (1) to switch on/off a led (pin13)	DDRB=0x20; //you can also use 0b00100000	}


int main(void)
{
    Config();
	unsigned char b;
    while (1) 
    {
		b= ReadADC(5);
		// do something with the read value!
		
	}
}

