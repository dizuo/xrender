#include <iostream>
#include <cassert>
using namespace std;

#define EXTRACT_CLASS_NAME(classname) (#classname)
	
class GlobalVar {
public:
	GlobalVar() 
	{
		cout << EXTRACT_CLASS_NAME(GlobalVar) << endl;
	}

};

GlobalVar g_var;
int g_initIntVar = 0;
int g_noInitIntVar;

void array_trim(int* buffer, int maxCursor, int size);
void array_print(int* buffer, int size);

void process_string(char *tokens, char* str);

int main() 
{
	printf("init var=%d\n", g_initIntVar);
	printf("no init var=%d\n", g_noInitIntVar);

	// int colors[] = {1, 0, 1, 1, 1, 3, 0, 3, 6, 0, 6, 8, 0, 8, 11};
	int colors2[] = {1,0,3, 0,3,12, 3,12,15, 1,15,16, 0,16,23, 0,23,27, 
		1,27,36, 0,36,37, 0,37,46, 1,46,48, 1,48,57, 2,57,63, 0,63,112,
		0,112,124, 0,124,167, 0,167,187, 0,187,239, 0,239,246, 0,246,262, 
		0,262,282, 3,282,285, 3,285,287};
 	int length = sizeof(colors2) / sizeof(colors2[0]);
	int maxCursor = colors2[length-1];
	// array_trim(colors2, maxCursor, length);

	char* dst_str = "#C255 244 233 22##H5##a1#";
	process_string("# ", dst_str);

	getchar();
	return 0;
}

void array_print(int* buffer, int size)
{
	for (int cursor = 0; cursor < size; cursor++) 
	{
		printf("%d,", buffer[cursor]);
	} printf("\n");
}

void array_trim(int* buffer, int maxCursor, int length) 
{
	// int* array2 = new int[maxCursor+1];
	int array2[500] = {0};
	int array2Length = maxCursor + 1;

	int colorSec = 0;
	int arr2Cursor = 0;
	for (int cursor = 0; cursor < array2Length; cursor++) 
	{
		int color = buffer[3*colorSec];
		int end = buffer[3*colorSec+2];
		if (cursor==end && colorSec < length/3-1)
		{
			colorSec++;			
			color = buffer[3*colorSec];
		}
		array2[arr2Cursor++] = color;
	}

	assert( array2Length==arr2Cursor );

	// ·ÖÉ¢
	array_print(array2, arr2Cursor);
 
	// ¾ÛºÏ
	int resCursor = 0;
	while (resCursor < arr2Cursor) {
		int color = array2[resCursor];
		int begin = resCursor;

		for (; resCursor < arr2Cursor && color == array2[resCursor]; resCursor++)
			;

		if (resCursor > arr2Cursor-1) {
			resCursor = arr2Cursor-1;
		}
		printf("(%d, %d,%d)  ", color, begin, resCursor);		 
		if (resCursor==arr2Cursor-1) {
			break;
		}
	} // while loop

	// delete[] array2;
}

void process_string(char *tokens, char* str)
{
	char bitmap[256] = {0};
	int value = 0;

	char* ptr = tokens;
	while (bitmap[*ptr++]++, *ptr) 
		;
	
	ptr = str;
	while (*ptr) 
	{
		if (bitmap[*ptr] > 0)
		{
			// Êä³ö»º´æÄÚÈÝ
			if (value > 0)
				printf("%d\n", value);

			value = 0;			
		} else if ( (*ptr >= 'a' && *ptr <= 'z')	// ×ÖÄ¸
			|| (*ptr >= 'A' && *ptr <= 'Z') )
		{
			printf("%c\n", *ptr);			
		} else if (*ptr >= '0' && *ptr <= '9')		// Êý×Ö
		{
			value *= 10;		// ×Ö·û´®×ª»¯Êý×Ö
			value += (*ptr-'0');
		}
		
		ptr++;
	}
}