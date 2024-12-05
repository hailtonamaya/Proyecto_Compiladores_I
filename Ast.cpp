#include "Ast.hpp"

std::string ProgramNode::toString() const
{
    std::string result;

    result += "ProgramNode(\n";
    result += "Variable Declarations\n{\n";
    for (const auto i : variableDecls) {
        result += "\t" + i->toString() + "\n" ;
    }
    result += "Method Declarations \n}\n";
    result += "{\n";
    for (const auto n : methodDecls) {
        result += "\t" + n->toString() + "\n" ;
    }
    result += "}\n";
    result += ")";

    return result;
}

std::string VariableDeclNode::toString() const
{
    std::string result;
    result += "'" + type->toString() + "' ";
    for(const auto n : ids){
        result += n + ", ";
    }
    return result;
}

std::string MethodDeclNode::toString() const
{
    std::string result;
    result += "" + rtype->toString() + " " + name + '\n';
    result += "\tParams: ";
    for(const auto n:params){
        result += n->toString() + ", ";
    }
    result += '\n';
    result += "\tVariableDecls: ";
    for (const auto i : variableDecls) {
        result += " " + i->toString();
    }
    result += '\n';

    result+= "\tStatements:\n ";
    for (const auto i : stmts) {
        result += "\t\t " + i->toString() + "\n";
    }
    result += '\n';
    return result;
}

std::string AssignStmt::toString() const
{
    std::string result;
    if(indexExpr != nullptr){
        result += identifier + "[" + indexExpr->toString() + "] =" + value->toString();
    }else{
        result += identifier + " = " + value->toString();
    }
    return result;
}