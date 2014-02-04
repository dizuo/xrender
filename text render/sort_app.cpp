// peteryfren 2013/6/18
// 归并排序，函数指针

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SEED 50

void produceSeed(int*);
void printSeed(int*);
void transformSeeds(int* buffer, int length, void(*pfunc)(int*));

void mergeSort(int* buffer, int beg, int end);
void merge(int* buffer, int beg, int mid, int end);

int myStr2Int(char* str)
{
	char* ptr = str;
	int retValue = 0;
	while (*ptr)
	{
		retValue = retValue*10 + (*ptr++ - '0');
	}
	return retValue;
}

int uglyStr2Int(char* str)
{
	char* ptr = str;
	int buffer[100];
	int count = 0;
	while (*ptr)
	{
		buffer[count++] = *ptr++ - '0';
	}

	--count;	
	int factor = 1;
	int retValue = 0;
	while (count >= 0)
	{
		retValue = buffer[count--]*factor + retValue;
		factor *= 10;
	}
	return retValue;
}

int main()
{
	srand((int)time(0));

	int buffer[10];

/*
	transformSeeds(buffer, 10, produceSeed);
	transformSeeds(buffer, 10, printSeed);
	printf("\n");
	
	mergeSort(buffer, 0, 9);

	transformSeeds(buffer, 10, printSeed);
	printf("\n");*/

	char str[] = "123";
	// printf("%d\n", myStr2Int(str));
	printf("%d\n", uglyStr2Int(str));
	getchar();

	return 0;
}

void produceSeed(int* ptr)
{
	*ptr = rand() % MAX_SEED;
}

void printSeed(int* ptr)
{
	printf("%d ", *ptr);
}

void transformSeeds(int* buffer, int length, void(*pfunc)(int*))
{
	for (int idx=0; idx < length; idx++)
	{
		(*pfunc)(buffer+idx);
	}
}


// 分治 递归调用
// 递归结束条件
void mergeSort(int* buffer, int beg, int end)
{
	if (beg < end)
	{
		int mid = ( (beg+end)>>1 );
		mergeSort(buffer, beg, mid);
		mergeSort(buffer, mid+1, end);
		merge(buffer, beg, mid, end);
	}
}

// [beg1, end1], [beg2, end2]
// [beg1, end2]
void merge(int* buffer, int beg, int mid, int end)
{
	// 临时数组
	int tempBuffer[100];
	int idx = 0;
	int idx1 = beg;
	int idx2 = mid+1;

	while (idx1 <= mid && idx2 <= end) 
	{		
		tempBuffer[idx++] = buffer[idx1] < buffer[idx2] ? buffer[idx1++] : buffer[idx2++];
	}
	while (idx1 <= mid)
	{
		tempBuffer[idx++] = buffer[idx1++];
	}
	while (idx2 <= end)
	{
		tempBuffer[idx++] = buffer[idx2++];
	}

	idx1 = beg;
	while (idx1 <= end)
	{
		buffer[idx1++] = tempBuffer[idx1-beg];
	}
}