#include <stdio.h>

#define SIZE 5

void arrayCopy()
{
	int x[SIZE];
	int y[SIZE] = {0,1,2,3};
	int* ptr1 = x;
	int* ptr2 = y;

	for (; ptr1 < &x[SIZE]; *ptr1++ = *ptr2++)
		;

	for(int i=0; i<SIZE; i++)
	{
		printf("%d ", x[i]);
	}
	printf("\n");
}

void test()
{
	int my_array[2][3] = {0,0,0, 1,1,1};
	
	int(*ptr)[3] = &my_array[1];
	int value = **(ptr+1);

	int* ptr1 = &value;

	int value2 = *ptr1;

	int value3 = *ptr1;
}

struct TestItem
{
	int a;
	int b;
	int arr[10];
};

int passValueApp(TestItem item)
{
	int value = item.a;
	int value1 = item.b;

	return value;
}

int passReferenceApp(TestItem& item)
{
	int value = item.a;
	int valu1 = item.b;

	return value;
}

int passPointerApp(TestItem* item)
{
	int value = item->a;
	int value1 = item->b;
	
	return value;
}

TestItem returnStructApp()
{
	TestItem item = { 1,2, {3,4,5} };
	return item;
}

int pushParametersOrderApp(int param1, int param2)
{
	int temp1 = param1;
	int temp2 = param2;
	return temp1 + temp2;
}

int main()
{	
	arrayCopy();
	test();

	TestItem item = {1, 4, {1,2,3}};
	passValueApp(item);
	passPointerApp(&item);
	passReferenceApp(item);	
	
	TestItem item1 = returnStructApp();

	int param1 = 1;
	int param2 = 3;
	int sum = pushParametersOrderApp(param1, param2);

	getchar();
	return 0;
}