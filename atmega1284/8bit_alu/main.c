// 8 bit alu
//-----------------------

// libraries
#include <avr/io.h>
#include "timer.h"

/* 
  IN
    f = opcode  - PORTD
    a = input 1 - PORTC
    b = input 2 - PORTA
  OUT
    return = f(a) || f(a,b)
*/
unsigned char alu(unsigned char f, unsigned char a, unsigned char b);

enum Ops
{
	F,T,A,B,NOTA,NOTB,
	OR,AND,XOR,NOR,NAND,XNOR,
	INCA,DECA,NEGA,INCB,DECB,NEGB,
	ADD,SUB,MUL,DIV,MOD
};

int main(void)
{
	// setup ports
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs
	DDRD = 0x00; PORTC = 0xFF; // Configure port D's 8 pins as inputs

	// setup timer
	TimerOn();
	TimerSet(100); // 100 ms
	
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	unsigned char tmpD = 0x00;
	
	while(1)
	{
		tmpA = ~PINA;
		tmpC = ~PINC;
		tmpD = ~PIND;		
		
		tmpB = alu(tmpD,tmpC,tmpA);
		PORTB = tmpB;
		
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}

unsigned char alu(unsigned char f, unsigned char a, unsigned char b)
{
	unsigned char ans = 0x00;
	
	switch(f) // transitions
	{
		case F:
		ans = 0x00;
		break;
		
		case T:
		ans = 0xff;
		break;

		case A:
		ans = a;
		break;

		case B:
		ans = b;
		break;

		case NOTA:
		ans = ~a;
		break;
		
		case NOTB:
		ans = ~b;
		break;

		case OR:
		ans = a|b;
		break;

		case AND:
		ans = a&b;
		break;
		
		case XOR:
		ans = a^b;
		break;

		case NOR:
		ans = ~(a|b);
		break;
		
		case NAND:
		ans = ~(a&b);
		break;

		case XNOR:
		ans = ~(a^b);
		break;

		case INCA:
		ans = a+1;
		break;

		case DECA:
		ans = a-1;
		break;

		case NEGA:
		ans = -1*a;
		break;

		case INCB:
		ans = b+1;
		break;

		case DECB:
		ans = b-1;
		break;

		case NEGB:
		ans = -1*b;
		break;

		case ADD:
		ans = a+b;
		break;

		case SUB:
		ans = a-b;
		break;

		case MUL:
		ans = a*b;
		break;

		case DIV:
		ans = a/b;
		break;

		case MOD:
		ans = a%b;
		break;
		
		default:
		ans = 0x00;
		break;
	}
	
	return ans;
}
