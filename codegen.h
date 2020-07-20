#ifndef CODEGEN_H
#define CODEGEN_H

#include <map>
#include <stack>
#include <vector>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/ValueSymbolTable.h>

class GlobalLLVMContext{
private:
    GlobalLLVMContext() {};
public:
    static llvm::LLVMContext &getGlobalContext()
    {
        static llvm::LLVMContext instance;
        return instance;
    }
    GlobalLLVMContext(GlobalLLVMContext const&) = delete;
    void operator = (GlobalLLVMContext const&) = delete;
};

class CodeGenBlock{
public:
    llvm::BasicBlock *block;
    llvm::Value *returnValue;
    CodeGenBlock *parent;
    std::map<std::string, llvm::Value*> locals;
};

class CodeGenContext{
private:
    std::stack<CodeGenBlock*> blocks;
public:
    llvm::Module *module;
    llvm::Function *curFunc;
    llvm::Function *mainFunc;
    
}

#endif // !CODEGEN_H
