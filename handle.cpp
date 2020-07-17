#include <iostream>
#include "handle.h"

using namespace std;

void Init_handle(void);
void Close_handle(void);
void PrintRow(void); 
int  GetNextChar(char *b, int maxBuffer);
void BeginToken(char*);
void PrintError(char *s, ...);

static int eof=0;
static int row_number=0;
static int Buffer_length=0;
static int Buffer_next=0;//lex 的下一个字符
static int TokenStart=0;//bison token start
static int TokenLength=0;//bison token length
static int TokenNextStart=0;
static int BufferMaxLen=2000;
static char *buffer;

void Init_handle()
{
    buffer = (char*)malloc(BufferMaxLen);
}
void Close_handle()
{
    free(buffer);
}
void PrintRow()
{
    if (row_number == 0) return;
    fprintf(stdout,"%6d |%.*s\n",row_number,Buffer_length,buffer);
}
void PrintError(const char *s, ...)
{
    static char errmsg[10000];
    va_list args;

    int start=TokenStart;
    int end=start + TokenLength - 1;
    int i;
    PrintRow();

    if (eof) {
        fprintf(stdout, "...... !");
        for (i=0; i<Buffer_length; i++)
        fprintf(stdout, ".");
        fprintf(stdout, "^-EOF\n");
    }
    else {
        fprintf(stdout, "...... !");
        for (i=1; i<start; i++)
        fprintf(stdout, ".");
        for (i=start; i<=end; i++)
        fprintf(stdout, "^");
        for (i=end+1; i<Buffer_length; i++)
        fprintf(stdout, ".");
        fprintf(stdout, "   token from %d to %d\n", start, end);
    }

    va_start(args, s);
    vsprintf(errmsg, s, args);
    va_end(args);
    
    fprintf(stdout, "Error: %s\n", errmsg);
}

int getNextLine()
{
    int i;
    char *p;
    
    Buffer_next = 0;
    TokenStart = -1;
    TokenNextStart = 1;
    eof = false;

    p = fgets(buffer, BufferMaxLen, file);
    if (p == NULL) {
        if (  ferror(file)  )
        return -1;//error
        eof = true;
        return 1;//tail
    }

    for(int i = 0; i < BufferMaxLen; i++)
    {
        if((buffer[i] == '\r' && buffer[i+1] == '\n') || buffer[i] == '\n')
        {
            buffer[i] = '\0';
            break;
        }
    }

    row_number += 1;
    Buffer_length = strlen(buffer);

    return 0;
}
static char DumpChar(char c) 
{
    if (isprint(c))
        return c;
    return '@';
}
int GetNextChar(char* b,int maxLen)
{
    if (eof) return 0;
    while (Buffer_next>=Buffer_length)
    {
        if (getNextLine()!=0) return 0;
    }
    b[0]=buffer[Buffer_next];
    Buffer_next++;

    /* if (isdebug)
    printf("GetNextChar() => '%c'0x%02x at %d\n",
                        DumpChar(b[0]), b[0], Buffer_next); */

    return b[0]==0?0:1;
}
static char *DumpString(char *s) 
{
    static char buf[101];
    int i;
    int n = strlen(s);

    if (  n > 100  )
        n = 100;

    for (i=0; i<n; i++)
        buf[i] = DumpChar(s[i]);
    buf[i] = 0;
    return buf;
}
void BeginToken(char *t)
{
    TokenStart=TokenNextStart;
    TokenLength=strlen(t);
    TokenNextStart=Buffer_next;

    yylloc.first_line = row_number;
    yylloc.first_column = TokenStart;
    yylloc.last_line = row_number;
    yylloc.last_column = TokenStart + TokenLength - 1;

/*     if (isdebug) 
    {
        printf("Token '%s' at %d-%d next at %d\n", DumpString(t),
                        yylloc.first_column,
                        yylloc.last_column, TokenNextStart);
    } */
}

