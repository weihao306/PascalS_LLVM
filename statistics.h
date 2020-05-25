#ifndef STATISTICS_H
#define STATISTICS_H

#include "shared.h"

using namespace std;
class Statistics
{
private:
	int line;
	int low;

public:
	int GetLine(void)
	{
		return this->line;
	}
	int GetLow(void) 
	{
		return this->low;
	}
	Statistics()
	{
		line = 0;
		low = 0;
	}
	void FindStr(string str);
	void FindChar(char ch);
};

#endif // !STATISTICS_H