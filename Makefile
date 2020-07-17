NAME = pascal

LLVM_CONFIG = /usr/bin/llvm-config

NO_WARNING =  -Wno-return-type \
	-Wno-c++11-compat-deprecated-writable-strings \
	-Wno-deprecated-register \
	-Wno-switch 

CXXFLAGS = `$(LLVM_CONFIG) --cppflags` -std=c++11 $(NO_WARNING)
LDFLAGS = `$(LLVM_CONFIG) --ldflags`
LIBS = `$(LLVM_CONFIG) --libs --system-libs`
BIN_NAME = pascal

OBJS = ast.o codegen.o token.o parser.o main.o handle.o

all: exec
	clang++ -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS)

lex: pascal.l
	flex --debug -o token.cpp pascal.l
lex_compile: lex yacc
	clang++ $(CXXFLAGS) -c token.cpp

yacc: pascal.y
	bison --debug -d pascal.y -o parser.cpp
yacc_compile: yacc
	clang++ $(CXXFLAGS) -c parser.cpp

parser.hpp: parser.cpp

exec: lex_compile yacc_compile main.cpp ast.cpp codegen.cpp handle.cpp
	clang++ $(CXXFLAGS) -c main.cpp ast.cpp codegen.cpp handle.cpp

clean:
	@rm -f parser.output *.o parser.cpp parser.hpp token.cpp
