#include <stdio.h>


int InternalFunctionA(int nSizeA1, int nSizeA2)
{
	int localnSizeA1 = nSizeA1;
	int localnSizeA2 = nSizeA2;

	int nFunctionA = localnSizeA1 + localnSizeA2;

	return nFunctionA;
}

int InternalFunctionB(int nSizeB1, int nSizeB2)
{

	int nFunctionA = InternalFunctionA(nSizeB1, nSizeB2);
	return 0;
}

int main()
{
	int nFunctionVal = InternalFunctionB(36, 64);
	
	getchar();

	return 0;
}