
#include <avr/io.h>
#include <avr/interrupt.h>


//Automatic doors. Opens/closes with a PWM 100Hz, Open (+ width) 3 ms, Close (-width) 4 ms. Configured in Timer1, FatPWM TOP ICR1
//Every 8 ms (timer 0 ctc) a 8 bit value is sent to the "infrared device" connected at PC. This is deactivated when infrarred is not needed
//Waiting 3 seconds when doors are opened. Timer 1 in CTC is used. The count is restared when a new person arrives
//Open/closed sensors are in PB4 and PB3 (pinchange). Person detection is at PD2 (INT0)

//CAUTION:: TIFR1 does not work on certain simulide versions! This example only works on "SimulIDE_0.4.15-SR9_Win64"
#define OPEN 14000
#define CLOSED 8000
#define STOP 21000
#define WAIT_3s 46875 
#define WAIT_8ms 125

volatile unsigned char  direction;
volatile unsigned char  state;


void InitTimer0()
{
	//TIMER CTC for 8 ms. 
	TCCR0A = (1<<WGM01) + (1<<COM0A0); //CTC Not needed COM0A0, just toggle to check the timer frequency
	TCCR0B=  (1<<CS00) + (1<<CS02);   //div 1024
	OCR0A= WAIT_8ms; 
	TIMSK0 = (1<<OCIE0A); //enable OC interrupt
	
	
	
}
void StopInfrarred()
{
	TCCR0B=  0x00;   //no clock source
}
void StartInfrarred()
{
	InitTimer0();
	//TCCR0B=  (1<<CS00) + (1<<CS02);   //div 1024
}
void Restart3seconds()
{
	TCCR1B &= 0xF8 ; //no clock source: CS10,11,12=0
	TCNT1H=0x00;
	TCNT1L=0x00;
	TCCR1B |= (1 << CS10) | (1 << CS12);  //prescale 1024
}
void SetTimer1toPWM(unsigned int mode)
{
	//Configuro FastPWM top=ICR1
	//inverting mode as explained in class
	//Div=8
	//PWM base: 50Hz; ICR1=20000
	//if stop-> stop the clck. Check with osciloscope using a word higher than the pwm width
	if (mode==STOP) 
	{	//TCCR1A=0x00;
		TCCR1B=0x00;
	}
	else
	{
		OCR1AH=mode>>8;
		OCR1AL=mode & 0x00FF;
		TCCR1A = (1 << WGM11) | (1 << COM1A1)  | (1 << COM1A0) | (1<<COM1A1);
		TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11) ;
	}
	TIFR1=0x00; //clear any previous interrupt flag
	TIMSK1= 0x00;
}

void SetTimer1toCTC()
{
	//OCR1AH=WAIT_3s>>8;
	//OCR1AL=WAIT_3s & 0x00FF;
	OCR1A=WAIT_3s;
	//CTC Prescale 1024
	
	TCCR1A = 0x00;
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12);
	TIFR1=0x00; //clear any previous interrupt flag
	TIMSK1= 1<<OCIE1A;
	
	
}
void Config()
{
	state=0;
	ICR1H=20000>>8; 
	ICR1L=20000 & 0x00FF; 
	
	SetTimer1toPWM(STOP);
	InitTimer0();
	
	
	//PORTC as output to control the infrarred device
	DDRC=0xFF;
	
	//bit B5 output (1) to switch on/off a led (pin13)
	//Output compare timer1 is in PB1, so it must be output
	DDRB|=(1<<PORTB1)+(1<<PORTB5);
	
	//PORTD as input for PD2 (INT0) and detect pinchange in PD3,PD4,
	//PD6 as output for OC0A
	DDRD=(1<<PORTD6);

	//Enable INT0
	EIMSK=0x01;
	EICRA=0x02;
	
	//Enable Pinchange PD
	PCICR=(1<<PCIE0);
	PCMSK0=(1<<PCINT3)+(1<<PCINT4); //Enable PB3 and PB4
	
	
		
}
////////ISR timer0, counting 8 ms
ISR(TIMER0_COMPA_vect)
{
	if (state==0 || state==3)
	{
		PORTC=direction;
		direction++;	
	}

}
ISR(TIMER1_COMPA_vect)
{

	if (state==2)
	{
		//Start closing doors since 3 seconds have passed already
		
		SetTimer1toPWM(CLOSED);

		state=3;
		
	}

}
//////ISR person detection
ISR(INT0_vect)
{
	if (state==0 || state==3) //it is closed or closing
	{
		//open door
		SetTimer1toPWM(OPEN);
		//disable infrared
		StopInfrarred();
		state=1;
	}
	else if (state==2) //waiting for 3 seconds
	{
		Restart3seconds();
		
	}
}

ISR(PCINT0_vect)
{
	if (PINB & (1<< PINB3))
	{
		//Interrupt for Door' Open Button A
		if (state==1)
		{
			SetTimer1toPWM(STOP);
			StartInfrarred();
			SetTimer1toCTC(); //wait for 3 seconds
			
			state=2;
		}
	}
	else if (PINB & (1<<PINB4))
	{
		//Interrupt for Door' Closed Button B

		if (state==3)
		{
			SetTimer1toPWM(STOP);
			state=0;
		}

	}
	
	
}

void main(void)
{
	cli();
    Config();
	sei();
	
    while (1) {};
    
}

