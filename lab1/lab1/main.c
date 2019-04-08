/*14 14.	F(x) = (sqrt (x) - 1)/(x+1), x >= 0 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include <windows.h>

void main(void)
{
	long double a = 0, b = 1000, step = 0.01, x, f;
	LARGE_INTEGER frequency, start, finish;
	float delay;
	int counter = 0, one = 1 ;

	x = a;
	f = 0;
	QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

	_asm{
			finit
			fld b
			fld x
loop_start:
			fcom
			fstsw ax
			and ah, 01000101b
			jz loop_end

			fld1
			fld x
			fsqrt
			fsubr
				
			fld1
			fadd x
				
			fdiv 
			fadd f
			fstp f

			/*
			fld counter
			fadd one
			fstp counter
			*/

			fadd step
			fst x

			jmp loop_start
loop_end:
			fwait
	}
	QueryPerformanceCounter(&finish);
	delay = (finish.QuadPart - start.QuadPart) * 1000.0f / frequency.QuadPart;
    printf ("Time ASM is %f seconds.\n", delay);
	printf("%lf\n%d\n", f, counter);

	x = a;
	f = 0;
	counter = 0;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	while(x < (b-step))
	{
		f+=(sqrt(x) - 1)/(x+1);
		x+=step;
		counter++;
	};
	QueryPerformanceCounter(&finish);
	delay = (finish.QuadPart - start.QuadPart) * 1000.0f / frequency.QuadPart;
    printf ("Time C is %f seconds.\n", delay);
	printf("%lf\n%d", f, counter);

	getch();
}
