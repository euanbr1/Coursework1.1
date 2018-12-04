#pragma once
#include <string>
#include <vector>

using namespace std;

class cHighScoreTable
{
private:
	struct Item {
		string Name;
		int score;

		bool operator < ( const Item& rhs) const
		{
			return rhs.score < score;
		}
	};

	int tableSize;
	int MAX_SIZE = 10;

	Item tableEntry;

	vector<Item*> tableEntries;
	vector<Item*>::iterator row;


public:
	cHighScoreTable();
	cHighScoreTable(int maxSize);

	bool loadFromFile(string highScoreFile);
	bool saveToFile(string highScoreFile);

	int  addItem(Item entry);
	Item getItem(int row);
	void clearTable();
	string convertToString();
	int getTableSize();
	void setTableSize(int theSize);

	virtual ~cHighScoreTable();
};

