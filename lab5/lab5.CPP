#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>

int msCounter = 0;

void interrupt(*oldInt70h)(...);		
void interrupt(*oldInt4Ah)(...);		
void interrupt newInt70handler(...);	

int BCDToInteger(int bcd);
unsigned char IntToBCD(int value);
void getTime(void);
void setTime(void);
void delay_time(void);
void wait(void);

void main() {
	char c, value;
	clrscr();
	printf("\n1. Show time\n2. Set time\n3. Delay time\n0. Exit\n\n");
	while (c != 0) {
		c = getch();
		switch (c) {
		case '1': getTime(); break;
		case '2': setTime(); break;
		case '3': delay_time(); break;
		case '0': return;
		}
	}
}

void getTime(void) {
	int value;

	outp(0x70, 0x04);					
	value = inp(0x71);
	printf("%d:", BCDToInteger(value));

	outp(0x70, 0x02);					
	value = inp(0x71);
	printf("%d:", BCDToInteger(value));

	outp(0x70, 0x00);					
	value = inp(0x71);
	printf("%d   ", BCDToInteger(value));

	outp(0x70, 0x07);					
	value = inp(0x71);
	printf("%d.", BCDToInteger(value));

	outp(0x70, 0x08);					
	value = inp(0x71);
	printf("%d.", BCDToInteger(value));

	outp(0x70, 0x09);					
	value = inp(0x71);
	printf("%d   ", BCDToInteger(value));

	outp(0x70, 0x06);					
	value = inp(0x71);
	switch (BCDToInteger(value)) {
	case 2: printf("Monday\n"); break;
	case 3: printf("Tuesday\n"); break;
	case 4: printf("Wednesday\n"); break;
	case 5: printf("Thursday\n"); break;
	case 6: printf("Friday\n"); break;
	case 7: printf("Saturday\n"); break;
	case 1: printf("Sunday\n"); break;
	default: printf("Day of week is not set\n"); break;
	}
}

void setTime(void) {
	do {								
		outp(0x70, 0x0A);
	} while (inp(0x71) & 0x80);

	outp(0x70, 0x0B);
	outp(0x71, inp(0x71) | 0x80);

	int value;
	do {
		printf("Enter hour: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 23 || value < 0);
	outp(0x70, 0x04);
	outp(0x71, IntToBCD(value));		

	do {
		printf("Enter minute: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 59 || value < 0);
	outp(0x70, 0x02);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter second: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 59 || value < 0);
	outp(0x70, 0x00);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter week day number: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 7 || value < 1);
	outp(0x70, 0x06);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter day of month: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 31 || value < 1);
	outp(0x70, 0x07);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter mounth: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 12 || value < 1);
	outp(0x70, 0x08);
	outp(0x71, IntToBCD(value));

	do {
		printf("Enter year: ");
		fflush(stdin);
		scanf("%d", &value);
	} while (value > 99 || value < 0);
	outp(0x70, 0x09);
	outp(0x71, IntToBCD(value));

	outp(0x70, 0x0B);
	outp(0x71, inp(0x71) & 0x7f);
}

void interrupt far newInt70handler(...) {
	msCounter++;						
	outp(0x70, 0x0C);					
	inp(0x71);
	outp(0x20, 0x20);					
	outp(0xA0, 0x20);					
}

void delay_time(void) {
	unsigned long delayPeriod;
	unsigned char value;

	do {
		printf("Enter delay time in milliseconds: ");
		fflush(stdin);
		scanf("%ld", &delayPeriod);
	} while (delayPeriod < 1);
	printf("Time before delay: ");
	getTime();
	printf("Delaying ...\n");

	_disable();							
	oldInt70h = getvect(0x70);
	setvect(0x70, newInt70handler);
	_enable();							

	outp(0xA1, inp(0xA1) & 0xFE);			

	outp(0x70, 0x0B);					
	outp(0x71, inp(0x71) | 0x40);			

	msCounter = 0;
	while (msCounter != delayPeriod);	 
	printf("End delay of %d ms\n", msCounter);

	_disable();
	setvect(0x70, oldInt70h);			
	_enable();
	printf("Time after delay: ");
	getTime();
}

int BCDToInteger(int bcd) {
	return bcd % 16 + bcd / 16 * 10;
}
unsigned char IntToBCD(int value) {
	return (unsigned char)((value / 10) << 4) | (value % 10);
}