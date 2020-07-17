%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ast.h"
#include "parser.hpp"
#include "handle.h"

AST::Program* astRoot;
extern int yylex(void);
void yyerror(char const *str);

%}

%locations

%union{
    char *                          debug;
    AST::Node*                      ast_Node;
    AST::Identifier*                ast_Identifier;
    AST::Statement*                 ast_Statement;
    AST::Expression*                ast_Expression;
    AST::Program*                   ast_Program;
    AST::Routine*                   ast_Routine;
    AST::RoutineHead*               ast_RoutineHead;
    AST::TypeDecl*                  ast_TypeDecl;
    AST::TypeDefine*                ast_TypeDefine;
    AST::VarDecl*                   ast_VarDecl;
    AST::AssignmentStmt*            ast_Assign;
    AST::ConstDecl*                 ast_ConstDecl;
    AST::ConstValue*                ast_ConstValue;
    AST::RecordType*                ast_RecordType;
    AST::RoutineDecl*               ast_RoutineDecl;
    AST::RoutineBody*               ast_RoutineBody;
    AST::CaseStmt*                  ast_CaseStmt;
    AST::SwitchStmt*                ast_SwitchStmt;
    AST::ParaDecl*                  ast_ParaDecl;

    AST::StatementList*             ast_StatementList;
    AST::VarDeclList*               ast_VarDeclList;
    AST::IdentifierList*            ast_IdentifierList;
    AST::RoutineDeclList*           ast_RoutineDeclList;
    AST::ExpressionList*            ast_ExpressionList;
    AST::ConstDeclList*             ast_ConstDeclList;
    AST::TypeDefineList*            ast_TypeDefineList;
    //AST::CaseList*                  ast_CaseList;
    AST::ParaDeclList*              ast_ParaDeclList;
}

%token ASSIGN LP RP LB RB DOT COMMA COLON SEMI
%token ARRAY _BEGIN SYS_TYPE CONST END FUNCTION PROGRAM
%token PROCEDURE RECORD VAR ID TYPE 
%token EQUAL UNEQUAL GE GT LE LT AND OR NOT     //布尔表达式
%token PLUS MINUS MUL DIV DIVI MOD  //四则运算
%token INTEGER REAL CHAR SYS_BOOL  //变量类型
%token READ SYS_PROC  //read()和write()
%token DOWNTO DO REPEAT TO THEN WHILE UNTIL FOR
%token IF ELSE CASE OF

%type <debug> ASSIGN LP RP LB RB DOT COMMA COLON SEMI
%type <debug> ARRAY _BEGIN SYS_TYPE CONST END FUNCTION PROGRAM
%type <debug> PROCEDURE RECORD VAR ID TYPE 
%type <debug> EQUAL UNEQUAL GE GT LE LT AND OR NOT     //布尔表达式
%type <debug> PLUS MINUS MUL DIV DIVI MOD  //四则运算
%type <debug> INTEGER REAL CHAR SYS_BOOL  //变量类型
%type <debug> READ SYS_PROC  //read()和write()
%type <debug> DOWNTO DO REPEAT TO THEN WHILE UNTIL FOR
%type <debug> IF ELSE CASE OF

%type <ast_Program> program
%type <ast_Identifier> program_head 
%type <ast_Routine> routine sub_routine
%type <ast_RoutineHead> routine_head 
%type <ast_RoutineBody> routine_body
%type <ast_TypeDecl> type_decl simple_type_decl array_type_decl record_type_decl range_type_decl
%type <ast_TypeDefine> type_define
%type <ast_Statement> proc_stmt stmt else_clause for_stmt repeat_stmt while_stmt if_stmt
%type <ast_RoutineDecl> func_decl proc_decl func_head proc_head
%type <ast_Assign> assign_stmt
%type <ast_Expression> expression simple_expression term factor
%type <ast_ConstValue> const_variable unsign_const_variable
%type <ast_CaseStmt> case_expr
%type <ast_SwitchStmt> case_expr_list case_stmt
%type <ast_VarDecl> var_decl
%type <ast_ParaDecl> para_type_list

%type <ast_StatementList> compound_stmt stmt_list
%type <ast_TypeDefineList> type_part type_decl_list
%type <ast_VarDeclList> var_part var_decl_list record_body
%type <ast_RoutineDeclList> routine_part
%type <ast_IdentifierList> identifier_list
%type <ast_ExpressionList> expression_list
%type <ast_ConstDeclList> const_decl_list const_part
%type <ast_ParaDeclList> parameters para_decl_list

%start program

%%

program: 
    program_head routine DOT { $$ = new AST::Program($1, $2); astRoot = $$; }
;
program_head:
    PROGRAM ID LP identifier_list RP SEMI { $$ = new AST::Identifier($2); }
;
identifier_list:
    identifier_list COMMA ID { $$ = $1; $$->insert($3); }
    | ID { $$ = new AST::IdentifierList(); $$->insert($1); }
;
routine:
    routine_head routine_body { $$ = new AST::Routine($1, $2); }
;
sub_routine: 
    routine_head routine_body { $$ = new AST::Routine($1,$2); }
;
routine_head:
    const_part type_part var_part routine_part { $$ = new AST::RoutineHead($1, $2, $3, $4); }
;
routine_body:
    compound_stmt { $$ = new AST::RoutineBody($1); }
;
const_part:
    CONST const_decl_list SEMI { $$ = $2; }
    | { $$ = new AST::ConstDeclList(); }
;
const_decl_list:
    const_decl_list SEMI ID EQUAL const_variable { 
        $$ = $1; 
        $$->const_decl_list.push_back(new AST::ConstDecl(new AST::Identifier($3), $5));
    }
    | ID EQUAL const_variable {
        $$ = new AST::ConstDeclList();
        $$->const_decl_list.push_back(new AST::ConstDecl(new AST::Identifier($1), $3));
    }
;
const_variable:
    PLUS INTEGER { $$ = new AST::IntegerType(atoi($2)); }
    | MINUS INTEGER { $$ = new AST::IntegerType(-atoi($2)); }
    | INTEGER { $$ = new AST::IntegerType(atoi($1)); }
    | PLUS REAL { $$ = new AST::RealType(atof($2)); }
    | MINUS REAL { $$ = new AST::RealType(-atof($2)); }
    | REAL { $$ = new AST::RealType(atof($1)); }
    | CHAR { $$ = new AST::CharType($1); }
    | SYS_BOOL  { $$ = new AST::BooleanType($1);}
;
type_part:
    TYPE type_decl_list SEMI { $$ = $2; }
    | { $$ = new AST::TypeDefineList(); }
;
type_decl_list:
    type_decl_list SEMI type_define{ $$ = $1; $$->type_define_list.push_back($3); }
    | type_define { $$ = new AST::TypeDefineList(); $$->type_define_list.push_back($1); }
;
type_define:
    ID EQUAL type_decl { $$ = new AST::TypeDefine(new AST::Identifier($1), $3); }
;
type_decl:
    simple_type_decl { $$ = $1; }
    | array_type_decl { $$ = $1; }
    | record_type_decl { $$ = $1; }
;
simple_type_decl:
    SYS_TYPE { $$ = new AST::TypeDecl($1); }
    | ID { $$ = new AST::TypeDecl(std::string($1)); }
;
range_type_decl:
    INTEGER DOT DOT INTEGER { $$ = new AST::TypeDecl(new AST::RangeType(atoi($1), atoi($4))); }
    | CHAR DOT DOT CHAR { $$ = new AST::TypeDecl(new AST::RangeType(*$1, *$4)); }
    | SYS_BOOL DOT DOT SYS_BOOL { $$ = new AST::TypeDecl(new AST::RangeType(std::string($1) == "true" ? 1 : 0, std::string($4) == "true" ? 1 : 0)); }
    | MINUS INTEGER DOT DOT INTEGER	{ $$ = new AST::TypeDecl(new AST::RangeType(-atoi($1), atoi($5))); }
	| MINUS INTEGER DOT DOT MINUS INTEGER { $$ = new AST::TypeDecl(new AST::RangeType(-atoi($1), -atoi($2))); }
    | ID DOT DOT ID { $$ = new AST::TypeDecl(new AST::RangeType(std::string($1), std::string($4))); }
;
array_type_decl:
    ARRAY LB range_type_decl RB OF type_decl { $$ = new AST::TypeDecl(new AST::ArrayType($3, $6)); }
;
record_type_decl:
    RECORD record_body END { $$ = new AST::TypeDecl(new AST::RecordType($2)); }
;
record_body:
    var_decl_list { $$ = $1; }
    | { $$ = new AST::VarDeclList(); }
;
var_decl_list:
    var_decl_list SEMI var_decl 	{ $$ = $1; $$->insert($3); }
    | var_decl 	{ $$ = new AST::VarDeclList(); $$->insert($1); }
;
var_decl:
	identifier_list COLON type_decl { $$ = new AST::VarDecl($1, $3); }
;
var_part:
    VAR var_decl_list SEMI { $$ = $2; }
    | { $$ = new AST::VarDeclList(); }   
;
routine_part:
    routine_part func_decl { $$ = $1; $$->insert($2); }
    | routine_part proc_decl { $$ = $1; $$->insert($2); }
    | { $$ = new AST::RoutineDeclList(); }
;
func_decl:
    func_head SEMI sub_routine SEMI { $$ = $1; $$->sub_routine = $3; }
;
func_head:
    FUNCTION ID parameters COLON simple_type_decl {
        $$ = new AST::RoutineDecl(AST::RoutineDecl::FUNCTION, new AST::Identifier($2), $3,$5);
    }
;
proc_decl:
    proc_head SEMI sub_routine SEMI { $$ = $1; $$->sub_routine = $3; }
;
proc_head:
    PROCEDURE ID parameters {
        $$ = new AST::RoutineDecl(AST::RoutineDecl::PROCEDURE, new AST::Identifier($2), $3, nullptr);
    }
;
parameters:
    LP para_decl_list RP { $$ = $2; }
    | { $$ = new AST::ParaDeclList(); }
;
para_decl_list:
	para_decl_list SEMI para_type_list 	{
        $$ = $1;
        $$->insert($3);
    }
    | para_type_list {
        $$ = new AST::ParaDeclList();
        $$->insert($1);
    }	
;
para_type_list:
	VAR identifier_list COLON simple_type_decl {
        $$ = new AST::ParaDecl($2,$4);
    }	
    | identifier_list COLON simple_type_decl {
        $$ = new AST::ParaDecl($1,$3);
    }
;
compound_stmt:
    _BEGIN stmt_list END { $$ = $2; }
;
stmt_list:
    stmt_list stmt SEMI { $$ = $1; $$->insert($2); }
    | { $$ = new AST::StatementList(); }
;
stmt:
    assign_stmt { $$ = dynamic_cast<AST::Statement *>($1); }
    | proc_stmt { $$ = dynamic_cast<AST::Statement *>($1); }
    | compound_stmt { $$ = dynamic_cast<AST::Statement *>($1); }
    | if_stmt { $$ = dynamic_cast<AST::Statement *>($1); }
    | repeat_stmt { $$ = dynamic_cast<AST::Statement *>($1); }
    | while_stmt { $$ = dynamic_cast<AST::Statement *>($1); }
    | for_stmt { $$ = dynamic_cast<AST::Statement *>($1); }
    | case_stmt { $$ = dynamic_cast<AST::Statement *>($1); }
;
assign_stmt:
    ID ASSIGN expression { $$ = new AST::AssignmentStmt(new AST::Identifier($1), $3); }
    | ID LB expression RB ASSIGN expression { 
        $$ = new AST::AssignmentStmt(new AST::ArrayRef(new AST::Identifier($1), $3), $6); 
    }
    | ID DOT ID ASSIGN expression { 
        $$ = new AST::AssignmentStmt(new AST::RecordRef(new AST::Identifier($1), new AST::Identifier($3)), $5); 
    }
;
proc_stmt:
    ID { $$ = new AST::ProcCall(new AST::Identifier($1)); }
    | ID LP expression_list RP { $$ = new AST::ProcCall(new AST::Identifier($1), $3); }
    | SYS_PROC { $$ = new AST::SysProcCall(new AST::Identifier($1)); }
    | SYS_PROC LP expression_list RP { $$ = new AST::SysProcCall(new AST::Identifier($1), $3); }
    | READ LP expression_list RP { $$ = new AST::SysProcCall(new AST::Identifier($1), $3); }
;
if_stmt:
    IF expression THEN stmt else_clause { $$ = (AST::Statement*)new AST::IfStmt($2, $4, $5); }
;
else_clause:
    ELSE stmt { $$ = $2; }
    | { $$ = nullptr; }
;
repeat_stmt:
    REPEAT stmt_list UNTIL expression { $$ = (AST::Statement*)new AST::RepeatStmt($4,$2); }
;
while_stmt:
    WHILE expression DO stmt { $$ = new AST::WhileStmt($2, $4); }
;
for_stmt:
    FOR ID ASSIGN expression TO expression DO stmt {
        $$ = new AST::ForStmt(new AST::Identifier($2), $4, $6, 1, $8);
    }
    | FOR ID ASSIGN expression DOWNTO expression DO stmt {
        $$ = new AST::ForStmt(new AST::Identifier($2), $4, $6, 1, $8);
    }
;
case_stmt:
    CASE expression OF case_expr_list END { $$ = $4; $$->condition = $2; }
    | CASE expression OF END { $$ = nullptr; }
;
case_expr_list:
    case_expr_list SEMI case_expr { $$ = $1; $$->insert($3); }
    | case_expr { $$ = new AST::SwitchStmt(); $$->insert($1); }
;
case_expr:
    const_variable COLON stmt {
        $$ = new AST::CaseStmt(static_cast<AST::IntegerType*>($1), $3);
    }
    | ID COLON stmt SEMI {$$=new AST::CaseStmt(new AST::Identifier($1),$3);}
;
expression_list:
    expression_list COMMA expression { $$ = $1; $$->insert($3); }
    | expression { $$ = new AST::ExpressionList(); $$->insert($1); }
;
expression:
    expression GE simple_expression { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::ge, $3); }
    | expression GT simple_expression { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::gt, $3); }
    | expression LE simple_expression { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::le, $3); }
    | expression LT simple_expression { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::lt, $3); }
    | expression EQUAL simple_expression { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::eq, $3); }
    | expression UNEQUAL simple_expression { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::ne, $3); }
    | simple_expression { $$ = $1; }
;
simple_expression:
    simple_expression PLUS term { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::plus, $3); }
    | simple_expression MINUS term { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::minus, $3); }
    | simple_expression OR term { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::bit_or, $3); }
    | term { $$ = $1; }
;
term:
    factor { $$ = $1; }
    | term MUL factor { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::mul, $3); }
    | term DIV factor { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::div, $3); }
    | term MOD factor { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::mod, $3); }
    | term AND factor { $$ = new AST::BinaryOperator($1, AST::BinaryOperator::OpType::bit_and, $3); }
;
factor:
    ID { $$ = new AST::Identifier($1); }
    | ID LP expression_list RP { $$ = new AST::FuncCall(new AST::Identifier($1), $3); }
    | unsign_const_variable { $$ = $1; };
	| LP expression  RP { $$ = $2; }
	| NOT factor { $$ = new AST::BinaryOperator(new AST::BooleanType("true"), AST::BinaryOperator::OpType::bit_xor, $2);}
	| MINUS factor { $$ = new AST::BinaryOperator(new AST::IntegerType(0), AST::BinaryOperator::OpType::minus, $2);}
	| ID LB expression RB { $$ = new AST::ArrayRef(new AST::Identifier($1), $3); }
	| ID DOT ID	{ $$ = new AST::RecordRef(new AST::Identifier($1), new AST::Identifier($3)); }
;
unsign_const_variable:
    INTEGER { $$ = new AST::IntegerType(atoi($1)); }
    | REAL { $$ = new AST::RealType(atof($1)); }
    | CHAR { $$ = new AST::CharType($1); }
;

%%

void yyerror(char const *str)
{
    PrintError(str);
	ParseError = 1;
}
// int main(void)
// {
//     printf("Pascal_S\n");
//     yyparse();
//     return 0;
// }