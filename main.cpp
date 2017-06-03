/*
 * ElevatorDriver.cpp
 *
 * Created: 2017-05-11 22:22:02
 * Author : Tomash
 *
 * Hardware Info :
 *	PD0..PD7 output relays (output high = relay on)
 *	PC0..PC5 inputs (input high 5..24V)
 *	PB1..PB2 inputs (input high 5..24V)
 *	NO tOSC !! internal clock only! (1MHz)
 *
 *
 */ 


#include <avr/io.h>

#define cycles 450  // ilo�� cykli w delay

#define  OutputPort PORTD
#define input1 (PINC & (1<<PC0))
#define input2 (PINC & (1<<PC1))
#define input3 (PINC & (1<<PC2))
#define input4 (PINC & (1<<PC3))
#define input5 (PINC & (1<<PC4))
#define input6 (PINC & (1<<PC5))
#define input7 (PINB & (1<<PB2))
#define input8 (PINB & (1<<PB1))


union outputs {
	uint8_t byte;
	struct {
		uint8_t out1:1;
		uint8_t out2:1;
		uint8_t out3:1;
		uint8_t out4:1;
		uint8_t out5:1;
		uint8_t out6:1;
		uint8_t out7:1;
		uint8_t out8:1;
			};
	};
	
volatile outputs GlobalOutputs;

union inputs {
	uint8_t byte;
	struct {
		uint8_t in1:1;
		uint8_t in2:1;
		uint8_t in3:1;
		uint8_t in4:1;
		uint8_t in5:1;
		uint8_t in6:1;
		uint8_t in7:1;
		uint8_t in8:1;
	};
};	

volatile inputs GlobalInputs;

void UpdateOutputs()
{
	OutputPort = GlobalOutputs.byte;
}

void UpdateInputs()
{
	GlobalInputs.byte=0x00;
	if input1 GlobalInputs.in1 = 1;
	if input2 GlobalInputs.in2 = 1;
	if input3 GlobalInputs.in3 = 1;
	if input4 GlobalInputs.in4 = 1;
	if input5 GlobalInputs.in5 = 1;
	if input6 GlobalInputs.in6 = 1;
	if input7 GlobalInputs.in7 = 1;
	if input8 GlobalInputs.in8 = 1;
	
	
	
	
}

void delayms(uint16_t czas)
{
	while (czas)
	{
		
		for(volatile int i=0; i<cycles ; i++) __asm("nop");
		czas--;
	}
}

void GoUp()
{
	UpdateInputs();
	if ((GlobalInputs.in1)&&(GlobalInputs.in5)) {
		GlobalOutputs.out1=1;
		UpdateOutputs();
		delayms(100);
		GlobalOutputs.out5=1;
		GlobalOutputs.out2=1;
		GlobalOutputs.out4=1;
		UpdateOutputs();
		while (GlobalInputs.in5) 
		{
			UpdateInputs();
			if (!GlobalInputs.in1)
			{
				GlobalOutputs.out7=1;
				GlobalOutputs.out8=1;
				GlobalOutputs.out4=0;
				UpdateOutputs();
			} else GlobalOutputs.out4=1;
		}
		GlobalOutputs.out4=0;
		GlobalOutputs.out2=0;
		GlobalOutputs.out8=1;
		UpdateOutputs();
		delayms(500);
		GlobalOutputs.byte=0;
		UpdateOutputs();
		delayms(100);
	}
}

void GoDn()
{
	UpdateInputs();
	if ((GlobalInputs.in1)&&(GlobalInputs.in6)) {
		GlobalOutputs.out1=1;
		UpdateOutputs();
		delayms(100);
		GlobalOutputs.out6=1;
		GlobalOutputs.out3=1;
		GlobalOutputs.out4=1;
		UpdateOutputs();
		while (GlobalInputs.in6) 
		{
			UpdateInputs();
			if (!GlobalInputs.in1) 
			{
			GlobalOutputs.out7=1;
			GlobalOutputs.out8=1;
			GlobalOutputs.out4=0;
			UpdateOutputs();
			} else GlobalOutputs.out4=1;
			
		}
		GlobalOutputs.out4=0;
		GlobalOutputs.out3=0;
		GlobalOutputs.out8=1;
		UpdateOutputs();
		delayms(500);
		GlobalOutputs.byte=0;
		UpdateOutputs();
		delayms(100);
	}
}

int main(void)
{
    /* Replace with your application code */
	DDRD=0xFF;
	DDRB=0x00;
	DDRC=0x00;
	PORTD=0x00;
	PORTB=0x00;
	PORTC=0x00;
	
	GlobalOutputs.byte = 0;
	
	
    while (1) 
    {
		UpdateInputs();
		
/*				Wy��czone do czasu poprawek uk�adu.
		if (GlobalInputs.in4)    // przycisk alarm - tr�bienie na pi�trach.
		{
			GlobalOutputs.out5=1;
			GlobalOutputs.out6=1;
			GlobalOutputs.out7=1;
			GlobalOutputs.out8=1;
			UpdateOutputs();
			while (GlobalInputs.in4) UpdateInputs();
			delayms(100);
			GlobalOutputs.byte=0;
			UpdateOutputs();
			delayms(500);
		}
	
*/	
		
		if ((GlobalInputs.in2)&&(!GlobalInputs.in5))  // w g�re - winda na g�rze.
		{ 
			GlobalOutputs.out8=1;
			UpdateOutputs();
			delayms(10);
			GlobalOutputs.byte=0;
			UpdateOutputs();
			delayms(100);
			
		}
		
		if ((GlobalInputs.in3)&&(!GlobalInputs.in6)) // w d� - winda na dole
		{  
			GlobalOutputs.out8=1;
			UpdateOutputs();
			delayms(10);
			GlobalOutputs.byte=0;
			UpdateOutputs();
			delayms(100);
			
		}
		
		if ((GlobalInputs.in2)&&(!GlobalInputs.in1)) // w g�re - nie ma zezw.
		{
			GlobalOutputs.out7=1;
			GlobalOutputs.out6=1;
			UpdateOutputs();
			delayms(200);
			GlobalOutputs.byte=0;
			UpdateOutputs();
			delayms(100);
		}
		
		if ((GlobalInputs.in3)&&(!GlobalInputs.in1)) // w d� - nie ma zezw.
		{
			GlobalOutputs.out7=1;
			GlobalOutputs.out5=1;
			UpdateOutputs();
			delayms(200);
			GlobalOutputs.byte=0;
			UpdateOutputs();
			delayms(100);
		}
		
		if ((GlobalInputs.in1)&&(GlobalInputs.in2)&&(!GlobalInputs.in6))
		{
			GoUp();
			
		}
		
		if ((GlobalInputs.in1)&&(GlobalInputs.in3)&&(!GlobalInputs.in5))
		{
			GoDn();
			
		}
		
		if ((GlobalInputs.in1)&&(GlobalInputs.in2)&&(GlobalInputs.in3)&&(GlobalInputs.in4)) {
		GlobalOutputs.out1=1;
		UpdateOutputs();
		delayms(100);
		GlobalOutputs.out5=1;
		GlobalOutputs.out2=1;
		GlobalOutputs.out4=1;
		UpdateOutputs();
		while (GlobalInputs.in5)
		{
			UpdateInputs();
			if (!GlobalInputs.in1)
			{
				GlobalOutputs.out7=1;
				GlobalOutputs.out8=1;
				GlobalOutputs.out4=0;
				UpdateOutputs();
			} else GlobalOutputs.out4=1;
		}
		GlobalOutputs.out4=0;
		GlobalOutputs.out2=0;
		GlobalOutputs.out8=1;
		UpdateOutputs();
		delayms(500);
		GlobalOutputs.byte=0;
		UpdateOutputs();
		delayms(100);
		}
	}	
		
}

