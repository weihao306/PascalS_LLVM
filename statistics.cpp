#include "statistics.h"
extern string source;

void Statistics::FindStr(string str)
{
	fstream file(source, ios_base::in);

	string linestr;
	while (!file.eof())
	{
		line++;
		getline(file,linestr);
		string::size_type pos = linestr.find(str);
		if (pos != linestr.npos)
		{
			low = int(pos);
			break;
		}
		else
			continue;
	}
	file.close();
}

void Statistics::FindChar(char ch)
{
	fstream file(source, ios_base::in);

	string linestr;
	while (!file.eof())
	{
		line++;
		getline(file, linestr);
		string::size_type pos = linestr.find(ch);
		if (pos != linestr.npos)
		{
			low = int(pos);
			break;
		}
		else
			continue;
	}
	file.close();
}
