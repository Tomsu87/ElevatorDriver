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

#define cycles 250  // iloœæ cykli w delay

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
		uint8_t rLocks:1;
		uint8_t rUp:1;
		uint8_t rDn:1;
		uint8_t rAllow:1;
		uint8_t lightUp:1;
		uint8_t lightDn:1;
		uint8_t lightAlert:1;
		uint8_t buzzAlert:1;
			};
	};
	
volatile outputs GlobalOutputs;

union inputs {
	uint8_t byte;
	struct {
		uint8_t pOBW:1;
		uint8_t UpBtn:1;
		uint8_t DnBtn:1;
		uint8_t AlBtn:1;
		uint8_t PosUp:1;
		uint8_t PosDn:1;
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
	if input1 GlobalInputs.pOBW = 1;
	if input2 GlobalInputs.UpBtn = 1;
	if input3 GlobalInputs.DnBtn = 1;
	if input4 GlobalInputs.AlBtn = 1;
	if input5 GlobalInputs.PosUp = 1;
	if input6 GlobalInputs.PosDn = 1;
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
	if ((GlobalInputs.pOBW)&&(GlobalInputs.PosUp)) {
		GlobalOutputs.rLocks=1;
		UpdateOutputs();
		delayms(100);
		GlobalOutputs.lightUp=1;
		GlobalOutputs.rUp=1;
		GlobalOutputs.rAllow=1;
		UpdateOutputs();
		while (GlobalInputs.PosUp) 
		{
			UpdateInputs();
			if (!GlobalInputs.pOBW)
			{
				GlobalOutputs.lightAlert=1;
				GlobalOutputs.buzzAlert=1;
				GlobalOutputs.rAllow=0;
			} else {
			GlobalOutputs.rAllow=1;
			GlobalOutputs.buzzAlert=0;
			GlobalOutputs.lightAlert=0;
		}
		UpdateOutputs();
		}
		GlobalOutputs.rAllow=0;
		GlobalOutputs.rUp=0;
		GlobalOutputs.lightAlert=1;
		GlobalOutputs.buzzAlert=1;
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
	if ((GlobalInputs.pOBW)&&(GlobalInputs.PosDn)) {
		GlobalOutputs.rLocks=1;
		UpdateOutputs();
		delayms(100);
		GlobalOutputs.lightDn=1;
		GlobalOutputs.rDn=1;
		GlobalOutputs.rAllow=1;
		UpdateOutputs();
		while (GlobalInputs.PosDn) 
		{
			UpdateInputs();
			if (!GlobalInputs.pOBW) 
			{
			GlobalOutputs.lightAlert=1;
			GlobalOutputs.buzzAlert=1;
			GlobalOutputs.rAllow=0;
			} else {
				 GlobalOutputs.rAllow=1;
				 GlobalOutputs.buzzAlert=0;
				 GlobalOutputs.lightAlert=0;
				}
			UpdateOutputs();
		}
		GlobalOutputs.rAllow=0;
		GlobalOutputs.rDn=0;
		GlobalOutputs.lightAlert=1;
		GlobalOutputs.buzzAlert=1;
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
		
/*				Wy³¹czone do czasu poprawek uk³adu.
		if (GlobalInputs.in4)    // przycisk alarm - tr¹bienie na piêtrach.
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
		
		if ((GlobalInputs.UpBtn)&&(GlobalInputs.PosUp))  // w góre - winda na górze.
		{ 
			GlobalOutputs.buzzAlert=1;
			UpdateOutputs();
			delayms(10);
			GlobalOutputs.byte=0;
			UpdateOutputs();
			delayms(100);
			
		}
		
		if ((GlobalInputs.DnBtn)&&(GlobalInputs.PosDn)) // w dó³ - winda na dole
		{  
			GlobalOutputs.buzzAlert=1;
			UpdateOutputs();
			delayms(10);
			GlobalOutputs.byte=0;
			UpdateOutputs();
			delayms(100);
			
		}
		
		if ((GlobalInputs.UpBtn)&&(!GlobalInputs.pOBW)) // w góre - nie ma zezw.
		{
			GlobalOutputs.lightAlert=1;
			GlobalOutputs.lightDn=1;
			UpdateOutputs();
			delayms(200);
			GlobalOutputs.byte=0;
			UpdateOutputs();
			delayms(100);
		}
		
		if ((GlobalInputs.DnBtn)&&(!GlobalInputs.pOBW)) // w dó³ - nie ma zezw.
		{
			GlobalOutputs.lightAlert=1;
			GlobalOutputs.lightUp=1;
			UpdateOutputs();
			delayms(200);
			GlobalOutputs.byte=0;
			UpdateOutputs();
			delayms(100);
		}
		
		uint8_t counter,test;
		
		if ((GlobalInputs.pOBW)&&(GlobalInputs.UpBtn)&&(GlobalInputs.PosDn))
		{
			test=1;
			counter=200;
			while (counter) {
				UpdateInputs();
				if (!GlobalInputs.UpBtn) test=0;
			}
			if (test) GoUp();
			
		}
		
		if ((GlobalInputs.pOBW)&&(GlobalInputs.DnBtn)&&(GlobalInputs.PosUp))
		{
			test=1;
			counter=200;
			while (counter) {
				UpdateInputs();
				if (!GlobalInputs.UpBtn) test=0;
			}
			if (test) GoDn();
			
		}
		
		if ((GlobalInputs.pOBW)&&(GlobalInputs.UpBtn)&&(GlobalInputs.DnBtn)&&(GlobalInputs.AlBtn)) {
		GlobalOutputs.rLocks=1;
		UpdateOutputs();
		delayms(100);
		GlobalOutputs.lightUp=1;
		GlobalOutputs.rUp=1;
		GlobalOutputs.rAllow=1;
		UpdateOutputs();
		while (GlobalInputs.PosUp)
		{
			UpdateInputs();
			if (!GlobalInputs.pOBW)
			{
				GlobalOutputs.lightAlert=1;
				GlobalOutputs.buzzAlert=1;
				GlobalOutputs.rAllow=0;
				UpdateOutputs();
			} else GlobalOutputs.rAllow=1;
		}
		GlobalOutputs.rAllow=0;
		GlobalOutputs.rUp=0;
		GlobalOutputs.buzzAlert=1;
		UpdateOutputs();
		delayms(500);
		GlobalOutputs.byte=0;
		UpdateOutputs();
		delayms(100);
		}
	}	
		
}

