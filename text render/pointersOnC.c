#include <stdio.h>

#define SIZE 50

void try1() 
{
	int x[SIZE];
	int y[SIZE];
	int i;

	for (i = 0; i < SIZE; i++)
	{
		x[i] = y[i];
	}
}

void try2() 
{
	int x[SIZE];
	int y[SIZE];
	int* p1;
	int* p2;

	for (p1 = x, p2 = y; p1 - x < SIZE; *p1++ = *p2++)
		;
}

void try3()
{
	int x[SIZE];
	int y[SIZE];
	int i;

	register int* p1 = x;
	register int* p2 = y;

	for (; p1 < &x[SIZE]; *p1++ = *p2++)
		;
}

void test()
{
	int i = 4;
	int* p = &i;
	*p = 5;
}

int main () 
{
	try1();
	try2();
	try3();
	test();

	getchar();

	return 0;
}