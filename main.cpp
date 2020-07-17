#include <iostream>
#include <fstream>
#include "ast.h"
#include "parser.hpp"
#include "codegen.h"
#include "handle.h"
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>

extern int yyparse();
extern AST::Program *astRoot;
int ParseError = 0;
FILE* file;
bool isdebug = false;
extern int yydebug;

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        file = fopen(argv[1], "r");
        if(file == NULL)
        {
            fprintf(stdout, "Can't open the pascal source file %s\n", argv[1]);
            exit(1);
        }
    }
    else if(argc == 3)
    {
        if(strcmp(argv[1], "debug") == 0)
        {
            isdebug = true;
            file = fopen(argv[2], "r");
            if(file == NULL)
            {
                fprintf(stdout, "Can't open the pascal source file %s\n", argv[1]);
                exit(1);
            }
        }
        else
        {
            fprintf(stdout, "Please Use %s -debug <file>, Or %s <file> \n", argv[0], argv[0]);
            exit(0);
        }
    }
    else
    {
        fprintf(stdout, "Please Use %s -debug <file>, Or %s <file> \n", argv[0], argv[0]);
        exit(0);
    }

    std::error_code EC;
    std::string irfile = argv[1];
    irfile += ".ll";
    llvm::raw_fd_ostream ir(irfile, EC, llvm::sys::fs::F_None);

    Init_handle();
    yydebug = int(isdebug);
    yyparse();
    Close_handle();
    if(ParseError) return 0;
    std::cout << "syntax check success!" << std::endl;

    /* CodeGenContext context;
    astRoot->CodeGen(context);
    std::cout << std::flush;
    context.Builder.CreateRetVoid();
    context.module->print(llvm::errs(), nullptr);
    context.module->print(ir, nullptr); */

    return 0;
}