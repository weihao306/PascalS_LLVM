#include "ast.h"
#include "codegen.h"

llvm::Value *AST::Program::CodeGen(CodeGenContext& context)
{
    std::vector<llvm::Type *>args;
    llvm::FunctionType *fty = llvm::FunctionType::Get(
        llvm::Type::getVoidTy(GlobalLLVMComtext::getGlobalContext()),
        llvm::makeArrayRef(args),false);
    context.mainFunc = llvm::Function::Create(fty, llvm::GlobalValue::LinkageTypes::ExternalLinkage,
        "main", context.module);
    llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "entry", context.mainFunc);
    
}