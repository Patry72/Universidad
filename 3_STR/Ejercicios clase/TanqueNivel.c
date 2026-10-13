
#include <avr/io.h>
#include <avr/interrupt.h>


//Tank example. The ADC is read every 15 ms.If ADC value is smaller than 128 (2.5V) the valve is closed
//otherwise it is open. A fast PWM is used for generate the valve command
//When an external interrupt is triggered, the ADC is disable and the
//valve is closed. Normal behavior is set when the external interrupt is
//triggered again
#define CERRAR 36000
#define ABRIR 20000

// value read from ADC
volatile unsigned char  b;

// state from mealy diagram
volatile unsigned char state;

//Initialization functions
void InitADC()
{
	// Select external voltage reference + Left Adjust	+ select ADC channel (channel=5) with safety mask
	ADMUX = (1 << REFS0) + (1<<ADLAR);
	ADMUX |= (5 & 0x0F);
	
	// Enable ADC + Interrupt +CLK Divider 128;
	ADCSRA = (1 << ADEN) + (1 << ADIE) +(1<<ADPS0)+(1<<ADPS1)+(1<<ADPS2);
}

void InitTimer1()
{
	//Configure FastPWM top=ICR1
	//inverting mode as explained in class
	//PWM base: 50Hz
	//ICR1=40000;
	ICR1=40000;
	    //The documentation suggests to load 16bits registers in two parts:
        //	ICR1H= 0x9C; (High byte of 40000)
	    //  ICR1L =0x40; (Low byte of 40000)

	OCR1A=CERRAR;  //closed valve
	
	TCCR1A = (1 << WGM11) | (1 << COM1A1)  | (1 << COM1A0);
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11) ;
	//No need to enable interruptions, PWM is caught in the OC1A pin
}

void InitTimer0()
{
	//TIMER CTC for 15 ms.
	TCCR0A = (1<<WGM01); //CTC
	TCCR0B=  (1<<CS00) + (1<<CS02);   //div 1024
	OCR0A= 234;
	TIMSK0 = (1<<OCIE0A); //enable OC interrupt
}

void Config()
{
	InitADC();
	InitTimer1();
	InitTimer0();
	
	//DDRC
	//PC5 is used for the ADC, so it is input (0)
	DDRC&=~(1<<PORTC5);
	
	//DDRB. B5 output (1) to switch on/off a led (pin13)
	//Output compare timer1 is in PB1, so it must be output too
	DDRB|=(1<<PORTB1)+(1<<PORTB5);
	
	//External interrupt PD2 as input
	DDRD&=~(1<<PORTD2);

	//External interrupt masks
	EIMSK=0x01;
	EICRA=0x02;
	
	//state from mealy diagram
	state=0;
}

////////////////////////////////////////////////////////////////////////
// ISRs obtained through the Mealy Diagram
///////////////////////////////////////////////////////////////////////
ISR(ADC_vect){
//When the ADC has finished the conversion, just read the value and commands de valve
	if (state==1)
	{

		if (ADCH<128)
		{
			OCR1A=ABRIR;
		}
		else
		{
			OCR1A=CERRAR;
		}
		state=0;
	}
}


ISR(TIMER0_COMPA_vect)
{
//Every 15ms 
	if (state==0)
	{
		PORTB^=(1<<PORTB5);
		ADCSRA |= (1<<ADSC);
		state=1;
	}
	
}

ISR(INT0_vect)
{
//When the user presses the button
	if (state==0 || state==1)
	{
		OCR1A=CERRAR;
		state=2;
	}
	else if (state==2)
	{
		state=0;
	}
	
}
///////////////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////////////
void main(void)
{
	cli(); //Clear the i-bit; no interruptions will be detected
	Config(); //Configure everything
	sei(); //Enable general interruptions. The system starts to work
	
	while (1) {}; //Waiting for events!!
	
}

