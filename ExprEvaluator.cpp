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
        case NodeKind::Constant:
            return std::to_string(constant(static_cast<Constant *>(node)));
            break;
        case NodeKind::ParamDeclNode:
            paramDeclNode(static_cast<ParamDeclNode *>(node), "methodId");
            break;
        case NodeKind::AssignStmt:
            assignStmtNode(static_cast<AssignStmt *>(node));
            break;
        case NodeKind::ReturnStmt:
            return std::to_string(returnStmtNode(static_cast<ReturnStmt *>(node)));
            break;
        case NodeKind::IfStmt:
            ifStmtNode(static_cast<IfStmt *>(node));
            break;
        case NodeKind::WhileStmt:
            whileStmtNode(static_cast<WhileStmt *>(node));
            break;
        case NodeKind::CallStmt:
            callStmtNode(static_cast<CallStmt *>(node));
            break;
        case NodeKind::TypeNode:
            return typeNode(static_cast<TypeNode *>(node));
            break;
        case NodeKind::MethodType:
            return methodTypeNode(static_cast<MethodType *>(node));
            break;
        case NodeKind::PrintStmt: 
            return printStmt(static_cast<PrintStmt *>(node));
            break;
        case NodeKind::ReadStmt:
             readStmtNode(static_cast<ReadStmt *>(node));
            break;
        case NodeKind::BooleanExpr:
            return booleanExpr(static_cast<BooleanExpr *>(node));
            break;
        case NodeKind::BooleanTerm:
            return booleanTerm(static_cast<BooleanTerm *>(node));
            break;
        case NodeKind::BooleanFactor:
            return booleanFactor(static_cast<BooleanFactor *>(node));
            break;
        case NodeKind::RelationalExpr:
            return relationalExpr(static_cast<RelationalExpr *>(node));
            break;
        case NodeKind::RelationalEqualExpr:
            return std::to_string(relationalEqualExpr(static_cast<RelationalEqualExpr *>(node)));
            break;
        case NodeKind::RelationalNotEqualExpr:
            return std::to_string(relationalNotEqualExpr(static_cast<RelationalNotEqualExpr *>(node)));
            break;
        case NodeKind::RelationalLessThanExpr:
            return std::to_string(relationalLessThanExpr(static_cast<RelationalLessThanExpr *>(node)));
            break;
        case NodeKind::RelationalLessEqualExpr:
            return std::to_string(relationalLessEqualExpr(static_cast<RelationalLessEqualExpr *>(node)));
            break; 
        case NodeKind::RelationalGreaterThanExpr:
            return relationalGreaterThanExpr(static_cast<RelationalGreaterThanExpr *>(node));
            break;  
        case NodeKind::RelationalGreaterEqualExpr:
            return std::to_string(relationalGreaterEqualExpr(static_cast<RelationalGreaterEqualExpr *>(node)));
            break;  
        case NodeKind::ArithmeticExpr:
            return arithmeticExpr(static_cast<ArithmeticExpr *>(node));
            break;  
        case NodeKind::ArithAddExpr:
            return std::to_string(arithAddExpr(static_cast<ArithAddExpr *>(node)));
            break;
        case NodeKind::ArithSubExpr:
            return std::to_string(arithSubExpr(static_cast<ArithSubExpr *>(node)));
            break;
        case NodeKind::Term:
            return term(static_cast<Term *>(node));
            break;
        case NodeKind::MulTerm:
            return std::to_string(mulTerm(static_cast<MulTerm *>(node)));
            break;
        case NodeKind::DivTerm:
            return std::to_string(divTerm(static_cast<DivTerm *>(node)));
            break;
        case NodeKind::ModTerm:
            return std::to_string(modTerm(static_cast<ModTerm *>(node)));
            break;
        case NodeKind::Factor:
            return factor(static_cast<Factor *>(node));
            break;
        case NodeKind::Primary:
            return primaryNode(static_cast<PrimaryNode *>(node));
            break;
        case NodeKind::PrimaryConst:
            return std::to_string(primaryConst(static_cast<PrimaryConst *>(node)));
            break;
        case NodeKind::PrimaryArray:
             primaryArray(static_cast<PrimaryArray *>(node));
            break;
        case NodeKind::PrimaryFuncCall:
             primaryFuncCall(static_cast<PrimaryFuncCall *>(node));
            break;   
        case NodeKind::ExprNode:
            std::cout << "ExprNode\n";
            break; 
        case NodeKind::PrimaryIdentifier:
            return primaryIdentifier(static_cast<PrimaryIdentifier *>(node));
            break;
        default:
            throw std::runtime_error("Unknown node type '" + node->toString() + "'\n");
            break;
    }
    return "";
}

bool ExprEvaluator::variableExists(std::string id)
{
    std::cout << "VariableExists\n";
    return variables.find(id) != variables.end();
}   

bool ExprEvaluator::methodExists(std::string id)
{
    //Informacion del vector de parametros
    std::cout << "MethodExists\n";
    for(ParamsFromMethod param : paramsFromMethod)
    {
        if(param.methodId == id)
        {
            return true;
        }
    }
    return false;
}

void ExprEvaluator::programNode(ProgramNode *node)
{
    std::cout << "ProgramNode\n";
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
    std::cout << "VariableDeclNode\n";
    for (std::string id : node->ids)
    {
        if(variableExists(id))
        {
            throw std::runtime_error("Variable '" + id + "' already exists\n");
            break;
        }else{
            variables[id] = 0;
        }
    }
}

int ExprEvaluator::constant(Constant *node)
{
    std::cout << "Constant\n";
    std::string value = node->value;
    if(value[0] == '0' && value[1] == 'x')
    {
        return std::stoi(value, nullptr, 16);
    }else if(value[0] == '0' && value[1] == 'b')
    {
        return std::stoi(value, nullptr, 2);
    }else{
        return std::stoi(value);
    }
}

void ExprEvaluator::methodDeclNode(MethodDeclNode *node)
{
    std::cout << "MethodDeclNode\n";
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

void ExprEvaluator::paramDeclNode(ParamDeclNode *node, std::string methodId)
{
    std::cout << "ParamDeclNode\n";
    if(methodExists(methodId))
    {
        throw std::runtime_error("Method '" + methodId + "' already exists\n");
    }else{
        ParamsFromMethod param;
        param.methodId = methodId;
        param.isRef = node->isReference;
        param.id = node->name;
        param.type = node->type->toString();
        paramsFromMethod.push_back(param);
    }
}

void ExprEvaluator::assignStmtNode(AssignStmt *node)
{
    std::cout << "AssignStmtNode\n";
    std::string id = node->identifier;
    if(variableExists(id))
    {
        if(node->indexExpr != nullptr)
        {
            std::string index = evaluate(node->indexExpr);
            if(variableExists("identifier[" + index+ "]"))
            {
                variables[id + "[" + index + "]"] = std::stoi(node->value->toString());
            }else{
                throw std::runtime_error("Variable '" + index + "' does not exist\n");
            }
        }else{
            if(variableExists(id))
            {
                int value = std::stoi(evaluate(node->value));
                variables[id] = value;
            }else{
                throw std::runtime_error("Variable '" + id + "' does not exist\n");
            }
        }
    }else{
        throw std::runtime_error("Variable '" + id + "' does not exist\n");
    }
}

int ExprEvaluator::returnStmtNode(ReturnStmt *node)
{
    std::cout << "ReturnStmtNode\n";
    return std::stoi(evaluate(node->expr));
}

void ExprEvaluator::ifStmtNode(IfStmt *node)
{
    std::cout << "IfStmtNode\n";
    if(evaluate(node->expr) == "true")
    {
        for(AstNode *stmt : node->stmts)
        {
            evaluate(stmt);
        }
    }else{
        for(AstNode *stmt : node->elseStmts)
        {
            evaluate(stmt);
        }
    }
}

void ExprEvaluator::whileStmtNode(WhileStmt *node)
{
    std::cout << "WhileStmtNode\n";
    while(evaluate(node->expr) == "True")
    {
        std::cout << "CANTIDADES DE WHILESSSS" << std::endl;
        for(AstNode *stmt : node->stmts)
        {
            //std::cout << std::to_string(static_cast<int>(stmt->kind())) << std::endl;
            evaluate(stmt);
        }
    }
}

void ExprEvaluator::callStmtNode(CallStmt *node)
{
    std::cout << "CallStmtNode\n";
    std::string id = node->id;
    if(methodExists(id))
    {
        for(AstNode *arg : node->args)
        {
            evaluate(arg);
        }
    }else{
        throw std::runtime_error("Method '" + id + "' does not exist\n");
    }
}

std::string ExprEvaluator::typeNode(TypeNode *node)
{   
    std::cout << "TypeNode\n";
    return node->type;
}

std::string ExprEvaluator::methodTypeNode(MethodType *node)
{
    std::cout << "MethodTypeNode\n";
    return node->type;
}

std::string ExprEvaluator::printStmt(PrintStmt *node)
{
    std::cout << "Print\n";
    if(node->printExpr != nullptr)
    {
        //std::cout << std::to_string(static_cast<int>(node->printExpr->kind())) << std::endl;
        std::cout << evaluate(node->printExpr);
        //return evaluate(node->printExpr);
    }else{
        std::cout << node->printStr;
        //return node->printStr;
    }
    return "";
}

void ExprEvaluator::readStmtNode(ReadStmt *node)
{
    std::cout << "ReadStmt\n";
    std::string id = node->id;
    if(variableExists(id))
    {
        std::cin >> variables[id];
    }else{
        throw std::runtime_error("Variable '" + id + "' does not exist\n");
    }
}

std::string ExprEvaluator::booleanExpr(BooleanExpr *node)
{
    std::cout << "BooleanExpr\n";
    for(AstNode *term : node->terms)
    {  
       return evaluate(term);
    }
}

std::string ExprEvaluator::booleanTerm(BooleanTerm *node)
{
    std::cout << "BooleanTerm\n";
    for(AstNode *factor : node->factors)
    {
        //std::cout << "BooleanFactor\n";
        //std::cout << std::to_string(static_cast<int>(factor->kind())) << std::endl;
        return evaluate(factor);
    }
}

std::string ExprEvaluator::booleanFactor(BooleanFactor *node)
{
    std::cout << "BooleanFactor\n";
    return node->factor->toString();
}

std::string ExprEvaluator::relationalExpr(RelationalExpr *node)
{
    std::cout << "RelationalExpr\n";
    return evaluate(node->ariths);
}

bool ExprEvaluator::relationalEqualExpr(RelationalEqualExpr *node)
{
    return evaluate(node->leftArith) == evaluate(node->rightArith);
}

bool ExprEvaluator::relationalNotEqualExpr(RelationalNotEqualExpr *node)
{
    return evaluate(node->leftArith) != evaluate(node->rightArith);
}

bool ExprEvaluator::relationalLessThanExpr(RelationalLessThanExpr *node)
{
    return evaluate(node->leftArith) < evaluate(node->rightArith);
}

bool ExprEvaluator::relationalLessEqualExpr(RelationalLessEqualExpr *node)
{
    return evaluate(node->leftArith) <= evaluate(node->rightArith);
}

std::string ExprEvaluator::relationalGreaterThanExpr(RelationalGreaterThanExpr *node)
{
    std::cout << "RelationalGreaterThanExpr\n" << node->leftArith->toString() << std::endl;
    bool value =  std::stoi(evaluate(node->leftArith)) > std::stoi(evaluate(node->rightArith));
    if(value)
    {
        std::cout << std::stoi(evaluate(node->leftArith)) << " > " << std::stoi(evaluate(node->rightArith)) << std::endl;
        return "True";
    }
    return "False";
}

bool ExprEvaluator::relationalGreaterEqualExpr(RelationalGreaterEqualExpr *node)
{
    return evaluate(node->leftArith) >= evaluate(node->rightArith);
}

std::string ExprEvaluator::arithmeticExpr(ArithmeticExpr *node)
{   
    std::cout << "ArithmeticExpr\n";
    for(AstNode *term : node->terms)
    {
        //std::cout << std::to_string(static_cast<int>(term->kind())) << std::endl;
        return evaluate(term);
    }
}

int ExprEvaluator::arithAddExpr(ArithAddExpr *node)
{
    std::cout << "ArithAddExpr\n";
    
    return std::stoi(evaluate(node->t1)) + std::stoi(evaluate(node->t2));
}

int ExprEvaluator::arithSubExpr(ArithSubExpr *node)
{
    std::cout << "ArithSubExpr\n";
    return std::stoi(evaluate(node->t1)) - std::stoi(evaluate(node->t2));
}

std::string ExprEvaluator::term(Term *node)
{
    std::cout << "Term\n";
    for(AstNode *factor : node->factors)
    {
        return evaluate(factor);
    }

}

int ExprEvaluator::mulTerm(MulTerm *node)
{   
    std::cout << "MulTerm\n";
    return std::stoi(evaluate(node->f1)) * std::stoi(evaluate(node->f2));
}

int ExprEvaluator::divTerm(DivTerm *node)
{
    std::cout << "DivTerm\n";
    return std::stoi(evaluate(node->f1)) / std::stoi(evaluate(node->f2));
}

int ExprEvaluator::modTerm(ModTerm *node)
{
    std::cout << "ModTerm\n";
    return std::stoi(evaluate(node->f1)) % std::stoi(evaluate(node->f2));
}

std::string ExprEvaluator::factor(Factor *node)
{   
    std::cout << "Factor\n";
    return evaluate(node->primary);
}

std::string ExprEvaluator::primaryNode(PrimaryNode *node)
{       
    std::cout << "PrimaryNode\n";
    std::string name = evaluate(node->args);
    return name;
}

int ExprEvaluator::primaryConst(PrimaryConst *node)
{
    std::cout << "PrimaryConst\n";
    return std::stoi(node->value->toString());
}

void ExprEvaluator::primaryArray(PrimaryArray *node)
{
    std::cout << "PrimaryArray\n";
    if(variableExists(node->identifier))
    {
        std::string index = evaluate(node->indexExpr);
        if(variableExists(node->identifier + "[" + index + "]"))
        {
            evaluate(node->indexExpr);
        }else{
            throw std::runtime_error("Variable '" + index + "' does not exist\n");
        }
    }else{
        throw std::runtime_error("Variable '" + node->identifier + "' does not exist\n");
    }
}

void ExprEvaluator::primaryFuncCall(PrimaryFuncCall *node)
{
    std::cout << "PrimaryFuncCall\n";
    std::string id = node->identifier;
    if(methodExists(id))
    {
        for(AstNode *arg : node->args)
        {
            evaluate(arg);
        }
    }else{
        throw std::runtime_error("Method '" + id + "' does not exist\n");
    }
}

std::string ExprEvaluator::primaryIdentifier(PrimaryIdentifier *node)
{
    std::cout << "PrimaryIdentifier\n";
    std::string id = node->identifier;
    if(variableExists(id))
    {
        std::cout << "Variable '" + id + std::to_string(variables[id]) + "' exists\n";
        return std::to_string(variables[id]);
    }else{
        throw std::runtime_error("Variable '" + id + "' does not exist\n");
    }
}