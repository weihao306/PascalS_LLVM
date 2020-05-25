#ifndef LEX_H
#define LEX_H

#include "shared.h"
#include "statistics.h"

using namespace std;
class Lex
{
private:
	enum Token {
		tok_eof = -1,

		//commands
		tok_and = -2,
		tok_array = -3,
		tok_begin = -4,
		tok_boolean = -5,
		tok_case = -6,
		tok_const = -7,
		tok_div = -8,
		tok_do = -9,
		tok_downto = -10,
		tok_else = -11,
		tok_end = -12,
		tok_for = -13,
		tok_function = -14,
		tok_if = -15,
		tok_integer = -16,
		tok_mod = -17,
		tok_not = -18,
		tok_of = -19,
		tok_or = -20,
		tok_procedure = -21,
		tok_program = -22,
		tok_real = -23,
		tok_record = -24,
		tok_repeat = -25,
		tok_then = -26,
		tok_to = -27,
		tok_type = -28,
		tok_until = -29,
		tok_var = -30,
		tok_while = -31,

		// primary
		tok_identifier = -50,
		tok_number = -51
	};

public:
	string IdentifierStr; // Filled in if tok_identifier
	double NumVal;             // Filled in if tok_number

	int GetTok(FILE *file);
	void Error(string str);		//illegal identifier
	void Error(char ch);		//illegal character

	int IsLegalChar(int ch);	//judge character
};

#endif // !LEX_H