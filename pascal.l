%{
#include <stdio.h>
#include "ast.h"
#include "parser.hpp"
#include "handle.h"

//重定义YY_INPUT，从FILE* file中读取，并做一些记录
#define YY_INPUT(buf,result,max_size)  {\
    result = GetNextChar(buf, max_size); \
    if (  result <= 0  ) \
        result = YY_NULL; \
    }

#define YYLVAL_DEBUG yylval.debug=strdup(yytext)

extern YYLTYPE yylloc;
int yycolumn = 1;
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;\
	yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; \
  yycolumn += yyleng; 

%}

ANY .
DIGIT [0-9]
DIGITS {DIGIT}+
LETTER [_a-zA-Z]
ID {LETTER}({LETTER}|{DIGIT})*
POINT "."{DIGITS}
EXPONENT (e|E)("+"|"-")?{DIGITS}
INTNUM {DIGITS}
REALNUM {DIGITS}({POINT})?({EXPONENT})?
%%

 /* Valid symbols */
";"     { BeginToken(yytext); YYLVAL_DEBUG; return SEMI; }//分号
":="    { BeginToken(yytext); YYLVAL_DEBUG; return ASSIGN; }//赋值
"="     { BeginToken(yytext); YYLVAL_DEBUG; return EQUAL; }//等号
"("     { BeginToken(yytext); YYLVAL_DEBUG; return LP; }//左括号
")"     { BeginToken(yytext); YYLVAL_DEBUG; return RP; }//右括号
"["     { BeginToken(yytext); YYLVAL_DEBUG; return LB; }//左中括号
"]"     { BeginToken(yytext); YYLVAL_DEBUG; return RB; }//右中括号
"."     { BeginToken(yytext); YYLVAL_DEBUG; return DOT; }//点
","     { BeginToken(yytext); YYLVAL_DEBUG; return COMMA; }//逗号
":"     { BeginToken(yytext); YYLVAL_DEBUG; return COLON; }//冒号
"+"     { BeginToken(yytext); YYLVAL_DEBUG; return PLUS; }//加
"-"     { BeginToken(yytext); YYLVAL_DEBUG; return MINUS; }//减
"*"     { BeginToken(yytext); YYLVAL_DEBUG; return MUL; }//乘
"/"     { BeginToken(yytext); YYLVAL_DEBUG; return DIV; }//普通除法，还有一个整除div
">="    { BeginToken(yytext); YYLVAL_DEBUG; return GE; }//大于等于
">"     { BeginToken(yytext); YYLVAL_DEBUG; return GT; }//大于
"<="    { BeginToken(yytext); YYLVAL_DEBUG; return LE; }//小于等于
"<>"    { BeginToken(yytext); YYLVAL_DEBUG; return UNEQUAL; }//不等
"<"     { BeginToken(yytext); YYLVAL_DEBUG; return LT; }//小于

 /* Keyword */
"and"       { BeginToken(yytext); YYLVAL_DEBUG; return AND; }
"array"     { BeginToken(yytext); YYLVAL_DEBUG; return ARRAY; }
"begin"     { BeginToken(yytext); YYLVAL_DEBUG; return _BEGIN; }
"case"      { BeginToken(yytext); YYLVAL_DEBUG; return CASE; }
"const"     { BeginToken(yytext); YYLVAL_DEBUG; return CONST; }
"downto"    { BeginToken(yytext); YYLVAL_DEBUG; return DOWNTO; }
"do"        { BeginToken(yytext); YYLVAL_DEBUG; return DO; }
"div"       { BeginToken(yytext); YYLVAL_DEBUG; return DIV; }
"end"       { BeginToken(yytext); YYLVAL_DEBUG; return END; }
"else"      { BeginToken(yytext); YYLVAL_DEBUG; return ELSE; }
"for"       { BeginToken(yytext); YYLVAL_DEBUG; return FOR; }
"function"  { BeginToken(yytext); YYLVAL_DEBUG; return FUNCTION; }
"if"        { BeginToken(yytext); YYLVAL_DEBUG; return IF; }
"mod"       { BeginToken(yytext); YYLVAL_DEBUG; return MOD;  }
"not"       { BeginToken(yytext); YYLVAL_DEBUG; return NOT; }
"of"        { BeginToken(yytext); YYLVAL_DEBUG; return OF; }
"or"        { BeginToken(yytext); YYLVAL_DEBUG; return OR; }
"program"   { BeginToken(yytext); YYLVAL_DEBUG; return PROGRAM; }
"procedure" { BeginToken(yytext); YYLVAL_DEBUG; return PROCEDURE; }
"record"    { BeginToken(yytext); YYLVAL_DEBUG; return RECORD; }
"repeat"    { BeginToken(yytext); YYLVAL_DEBUG; return REPEAT; }
"to"        { BeginToken(yytext); YYLVAL_DEBUG; return TO; }
"then"      { BeginToken(yytext); YYLVAL_DEBUG; return THEN; }
"type"      { BeginToken(yytext); YYLVAL_DEBUG; return TYPE; }
"until"     { BeginToken(yytext); YYLVAL_DEBUG; return UNTIL; }
"var"       { BeginToken(yytext); YYLVAL_DEBUG; return VAR; }
"while"     { BeginToken(yytext); YYLVAL_DEBUG; return WHILE; }
"integer"|"boolean"|"real"|"char" { BeginToken(yytext); YYLVAL_DEBUG; return SYS_TYPE; }
"false"|"true" { BeginToken(yytext); YYLVAL_DEBUG; return SYS_BOOL; }
"read"      { BeginToken(yytext); YYLVAL_DEBUG; return READ; }
"write"     { BeginToken(yytext); YYLVAL_DEBUG; return SYS_PROC; }

 /* number\indentifer\charactor */
"'"{ANY}"'"     { BeginToken(yytext); YYLVAL_DEBUG; return CHAR; }
{ID}            { BeginToken(yytext); YYLVAL_DEBUG; return ID; }
[ \t]+|\r\n|\n  { BeginToken(yytext); }
{INTNUM}        { BeginToken(yytext); YYLVAL_DEBUG; return INTEGER; }
{REALNUM}       { BeginToken(yytext); YYLVAL_DEBUG; return REAL; }

 /* comment */
"{"{ANY}*"}"    {}
"(*"{ANY}*"*)"  {}

.               { BeginToken(yytext); ParseError = 1; PrintError("Invalid Input"); }
%% 

int yywrap(void)
{
    return 1;
}