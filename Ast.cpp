#include "Ast.hpp"

std::string ProgramNode::toString() const
{
    std::string result;

    result += "ProgramNode(\n";
    result += "Variable Declarations\n";
    for (const auto i : variableDecls) {
        result += "\t" + i->toString() + "\n" ;
    }
    result += "Method Declarations \n";
    for(int i = 0; i < methodDecls.size(); i++){
        result += "\tMethod "+ std::to_string(i + 1);
        result += ": " + methodDecls[i]->toString() + "\n" ;
    }
    result += ")";

    return result;
}

std::string VariableDeclNode::toString() const
{
    std::string result;
    result += "'" + type->toString() + "' ";
    for(int i = 0; i < ids.size(); i++){
        result += ids[i];
        if(i != ids.size() - 1){
            result += ", ";
        }
    }
    return result;
}

std::string MethodDeclNode::toString() const
{
    std::string result;
    result += "" + rtype->toString() + " " + name + '\n';
    result += "\tParams: ";
    for(int i = 0; i < params.size(); i++){
        result += params[i]->toString();
        if(i != params.size() - 1){
            result += ", ";
        }
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

std::string WhileStmt::toString() const
{
    std::string result;
    result += "WhileStmt( 'Condition' : " + expr->toString() + ")\n";
    for (const auto i : stmts) {
        result += "\t\t\t" + i->toString() + "\n" ;
    }
    result += "\n";
    return result;
}

std::string CallStmt::toString() const
{
    std::string result;
    result += "CallStmt( 'Function' : " + id + "(";
    for(int i = 0; i < args.size(); i++){
        result += args[i]->toString();
        if(i != args.size() - 1){
            result += ", ";
        }
    }
    result += "))\n";
    return result;
}

std::string ReturnStmt::toString() const
{
    std::string result;
    result += "ReturnStmt( 'Return' : " + expr->toString() + ")\n";
    return result;
}

std::string IfStmt::toString() const
{
    std::string result;
    result += "IfStmt( 'Condition' : " + expr->toString() + ")\n";
    for (const auto i : stmts) {
        result += "\t\t\t" + i->toString() + "\n" ;
    }
    if(elseStmts.size() > 0){
        result += "\t\tElseStmts\n";
        for (const auto i : elseStmts) {
            result += "\t\t\t" + i->toString() + "\n" ;
        }
    }
    result += "\n";
    return result;
}

std::string PrintStmt::toString() const
{
    std::string result;
    if(printExpr != nullptr){
        result += "PrintStmt( 'Print' : " + printExpr->toString() + ")\n";
    }else{
        result += "PrintStmt( 'Print' : " + printStr + ")\n";
    }
    return result;
}

std::string ReadStmt::toString() const
{
    std::string result;
    result += "ReadStmt( 'Read' : " + id + ")\n";
    return result;
}

std::string BooleanExpr::toString() const
{
    std::string result;
    for(int i = 0; i < terms.size(); i++){
        result += terms[i]->toString();
        if(i != terms.size() - 1){
            result += " || ";
        }
    }
    return result;
}

std::string BooleanTerm::toString() const
{
    std::string result;
    for(int i = 0; i < factors.size(); i++){
        result += factors[i]->toString();
        if(i != factors.size() - 1){
            result += " && ";
        }
    }
    return result;
}

std::string BooleanFactor::toString() const
{
    std::string result;
    if(negated){
        result += "!";
    }
    result += factor->toString();
    return result;
}

std::string RelationalExpr::toString() const
{
    return ariths->toString();
}

std::string RelationalEqualExpr::toString() const
{
    return leftArith->toString() + " == " + rightArith->toString();
}

std::string RelationalNotEqualExpr::toString() const
{
    return leftArith->toString() + " != " + rightArith->toString();
}

std::string RelationalLessThanExpr::toString() const
{
    return leftArith->toString() + " < " + rightArith->toString();
}

std::string RelationalLessEqualExpr::toString() const
{
    return leftArith->toString() + " <= " + rightArith->toString();
}

std::string RelationalGreaterThanExpr::toString() const
{
    return leftArith->toString() + " > " + rightArith->toString();
}

std::string RelationalGreaterEqualExpr::toString() const
{
    return leftArith->toString() + " >= " + rightArith->toString();
}

std::string ArithmeticExpr::toString() const
{
    std::string result;
    for(int i = 0; i < terms.size(); i++){
        result += terms[i]->toString();
    }
    return result;
}

std::string ArithAddExpr::toString() const
{
    return t1->toString() + " + " + t2->toString();
}

std::string ArithSubExpr::toString() const
{
    return t1->toString() + " - " + t2->toString();
}

std::string Term::toString() const
{
    std::string result;
    for(int i = 0; i < factors.size(); i++){
        result += factors[i]->toString();
    }
    return result;
}

std::string MulTerm::toString() const
{
    return f1->toString() + " * " + f2->toString();
}

std::string DivTerm::toString() const
{
    return f1->toString() + " / " + f2->toString();
}

std::string ModTerm::toString() const
{
    return f1->toString() + " % " + f2->toString();
}

std::string Factor::toString() const
{
    std::string result;
    if(isNegative){
        result += "-";
    }
    result += primary->toString();
    return result;
}

std::string PrimaryNode::toString() const
{
    
    return args->toString();
}

std::string PrimaryConst::toString() const
{
    return value->toString();
}

std::string PrimaryArray::toString() const
{
    return identifier + "[" + indexExpr->toString() + "]";
}

// std::string PrimaryFuncCall::toString() const
// {
//     std::string result;
//     result += identifier + "(";
//     for(int i = 0; i < args.size(); i++){
//         result += args[i]->toString();
//         if(i != args.size() - 1){
//             result += ", ";
//         }
//     }
//     result += ")";
//     return result;
// }

// std::string ParamDeclNode::toString() const
// {
//     std::string result;
//     if(isReference){
//         result += "ref " + type->toString() + " " + name;
//     }else{
//         result += type->toString() + " " + name;
//     }
//     return result;
// }