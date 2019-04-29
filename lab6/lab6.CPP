#include <dos.h>
#include <conio.h>
#include <stdio.h>

const unsigned char QUIT_CHAR = 0x01;
const unsigned char HIGHLIGHT_CHAR = 0xa3;

void interrupt (*OldInt9)(...);
void SetMask(unsigned char mask);
void Highlight (void); 

int commandIsExecuted = 0;
int quitFlag = 0;
int needHighlight = 0;
int count = 0;

void interrupt NewInt9(...)
{
	unsigned char value = 0;
	value = inp(0x60);

	if (value == QUIT_CHAR)
		quitFlag = 1;

	if (value == HIGHLIGHT_CHAR)
	{
		if (needHighlight == 0)
			needHighlight = 1;
		else
			needHighlight = 0;
	}

	if (value == 0xFE && needHighlight == 1) {		// Если нет подтверждения успешного выполнения команды
		commandIsExecuted = 1;							
		count++;
		if (count > 3) {
			printf("\n\nError!\n");
			quitFlag = 1;
			outp(0x20, 0x20);							
			return;
		}
	}
	else commandIsExecuted = 0;

	printf("\t%x", value);
	outp(0x20, 0x20);
}

int main()
{		
	_disable();
	OldInt9 = getvect(9);
	setvect(9, NewInt9);
	_enable();
	
	while(!quitFlag)
		if (needHighlight)
			Highlight();

	_disable();
	setvect(9, OldInt9);
	_enable();

	return 0;
}

void SetMask(unsigned char mask)
{
	commandIsExecuted = 1;
	
	while (!commandIsExecuted)
	{		
		while ((inp(0x64) & 0x02) != 0x00);
		outp(0x60, 0xED);		
		outp(0x60, mask);
		printf("\tset mask %d", mask);
		delay(50);		
	}		
}

void Highlight ()
{
	printf("\tBegin highligt");
	SetMask(0x07);
	delay(1000);
	SetMask(0x00);
	delay(1000);
	SetMask(0x07);
	delay(500);
	SetMask(0x00);
	delay(1000);
	printf("\tEnd highlight");	
}