// peteryfren 2013/6/20
// 堆，优先级队列，堆排序

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>

#define MAX_SEED 500
#define MAX_LEN 10

void minHeapSiftUp(int* buffer, int length, int pos);
void minHeapSiftDown(int* buffer, int length, int pos);
void minHeapInsert(int* buffer, int value, int* length);
int minHeapExtract(int* buffer, int* value, int* length);
void swap(int* left, int* right);

int main()
{
	srand((int)time(0));
	int buffer[MAX_LEN];

	int heapLength = 0;
	for (int i=0; i<MAX_LEN; i++)
	{
		minHeapInsert(buffer, rand()%MAX_SEED, &heapLength);
	}
	
	int value = 0;
	for (int i=0; i<MAX_LEN; i++)
	{
		if (minHeapExtract(buffer, &value, &heapLength)==1)
			printf("%d ", value);
	}
	
	printf("\n");
	for (int i=0; i<MAX_LEN; i++)
	{
		printf("%d ", buffer[i]);
	}
	printf("\n");

	getchar();
	return 0;
}

void minHeapInsert(int* buffer, int value, int* length)
{	
	if (*length >= MAX_LEN)
		return;
	
	buffer[*length] = value;
	(*length)++;
	
	minHeapSiftUp(buffer, *length, *length-1);
}


int minHeapExtract(int* buffer, int* value, int* length)
{
	if (*length <= 0)
		return 0;
	
	*value = buffer[0];
	// buffer[0] = buffer[*length-1];
	swap(buffer+0, buffer+*length-1);
	minHeapSiftDown(buffer, *length-1, 0);
	(*length)--;
	return 1;
}

void swap(int* left, int* right)
{
	int temp = *left;
	*left = *right;
	*right = temp;
}

void minHeapSiftUp(int* buffer, int length, int pos)
{
	if (pos >= length)
		return;

	while (pos > 0)	// loop exit condition
	{
		int father = (pos-1)/2;		// father pos
		int value = buffer[pos];	// current value

		if (buffer[father] < value)
			break;

		swap(buffer+father, buffer+pos);
		pos = father;				// update for next loop
	}
}

void minHeapSiftDown(int* buffer, int length, int pos)
{
	if (pos >= length)
		return;

	while (pos < length)
	{
		int value = buffer[pos];
		int cursor = (pos<<1)+1;

		if (cursor >= length)
			break;
		
		if (cursor+1 < length && buffer[cursor+1] < buffer[cursor])
			cursor++;

		if (value < buffer[cursor])	// 已经满足小顶堆特性
			break;

		swap(buffer+cursor, buffer+pos);
		pos = cursor;
	}

}
