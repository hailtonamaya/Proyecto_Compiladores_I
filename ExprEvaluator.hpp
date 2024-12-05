#pragma once 

#include <string>
#include <unordered_map>
#include "Ast.hpp"

class ExprEvaluator
{
    public:
        ExprEvaluator() = default;

        std::string evaluate(AstNode *node);
        void programNode(ProgramNode *node);
        void variableDeclNode(VariableDeclNode *node);
        void methodDeclNode(MethodDeclNode *node);

    private:
        std::unordered_map<std::string, int> variables;
};