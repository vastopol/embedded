// basic SyncSM
//-----------------------

// libraries
#include <avr/io.h>
#include "timer.h"

// function declarations
void sm_tick();

// global variables
enum SM_State{START,WAIT} state;

int main(void)
{
	// setup ports
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs

	// setup timer
	TimerOn();
	TimerSet(1000); // 1000 ms
	
	// set sm initial state
	state = START;
	
	while(1)
	{
		sm_tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}

void sm_tick()
{
	switch(state) // transitions
	{
		case START:
		state = WAIT;
		break;
		
		case WAIT:
		state = WAIT;
		break;
		
		default:
		state = START;
		break;
	}
	
	switch(state) // actions
	{
		case WAIT:
		break;
		
		default:
		break;
	}
}