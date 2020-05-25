#include "lex.h"

int Lex::GetTok(FILE *file)
{	
	static int lastchar = ' ';
    int state = -1;

	// skip any whitespace
	while (isspace(lastchar))
		lastchar = fgetc(file);

	// identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(lastchar) && state == -1) 
    {
        int identnum = 0;
        IdentifierStr = lastchar;
        identnum++;
        while (isalnum(lastchar = fgetc(file)))
        {
            IdentifierStr += lastchar;
            identnum++;
            if (identnum > 8)   //Identifier length is more than 8
            {
                while (isalnum(lastchar = fgetc(file)))
                    IdentifierStr += lastchar;
                string errorstr = IdentifierStr;

                Error(errorstr);
                IdentifierStr = "";
                return 0;
                break;
            }
        }

        // identify keyword
        if (IdentifierStr == "and") return tok_and;
        if (IdentifierStr == "array") return tok_array;
        if (IdentifierStr == "begin") return tok_begin;
        if (IdentifierStr == "boolean") return tok_begin;
        if (IdentifierStr == "case") return tok_case;
        if (IdentifierStr == "const") return tok_const;
        if (IdentifierStr == "div") return tok_div;
        if (IdentifierStr == "do") return tok_do;
        if (IdentifierStr == "downto") return tok_downto;
        if (IdentifierStr == "else") return tok_else;
        if (IdentifierStr == "end") return tok_end;
        if (IdentifierStr == "for") return tok_for;
        if (IdentifierStr == "function") return tok_function;
        if (IdentifierStr == "if") return tok_if;
        if (IdentifierStr == "integer") return tok_integer;
        if (IdentifierStr == "mod") return tok_mod;
        if (IdentifierStr == "not") return tok_not;
        if (IdentifierStr == "of") return tok_of;
        if (IdentifierStr == "or") return tok_or;
        if (IdentifierStr == "procedure") return tok_procedure;
        if (IdentifierStr == "program") return tok_program;
        if (IdentifierStr == "real") return tok_real;
        if (IdentifierStr == "record") return tok_record;
        if (IdentifierStr == "repeat") return tok_repeat;
        if (IdentifierStr == "then") return tok_then;
        if (IdentifierStr == "to") return tok_to;
        if (IdentifierStr == "type") return tok_type;
        if (IdentifierStr == "until") return tok_until;
        if (IdentifierStr == "var") return tok_var;
        if (IdentifierStr == "while") return tok_while;

        return tok_identifier;
    }

    // Number: [0-9]+.[0-9]+
    if (isdigit(lastchar) && state == -1)
    {
        string numstr = "";
        string pointstr = "";
        string estr = "";
        bool point = false;
        bool index = false;

        //get integer part
        do
        {
            numstr += lastchar;
            lastchar = fgetc(file);
        } while (isdigit(lastchar));
        //get decimal part, if exist
        if (lastchar == '.' && point == false)
        {
            point = true;
            lastchar = fgetc(file);
            if (isdigit(lastchar))
            {
                pointstr += '.';
                pointstr += lastchar;
                while (isdigit(lastchar = fgetc(file))) pointstr += lastchar;
                
                if (isalpha(lastchar))  //error: like 223.43fadfad
                {
                    string errorstr = numstr + pointstr + char(lastchar);
                    while (isalnum(lastchar = fgetc(file))) errorstr += lastchar;
                    
                    Error(errorstr);
                    NumVal = 0;
                    return 0;
                }
            }
            else //error: like 212.adfa
            {
                string errorstr = numstr + '.' + char(lastchar);
                while (isalnum(lastchar = fgetc(file))) errorstr += lastchar;

                Error(errorstr);
                NumVal = 0;
                return 0;
            }
        }
        //get index part, if exist
        if (lastchar == 'E' && index == false)
        {
            index = true;
            lastchar = fgetc(file);
            if (isdigit(lastchar) || lastchar == '+' || lastchar == '-')
            {
                estr += 'E';
                estr += lastchar;
                while (isdigit(lastchar = fgetc(file))) estr += lastchar;

                if (isalpha(lastchar))  //error: like 454.2E34fasfdeaf
                {
                    string errorstr = numstr + pointstr;
                    while (isalnum(lastchar = fgetc(file))) errorstr += lastchar;

                    Error(errorstr);
                    NumVal = 0;
                    return 0;
                }
            }
            else    //error: like 21321.32Eyuifhasdjh
            {
                string errorstr;
                if (point == true)
                    errorstr = numstr + pointstr + 'E' + char(lastchar);
                else
                    errorstr = numstr + 'E' + char(lastchar);
                while (isalnum(lastchar = fgetc(file))) errorstr += lastchar;

                Error(errorstr);
                NumVal = 0;
                return 0;
            }          
        }

        if (point == false && index == false)   //[0-9]+
            NumVal = strtod(numstr.c_str(), 0);
        else if (point == true && index == false)   //[0-9]+.[0-9]+
            NumVal = strtod((numstr + pointstr).c_str(), 0);
        else if (point == false && index == true)   //[0-9]+E[+-][0-9]+
            NumVal = strtod((numstr + estr).c_str(), 0);
        else    //[0-9]+.[0-9]+E[+-][0-9]+
            NumVal = strtod((numstr + pointstr + estr).c_str(), 0);
        
        return tok_number;
    }
    
    //EOF
    if (lastchar == EOF)
        return tok_eof;

    //notes
    if (lastchar == '{' && state == -1)
    {
        state = 0;
        lastchar = fgetc(file);
        while (lastchar != '}')
            lastchar = fgetc(file);
        lastchar = fgetc(file);
        state = -1;
        return 0;
    }
    if (lastchar == '(' && state == -1)
    {
        state = 1;
        lastchar = fgetc(file);
        if (lastchar == '*' && state == 1)
        {
            state = 2;
            while (true)
            {
                lastchar = fgetc(file);
                if (lastchar == '*' && state == 2)
                {
                    state = 3;
                    lastchar = fgetc(file);
                    if (lastchar == ')' && state == 3)
                    {
                        state = -1;
                        lastchar = fgetc(file);
                        break;
                    }
                }
            }
        }
        else
        {
            return '(';
        }
    }

    //Otherwise, just return the character as its ascii value.
    if (lastchar == '<' && state == -1)
    {
        lastchar = fgetc(file);
        if (lastchar == '=') 
        {
            lastchar = fgetc(file);
            return 200;     //200 is <=
        }
        else if (lastchar == '>')
        {
            lastchar = fgetc(file);
            return 201;     //201 is <>
        }
        else
        {
            return '<';
        }
    }
    if (lastchar == '>' && state == -1)
    {
        lastchar = fgetc(file);
        if (lastchar == '=')
        {
            lastchar = fgetc(file);
            return 202;     //202 is >=
        }
        else
        {
            return '>';
        }
    }
    if (lastchar == ':' && state == -1)
    {
        lastchar = fgetc(file);
        if (lastchar == '=')
        {
            lastchar = fgetc(file);
            return 203;     //203 is :=
        }
        else
        {
            return ':';
        }
    }

    int thischar = IsLegalChar(lastchar);
    if (thischar != 999) 
    {
        lastchar = fgetc(file);
        return thischar;
    }
    else
    {
        Error(lastchar);
        lastchar = fgetc(file);
        return 0;
    }

    if (lastchar == '\n')
    {
        lastchar = fgetc(file);
        return 0;
    }
}

void Lex::Error(string str)
{
    class Statistics stat;
    stat.FindStr(str);
    cout << "Error! line: " << stat.GetLine() << "  low: " << stat.GetLow() << "  String: \"" << str << "\" is not a legal identifier" << endl;
}

void Lex::Error(char ch)
{
    class Statistics stat;
    stat.FindChar(ch);
    cout << "Error! line: " << stat.GetLine() << "  low: " << stat.GetLow() << "  Character: \"" << char(ch) << "\" is not a legal character" << endl;
}

int Lex::IsLegalChar(int ch)
{
    vector<char> legalchar = { '+','-','*','/','<','>','=','.',',',';','(',')','[',']',':'};
    for (int i = 0; i < legalchar.size(); i++)
    {
        if (ch == legalchar[i])
            return ch;
    }
    return 999;
}