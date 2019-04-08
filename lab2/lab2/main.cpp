/*14 Выполнить операцию логического ИЛИ для каждой пары соответствующих элементов.*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#include <windows.h>

void main(void)
{
	srand(time(0));
	long int step = 0;
	LARGE_INTEGER frequency, start, finish;
	float delay;
	int j;
	bool arr1[32],arr2[32],res[32],res2[32],res3[32];
	
	for(j=0; j<32; j++)
	{
		arr1[j]=rand() % 2;
		arr2[j]=rand() % 2;
	}

	QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
	while(step<=1000000)
	{
		for(j=0; j < 32; j++)
		{
			res[j]=arr1[j] || arr2[j];
		}
		step++;
	}
	QueryPerformanceCounter(&finish);
	delay = (finish.QuadPart - start.QuadPart) * 1000.0f / frequency.QuadPart;
    printf ("Time C is %f seconds.\n", delay);
	for(j=0; j<32; j++)
		printf ("%d ", res[j]);
	
	
	QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

	int cnt;
	step = 1000000;

	_asm{
		pusha

		xor esi, esi

loop2:
		mov esi, 0

		mov cnt, 020h
loop1:

		mov ah, arr1[esi] ; чтение из памяти

		mov bh, arr2[esi]

		or ah, bh

		mov res2[esi], ah

		add esi, 1 ; bool → шаг по 1 байту

		sub cnt, 1 ; по 1 числу за итерацию

		jnz loop1

		sub step, 1

		jnz loop2

		popa
	}
			
	QueryPerformanceCounter(&finish);
	delay = (finish.QuadPart - start.QuadPart) * 1000.0f / frequency.QuadPart;
    printf ("\nTime ASM wtihout MMX is %f seconds.\n", delay);
	
	for(j=0; j<32; j++)
		printf ("%d ", res2[j]);

	QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

	step = 1000000;

	_asm{
		pusha

		xor esi, esi

loop4:
		mov esi, 0

		mov cnt, 020h
loop3:

		movq MM0, arr1[esi] 

		movq MM1, arr2[esi]

		POR MM0, MM1
		
		movq res3[esi], MM0

		add esi, 8  

		sub cnt, 8

		jnz loop3

		sub step, 1

		jnz loop4

		emms

		popa
	}
			
	QueryPerformanceCounter(&finish);
	delay = (finish.QuadPart - start.QuadPart) * 1000.0f / frequency.QuadPart;
    printf ("\nTime ASM wtih MMX is %f seconds.\n", delay);
	
	for(j=0; j<32; j++)
		printf ("%d ", res3[j]);

	getch();
}
