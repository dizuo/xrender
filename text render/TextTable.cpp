// 2013-6-17
// 字符串散列表练习
// peteryfren

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN 32
#define TABLE_LEN 31
#define MULT_FACTOR 29989

struct Node
{
	char word[WORD_LEN];
	int count;
	Node* next;
};

unsigned int hash(char* str);
bool isAvailable(char);
void transformArticle(Node* hashTable, char* content);
void insertNodeToHashTable(unsigned int key, char* str, Node* hashTable);
void displayHashTable(Node* hashTable);
void destroyHashTable(Node* hashTable);

void process_string(char* str);

int main()
{
	Node* hashTable = (Node*)malloc(sizeof(Node)*TABLE_LEN);
	memset(hashTable, 0, sizeof(Node)*TABLE_LEN);
	
 	char* content = "The English language belongs to the Anglo-Frisian sub-group of the West Germanic branch of the Germanic family, a member of the Indo-European languages. Modern English is the direct descendant of Middle English, itself a direct descendant of Old English, a descendant of Proto-Germanic. Typical of most Germanic languages, English is characterised by the use of modal verbs, the division of verbs into strong and weak classes, and common sound shifts from Proto-Indo-European known as Grimm's Law. The closest living relatives of English are Scots (spoken primarily in Scotland and parts of Northern Ireland where Ulster Scots is spoken) and Frisian (spoken on the southern fringes of the North Sea in Denmark, the Netherlands, and Germany). \
 		After Scots and Frisian come those Germanic languages that are more distantly related: the non-Anglo-Frisian West Germanic languages (Dutch, Afrikaans, Low German, High German), and the North Germanic languages (Swedish, Danish, Norwegian, Icelandic, and Faroese). With the (partial) exception of Scots, none of the other languages are mutually intelligible with English, owing in part to the divergences in lexis, syntax, semantics, and phonology, and to the isolation afforded to the English language by the British Isles, although some, such as Dutch, do show strong affinities with English, especially to earlier stages of the language. Isolation has allowed English and Scots (as well as Icelandic and Faroese) to develop independently of the Continental Germanic languages and their influences over time.[40]";

	transformArticle(hashTable, content);
	displayHashTable(hashTable);
	destroyHashTable(hashTable);	
	
	// process_string(content);

	getchar();

	return 0;
}

void process_string(char* str)
{
	// 
	char* ptr = str;
	char* pre = str;
	char buffer[WORD_LEN];
	int buffer_len = 0;

	while (*ptr)
	{
		if (isAvailable(*ptr))
		{
			buffer[ptr-pre] = *ptr;			
			++ptr;
		} 
		else 
		{
			buffer[ptr-pre] = 0;
			if (ptr-pre > 0)	// 非空串
				printf("%s\n", buffer);
			memset(buffer, 0, WORD_LEN);
			ptr++;	// 跳过非法字符
			pre = ptr;
		}
	}	
	if (ptr-ptr > 0)
		printf("%s\n", buffer);
}

bool isAvailable(char ch)
{
	return ch=='-' 
		|| (ch>='a' && ch<='z')
		|| (ch>='A' && ch<='Z') ;
}

unsigned int hash(char* ptr)
{
	unsigned int h = 0;
	for (; *ptr; ptr++)
		h = MULT_FACTOR * h + *ptr;
	return h % TABLE_LEN;
}

void transformArticle(Node* hashTable, char* str)
{
	// 
	char* ptr = str;
	char* pre = str;
	char buffer[WORD_LEN];
	int buffer_len = 0;

	while (*ptr)
	{
		if (isAvailable(*ptr))
		{
			buffer[ptr-pre] = *ptr;			
			++ptr;
		} 
		else 
		{
			buffer[ptr-pre] = 0;
			if (ptr-pre > 0)	// 非空串
			{
				insertNodeToHashTable(hash(buffer), buffer, hashTable);
			}
			memset(buffer, 0, WORD_LEN);
			ptr++;	// 跳过非法字符
			pre = ptr;
		}
	}	
	if (ptr-pre > 0) 
	{
		insertNodeToHashTable(hash(buffer), buffer, hashTable);
	}
}

void insertNodeToHashTable(unsigned int key, char* str, Node* hashTable)
{
	Node* pRow = hashTable + key;
	Node* pRowTail = 0;

	if (pRow->count==0) 
	{
		strcpy(pRow->word, str);
		pRow->count = 1;
		return;		
	}
	// traverse list
	for (; pRow; pRow = pRow->next)
	{
		if (0==strcmp(str, pRow->word))
		{
			pRow->count++;
			break;
		}
		pRowTail = pRow;
	}
	if (0==pRow)	
	{
		Node* node = (Node*)malloc(sizeof(Node));
		node->count = 1;
		node->next = 0;
		strcpy(node->word, str);
		pRowTail->next = node;
	}
}

void displayHashTable(Node* hashTable)
{
	Node* pRow = hashTable;
	Node* pCol = 0;
	while(pRow < hashTable+TABLE_LEN)
	{	
		for (pCol = pRow->next; pCol; pCol=pCol->next)
		{
			printf("word=\"%s\",counter = %d ", pCol->word, pCol->count);
		}
		printf("\n");

		pRow++;
	}
}

void destroyHashTable(Node* hashTable)
{
	Node* pRow = hashTable;
	Node* pCol = 0;
	while(pRow < hashTable+TABLE_LEN)
	{	
		pCol = pRow->next;
		while(pCol)
		{
			Node* pTemp = pCol;
			pCol = pCol->next;
			free(pTemp);	// free ptr
		}	
		pRow++;
	}
	free(hashTable);		// free array
}
