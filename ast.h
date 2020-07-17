#ifndef AST_H
#define AST_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <climits>
#include "llvm/IR/Value.h"

#include "codegen.h"

namespace AST
{
    class Program;      
    class Identifier;   //标识符
    class Routine;
    class RoutineHead;
    class RoutineBody;
    class ConstDecl;    //常量声明
    class TypeDecl;     //类型声明
    class VarDecl;      //变量声明
    class Statement;    //语句 
    class Expression;   //表达式
    class IntegerType;  
    class RealType;  
    class CharType;
    class BooleanType;
    class RangeType;
    class ArrayType;
    class RecordType;
    // class RecordBody;
    class ConstValue;   //常量
    class TypeDefine;    
    class CaseStmt;
    class ParaDecl;

    class IdentifierList;
    class StatementList;  
    class ExpressionList;
    class ParaDeclList;
    class VarDeclList;
    class TypeDefineList;
    class ConstDeclList;
    class RoutineDeclList;

    enum SysType
    {
        Integer,
        Real,
        Char,
        Boolean
    };

    enum
    {
        NODE = 0,
        PROGRAM,
        IDENTIFIER,
        ROUTINE,
        ROUTINE_HEAD,
        CONST_DECL_LIST,
        CONST_DECL,
        CONST_VALUE,
        TYPE_DEFINE_LIST,
        TYPE_DEFINE,
        TYPE_DECL,
        SIMPLE_TYPE_DECL,
        ARRAY_TYPE_DECL,
        RECORD_TYPE_DECL,
        STMT_LIST
    } NodeType;

    class Syscon{
    public:
        bool True = true;
        bool False = false;
    };

    class Node{
    public:
        int _type = NODE;

        virtual ~Node() {}

        //virtual llvm::Value *CodeGen(CodeGenContext& context) = 0;
    };

    class Statement : public Node{
    public:
        Statement() {};
        virtual std::vector<Statement *> *getlist() {}
        //virtual llvm::Value *CodeGen(CodeGenContext& context) {}
    };

    class StatementList : public Statement{
    public:
        std::vector<Statement*> list;

        StatementList() { this->_type = STMT_LIST; }
        void insert(Statement *node) { list.push_back(node); }
        virtual std::vector<Statement *> *getlist() { return &list; }

       //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class Program : public Node{
    public:
        Routine *routine = nullptr;
        Identifier *program_head = nullptr;

        Program() { this->_type = PROGRAM; }
        Program(Identifier *head, Routine *body) : routine(body), program_head(head) { this->_type = PROGRAM; }

        //virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class Routine : public Node{
    public:
        RoutineHead *head = nullptr;
        RoutineBody *body = nullptr;

        Routine() { this->_type = ROUTINE; }
        Routine(RoutineHead *h, RoutineBody *b) : head(h), body(b) { this->_type = ROUTINE; }

        //virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class RoutineHead : public Node{
    public:
        ConstDeclList *const_part = nullptr;
        VarDeclList *var_part = nullptr;
        TypeDefineList *type_part = nullptr;
        RoutineDeclList *routine_part = nullptr;

        RoutineHead(ConstDeclList *c, TypeDefineList *t, VarDeclList *v, RoutineDeclList *r) : 
            const_part(c), var_part(v), type_part(t), routine_part(r) {}
        
        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class RoutineBody : public Node{
    public:
        StatementList *list;

        RoutineBody(StatementList *node) : list(node) {}

        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class RoutineDecl : public Node{
    public:
        enum{
            PROCEDURE,
            FUNCTION
        };
        int type;
        Identifier *id = nullptr;
        ParaDeclList *para_decl_list = nullptr;
        TypeDecl *type_decl = nullptr;
        Routine *sub_routine = nullptr;

        RoutineDecl(int fORp, Identifier *idd, ParaDeclList *para, TypeDecl *type_, Routine *sub_ = nullptr) :
            sub_routine(sub_), type(fORp), id(idd), para_decl_list(para), type_decl(type_) {}
        
        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class RoutineDeclList : public Node{
    public:
        std::vector<RoutineDecl *> list;

        RoutineDeclList() {}
        RoutineDeclList(std::vector<RoutineDecl *> &ls) : list(ls) {}   
        void insert(RoutineDecl *node) { list.push_back(node); }

        //virtual llvm::Value *CodeGen(CodeGenContext &context);      
    };

    class ParaDecl : public Node{
    public:
        IdentifierList *id_list = nullptr;
        TypeDecl *type_decl = nullptr;

        ParaDecl(IdentifierList *idd, TypeDecl *type_) : id_list(idd), type_decl(type_) {}

        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class ParaDeclList : public Node{
    public:
        std::vector<ParaDecl *> list;

        void insert(ParaDecl *node) { list.push_back(node); }

        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class Expression : public Node{
    public:
        //virtual llvm::Value *CodeGen(CodeGenContext& context) = 0;
    };

    class ExpressionList : public Expression
    {
    public:
        std::vector<Expression *> list;

        void insert(Expression *node) { list.push_back(node); }

        std::vector<Expression *> *getListPtr() { return &list; }

        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class TypeDecl : public Node{
    public:
        enum class TypeName {
            error,
            integer,
            real,
            charactor,
            boolean,
            range,
            array,
            record,
            user_defined
        };
        std::string raw_name = "";
        TypeName sys_name = TypeName::error;
        RangeType *range_type = nullptr;
        ArrayType *array_type = nullptr;
        RecordType *record_type = nullptr;

        TypeDecl(ArrayType *atp) : array_type(atp) { init(); }
        TypeDecl(RecordType *rtp) : record_type(rtp) { init(); }
        TypeDecl(RangeType *rtp) :range_type(rtp) { init(); }
        TypeDecl(TypeName tpname) : sys_name(tpname) { init(); }
        TypeDecl(const std::string &str) : raw_name(str) { init(); }
        TypeDecl(const char * ptr) : raw_name(*(new std::string(ptr))) { init(); }

        void init() { 
            // std::cout << this << std::endl;
            if (sys_name != TypeName::error)
                return;
            if (raw_name == "integer")          sys_name = TypeName::integer;
            else if (raw_name == "real")        sys_name = TypeName::real;
            else if (raw_name == "char")        sys_name = TypeName::charactor;
            else if (raw_name == "boolean")     sys_name = TypeName::boolean;
            else if (raw_name.size() > 0)       sys_name = TypeName::user_defined;
            else if (range_type)                sys_name = TypeName::range;
            else if (array_type)                sys_name = TypeName::array;
            else if (record_type)               sys_name = TypeName::record;
            else                                throw std::logic_error("Unimplemented type");
        } 

        //llvm::Type *toLLVMType();
        //virtual llvm::Value* CodeGen(CodeGenContext& context);
    }; 

    class ArrayType : public Statement{
    public:
        TypeDecl *subscript = nullptr;
        TypeDecl *array_type = nullptr;

        ArrayType(TypeDecl *ss, TypeDecl *at) : subscript(ss), array_type(at)
        {
            this->_type = ARRAY_TYPE_DECL;
        }
        //virtual llvm::Value* CodeGen(CodeGenContext& context);
    };

    /* class RecordBody : public Statement{
    public:
        Identitfier *first = nullptr;
        TypeDecl *second = nullptr;
        RecordBody(Identifier *one, TypeDecl *two) : first(one), second(two) {}
        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    }; */

    class RecordType : public Statement{
    public:
        VarDeclList *list;
         
        RecordType(VarDeclList *ls) : list(ls)
        {
            this->_type = RECORD_TYPE_DECL;
        }
        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class Identifier : public Expression{
    public:
        std::string name;

        Identifier(const std::string &na) : name(na) {}
        Identifier(const char *str) : name(*(new std::string(str))) {}

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {} 
    };

    class IdentifierList : public Node{
    public:
        std::vector<std::string> list;

        void insert(const char *node)
        {
            std::string str = node;
            list.push_back(str);
        }

        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class ArrayRef : public Expression{
    public:
        Identifier *array = nullptr;
        Expression *index = nullptr;

        ArrayRef(Identifier *ar, Expression *in) : array(ar), index(in) {}

        //llvm::Value* getRef(CodeGenContext& context);
        //virtual llvm::Value* CodeGen(CodeGenContext& context);
    };

    class RecordRef : public Expression{
    public:
        Identifier *record = nullptr;
        Identifier *body = nullptr;

        RecordRef(Identifier *re, Identifier *bo) : record(re), body(bo) {}

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class ConstValue : public Expression{
    public:
        virtual TypeDecl::TypeName getConstType() = 0;
        virtual int toRange() = 0;
        virtual bool notRange() { return false; }

        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class TypeDefine : public Statement{
    public:
        Identifier *name = nullptr;
        TypeDecl *type_decl = nullptr;

        TypeDefine() { this->_type = TYPE_DEFINE; }
        TypeDefine(Identifier *na, TypeDecl *td) : name(na), type_decl(td) { this->_type = TYPE_DEFINE; }

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class TypeDefineList : public TypeDefine{
    public:
        std::vector<TypeDefine *> type_define_list;

        TypeDefineList() { this->_type = TYPE_DEFINE_LIST; }
        TypeDefineList(std::vector<TypeDefine *> &list) : type_define_list(list) 
        {
            this->_type = TYPE_DEFINE_LIST;
        }
        TypeDefineList(TypeDefine *node) 
        {
            this->_type = TYPE_DEFINE_LIST;
            this->type_define_list.push_back(node);
        }

        //virtual llvm::Value *CodeGen(CodeGenContext &context) {};
    };

    class ConstDecl : public Statement{
    public:
        Identifier *name = nullptr;
        ConstValue *val = nullptr;
        TypeDecl *type = nullptr;

        ConstDecl() { this->_type = CONST_DECL; }
        ConstDecl(Identifier *na, ConstValue *cv) : name(na), val(cv), type(new TypeDecl(val->getConstType())) {}

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class ConstDeclList : public Statement{
    public:
        std::vector<ConstDecl *> const_decl_list;

        ConstDeclList() { this->_type = CONST_DECL_LIST; }
        ConstDeclList(ConstDecl *node)
        {
            this->_type = CONST_DECL_LIST;
            this->const_decl_list.push_back(node);
        }
        ConstDeclList(std::vector<ConstDecl *> &list)
        {
            this->_type = CONST_DECL_LIST;
            const_decl_list = list;
        }
        void insert(ConstDecl *node) { this->const_decl_list.push_back(node); }

        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class VarDecl : public Statement{
    public:
        IdentifierList *name = nullptr;
        TypeDecl *type = nullptr;
        bool isGlobal;

        VarDecl(IdentifierList *na, TypeDecl *ty) : name(na), type(ty) {}

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class VarDeclList : public Statement{
    public: 
        std::vector<VarDecl *> list;

        void insert(VarDecl *node) { list.push_back(node); }

        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };

    class IntegerType : public ConstValue{
    public:
        int value;

        IntegerType(int val) : value(val) {}
        virtual TypeDecl::TypeName getConstType() { return TypeDecl::TypeName::integer; }
        virtual int toRange() {return value; }

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class RealType : public ConstValue{
    public:
        double value;

        RealType(double val) : value(val) {}
        virtual TypeDecl::TypeName getConstType() { return TypeDecl::TypeName::real; }
        virtual int toRange() { return 0; }
        virtual bool notRange() { return true; }

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class CharType : public ConstValue{
    public:
        char value;

        CharType(const char *ptr) : value(*(ptr)) {}
        virtual TypeDecl::TypeName getConstType() { return TypeDecl::TypeName::charactor; }
        virtual int toRange() { return int(value); }

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class BooleanType : public ConstValue{
    public:
        int value;

        BooleanType(const char *ptr) : value(std::string(ptr) == "true" ? 1 : 0) {}
        virtual TypeDecl::TypeName getConstType() { return TypeDecl::TypeName::boolean; }
        virtual int toRange() { return value; }

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class RangeType : public Expression{
    public:
        int low, high;
        std::string low_s, high_s;
        bool isNameRange = false;

        RangeType(int l, int h) : low(l), high(h) {}
        RangeType(std::string ls, std::string hs) : low_s(ls), high_s(hs), isNameRange(true) {}
        size_t size() { return high - low + 1; }

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class FuncCall : public Expression , public Statement{
    public:
        Identifier *id = nullptr;
        ExpressionList *argument = nullptr;

        FuncCall(Identifier *idd) : id(idd), argument(nullptr) {}
        FuncCall(Identifier *idd, ExpressionList *list) : id(idd), argument(list) {}

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class ProcCall : public Statement{
    public:
        Identifier *id = nullptr;
        ExpressionList *argument = nullptr;

        ProcCall(Identifier *idd) : id(idd), argument(new ExpressionList) {}
        ProcCall(Identifier *idd, ExpressionList *list) : id(idd), argument(list) {}

        //virtual llvm::Value* CodeGen(CodeGenContext& context) {}
    };

    class SysProcCall : public ProcCall{
    public:
        SysProcCall(Identifier *idd) : ProcCall(idd) {}
        SysProcCall(Identifier *idd, ExpressionList *al) : ProcCall(idd,al) {}

        //llvm::Value* SysProc_write(CodeGenContext& context, bool writeln);
        //virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class BinaryOperator : public Expression{
    public:
        enum class OpType : int{
            plus,
            minus,
            mul,
            div,
            mod,
            bit_and,
            bit_or,
            bit_xor,
            eq,
            ne,
            lt,
            gt,
            le,
            ge
        };

        Expression *op1, *op2;
        OpType op;

        BinaryOperator(Expression *op1, OpType op, Expression *op2) : op1(op1), op(op), op2(op2) {}

        //virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class AssignmentStmt : public Statement{
    public:
        Identifier *lhs = nullptr;
        Expression *rhs = nullptr;
        bool complex_assign = false;

        AssignmentStmt(Identifier *lh, Expression *rh) : lhs(lh), rhs(rh) {}
        AssignmentStmt(ArrayRef *lh, Expression *rh) : lhs((Identifier*)lh), rhs(rh), complex_assign(true) {}
        AssignmentStmt(RecordRef *lh, Expression *rh) : lhs((Identifier*)lh), rhs(rh), complex_assign(true) {}
        
        std::string getlhsName()
        {
            return complex_assign ? ((AST::ArrayRef*) this->lhs)->array->name : this->lhs->name;
        }

        //virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class IfStmt : public Statement{
    public:
        Expression *condition = nullptr;
        Statement *thenstmt = nullptr;
        Statement *elsestmt = nullptr;

        IfStmt(Expression *c, Statement *t, Statement *e) : condition(c), thenstmt(t), elsestmt(e) {}

        //virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class WhileStmt : public Statement{
    public:
        Expression *condition = nullptr;
        Statement *loopstmt = nullptr;

        WhileStmt(Expression *condition, Statement *loopStmt)
            : condition(condition), loopstmt(loopStmt) {}


        //virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class RepeatStmt : public Statement{
    public:
        Expression *condition = nullptr;
        Statement *loopstmt = nullptr;

        RepeatStmt(Expression *c, Statement *l) : condition(c), loopstmt(l) {}

        //virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class ForStmt : public Statement{
    public:
        Identifier *loopvar = nullptr;
        Expression *startexp = nullptr;
        Expression *endexp = nullptr;
        int direction;
        Statement *loopstmt = nullptr;

        ForStmt(Identifier *id, Expression *start, Expression *end, int direc, Statement *loop) :
            loopvar(id), startexp(start), endexp(end), direction(direc), loopstmt(loop) {}
        
        //virtual llvm::Value *CodeGen(CodeGenContext& context);
    };

    class CaseStmt : public Statement{
    public:
        Expression* condition;
        Statement* thenStmt;

        CaseStmt(Expression* condition,Statement* thenStmt):condition(condition),thenStmt(thenStmt){}
        
        //virtual llvm::Value *CodeGen(CodeGenContext &context,llvm::SwitchInst* sw, llvm::BasicBlock* exitBB, llvm::Type* ty);
        //virtual llvm::Value* CodeGen(CodeGenContext& context) {};
    };

    class SwitchStmt : public Statement{
    public:
        Expression *condition;
        std::vector<CaseStmt *> list;

        void insert(CaseStmt *node) { list.push_back(node);}
        SwitchStmt() { condition = nullptr; }
        SwitchStmt(Expression *c, std::vector<CaseStmt *> &l)
            : condition(c), list(l) {}
        
        //virtual llvm::Value *CodeGen(CodeGenContext &context);
    };
} 

#endif