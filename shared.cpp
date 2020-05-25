#include "shared.h"

extern int curtok;
extern Lex lex;

int GetNextTok(FILE* file)
{
	return curtok = lex.GetTok(file);
}

void MainLoop(FILE *file)
{
	while (true)
	{
		switch (curtok)
		{
		case -1:
			return;
		case 0:
			GetNextTok(file);
		default:
			cout << curtok << endl;
			GetNextTok(file);
		}
	}
}