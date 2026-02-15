#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace pallas::frontend {

    class ExprAST {
    public:
        virtual ~ExprAST() = default;
    };

    class NumberExprAST : public ExprAST {
        double val;
        public:
        NumberExprAST(double val) : val(val) {}
    };
    class VariableExprAST : public ExprAST {
        std::string name;
        public:
        VariableExprAST(const std::string& name) : name(name) {}
    };
    class BinaryExprAST : public ExprAST {
      char Op;
      std::unique_ptr<ExprAST> LHS, RHS;

    public:
      BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
          : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    };

    class CallExprAST : public ExprAST {
        std::string callee;
        std::vector<std::unique_ptr<ExprAST>> args;
        public:
        CallExprAST(const std::string& callee, std::vector<std::unique_ptr<ExprAST>> args)
              : callee(callee), args(std::move(args)) {}
    };
    class PrototypeAST {
        std::string name;
        std::vector<std::string> args;
        public:
        PrototypeAST(const std::string& name, std::vector<std::string> args)
            : name(name), args(std::move(args)) {}
        const std::string& getName() const {return name;}
    };
    class FunctionAST {
        std::unique_ptr<PrototypeAST> proto;
        std::unique_ptr<ExprAST> body;
        public:
        FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body)
            : proto(std::move(proto)), body(std::move(body)) {}
    };



enum class TypeKind {
    TYPE_VOID,
    TYPE_BOOL,
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_I128,
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_U128,
    TYPE_F32,
    TYPE_F64,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_POINTER,
    TYPE_ARRAY,
    TYPE_STRUCT,
    TYPE_CLASS,
    TYPE_FUNCTION,
    TYPE_UNKNOWN,
};

struct Type {
  TypeKind kind;
  std::string name;
};


enum class NodeType {
    NODE_ROOT,

    NODE_FUNCTION,
    NODE_BLOCK,
    NODE_RETURN,
    NODE_CONTINUE,
    NODE_BREAK,
    NODE_VAR_DECL,
    NODE_CONST,
};
}  // namespace pallas::frontend
