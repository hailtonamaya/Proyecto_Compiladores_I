#include <stdexcept>
#include <iostream>
#include "ExprEvaluator.hpp"

std::string ExprEvaluator::evaluate(AstNode *node)
{
    switch (node->kind())
    {
        case NodeKind::ProgramNode:
            programNode(static_cast<ProgramNode *>(node));            
            break;
        case NodeKind::VariableDeclNode:
            variableDeclNode(static_cast<VariableDeclNode *>(node));
            break;
        case NodeKind::MethodDeclNode:
            methodDeclNode(static_cast<MethodDeclNode *>(node));
            break;
    
        default:
            throw std::runtime_error("Unknown node type '" + node->toString() + "'\n");
            break;
    }
    return "";
}

void ExprEvaluator::programNode(ProgramNode *node)
{
    for (AstNode *varDecl : node->variableDecls)
    {
        evaluate(varDecl);
    }

    for (AstNode *methodDecl : node->methodDecls)
    {
        evaluate(methodDecl);
    }
}

void ExprEvaluator::variableDeclNode(VariableDeclNode *node)
{
    for (std::string id : node->ids)
    {
        variables[id] = 0;
    }
}

void ExprEvaluator::methodDeclNode(MethodDeclNode *node)
{
    for(AstNode *param : node->params)
    {
        evaluate(param);
    }

    for(AstNode *varDecl : node->variableDecls)
    {
        evaluate(varDecl);
    }

    for(AstNode *stmt : node->stmts)
    {
        evaluate(stmt);
    }
}