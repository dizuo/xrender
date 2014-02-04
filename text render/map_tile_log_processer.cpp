#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <list>
using namespace std;

typedef set<string> SetType;
typedef pair<SetType::iterator, bool> RetType;

void process_tile_log(const char* filename) 
{
	ifstream infile(filename);
	char line[1024];	

	SetType uniqueSet;
	RetType ret;
	int repeatHitCounter = 0;
	int totalCounter = 0;
	cout << "==========================\n";

	while ( infile.getline(line, 1024) )
	{
		char* ptr = line;
		for (; *ptr != 'm' && *ptr != '\0'; ptr++)
			;

		// cout << ptr << endl;

		if (*ptr) 
		{
			totalCounter++;

			ret = uniqueSet.insert( string(ptr) );
			if (!ret.second) 
			{
				repeatHitCounter++;
				cout << "ERROR\t" << ptr << endl;
			}
		}

		if (totalCounter >= 100) {
			break;
		}	// µ×Í¼ÍßÆ¬ÎÆÀí»º´æ×î¶à100¸ö£¡
	}
	float rate = totalCounter > 0 ? (repeatHitCounter*1.0f / totalCounter) : 0;
	cout << "repeat hit rate = " << rate << endl;
	cout << "total hit = " << totalCounter << "\t" << "repeat hit = " << repeatHitCounter << endl;
	cout << "==========================\n";

	infile.close();
	uniqueSet.clear();
}

struct Item
{
	int data;

	Item(int i) : data(i) {}
};

int main() 
{
// 	process_tile_log("tile_beforeOp.txt");
// 	process_tile_log("tile_afterOp.txt");
	
	typedef list<Item*> ListType;
	typedef ListType::iterator IterType;

	ListType item_list;
	for (int i = 0; i < 10; i++)
	{
		item_list.push_back( new Item(i) );
	}

	IterType beg = item_list.begin();
	printf("%d\n", (*beg)->data); 

	for (IterType iter = item_list.begin();
		iter != item_list.end();
		iter++) 
	{
		printf("%d ", (*iter)->data);
		delete *iter;
	}


	getchar();
	return 0;	
}
