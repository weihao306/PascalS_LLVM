#include "shared.h"
#include "lex.h"

Lex lex;
int curtok;
string source;

int main(int argc, char* argv[])
{ 
    FILE* file;
    if (argc == 2)
    {
        file = fopen(argv[1], "r");
        if (NULL == file) {
            cout << "The file is not exist.";
            exit(1);
        }
        source = argv[1];
    }
    else
    {
        cout << "Use complier.exe <filepath>";
        exit(2);
    }

    GetNextTok(file);
    MainLoop(file);

    return 0;
}