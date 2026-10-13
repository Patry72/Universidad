
#include <avr/io.h>
#include <avr/interrupt.h>

//ADC polling temporizado. Start conversion every 250 ms
//using timer 1 in CTC
volatile unsigned char  b;

void InitADC()
{
	ADMUX = (1 << REFS0) + (1<<ADLAR);	// Select external voltage reference + Left Adjust
	ADCSRA = (1 << ADEN) + (1<<ADPS0)+(1<<ADPS1)+(1<<ADPS2);	// Enable ADC + CLK Divider 128;
}

void InitTimer1()
{
	//Configuro timer1 en CTC
	TCCR1A = 0x00;
	TCCR1B= (1<<WGM12)+(1<<CS10) + (1<<CS11);   //CTC+div 64.
	
	OCR1A=62500;  //250 ms for div64
	TIMSK1=(1<<OCIE1A);//mask for timer
}

void InitTimer0()
{ //FastPWM f_PWM=980.32Hz --> frec_div=64. Non_inverted mode
  //TOP=0xFF, OCR0A for comparing
  //negative pulse 0.1ms --> 0V
  //negative pulse 1 ms  --> 5V
  //width pulse according to the analog value read
  //negative_pulse=(0.9*input/5)+0.1
  
	TCCR0A = (1<<WGM01)+(1<<WGM00)+(1<<COM0A1); //FastPWM, non_inverted
	TCCR0B=  (1<<CS00) + (1<<CS01);   //div 64.
	
	
	
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
	InitTimer1();
	InitTimer0();
	
	//inicializamos DDRC	//caution PC5 is used for the ADC... 	//so it must be configured as input (0)	DDRC=0x00;		//bit B5 output (1) to switch on/off a led (pin13)	DDRB=0x20; //you can also use 0b00100000		//Output compare is in PD6, so it must be output	DDRD|=(1<<PORTD6);		}
ISR(TIMER1_COMPA_vect)
{
	unsigned input;
	b= ReadADC(5);
	
	//width pulse according to the analog value read
	//voltage=b*5/255;
	//negative_pulse=(0.9*voltage/5)+0.1 (in ms)
	//OCR0A=negative_pulse *10^-3 *250*10^3 = negative_pulse*250
	input=b*5/255;
	OCR0A=((0.9*input/5)+0.1)*250;
	
	
}

void main(void)
{
	cli();
    Config();
	sei();
	
    while (1) {};
    
}

