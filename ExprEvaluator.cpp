#include <stdexcept>
#include <iostream>
#include "ExprEvaluator.hpp"

std::string ExprEvaluator::evaluate(AstNode *node, std::string methodId)
{   
    switch (node->kind())
    {
        case NodeKind::ProgramNode:
            programNode(static_cast<ProgramNode *>(node));            
            break;
        case NodeKind::VariableDeclNode:
            variableDeclNode(static_cast<VariableDeclNode *>(node), false);
            break;
        case NodeKind::MethodDeclNode:
            methodDeclNode(static_cast<MethodDeclNode *>(node));
            break;
        case NodeKind::Constant:
            return std::to_string(constant(static_cast<Constant *>(node)));
            break;
        case NodeKind::ParamDeclNode:
            return paramDeclNode(static_cast<ParamDeclNode *>(node), methodId);
            break;
        case NodeKind::AssignStmt:
            assignStmtNode(static_cast<AssignStmt *>(node), methodId);
            break;
        case NodeKind::ReturnStmt:
            return returnStmtNode(static_cast<ReturnStmt *>(node), methodId);
            break;
        case NodeKind::IfStmt:
            return ifStmtNode(static_cast<IfStmt *>(node), methodId);
            break;
        case NodeKind::WhileStmt:
            whileStmtNode(static_cast<WhileStmt *>(node), methodId);
            break;
        case NodeKind::CallStmt:
            callStmtNode(static_cast<CallStmt *>(node), methodId);
            break;
        case NodeKind::TypeNode:
            return typeNode(static_cast<TypeNode *>(node));
            break;
        case NodeKind::MethodType:
            return methodTypeNode(static_cast<MethodType *>(node));
            break;
        case NodeKind::PrintStmt: 
            return printStmt(static_cast<PrintStmt *>(node), methodId);
            break;
        case NodeKind::ReadStmt:
             readStmtNode(static_cast<ReadStmt *>(node), methodId);
            break;
        case NodeKind::BooleanExpr:
            return booleanExpr(static_cast<BooleanExpr *>(node), methodId);
            break;
        case NodeKind::BooleanTerm:
            return booleanTerm(static_cast<BooleanTerm *>(node), methodId);
            break;
        case NodeKind::BooleanFactor:
            return booleanFactor(static_cast<BooleanFactor *>(node), methodId);
            break;
        case NodeKind::RelationalExpr:
            return relationalExpr(static_cast<RelationalExpr *>(node), methodId);
            break;
        case NodeKind::RelationalEqualExpr:
            return relationalEqualExpr(static_cast<RelationalEqualExpr *>(node), methodId);
            break;
        case NodeKind::RelationalNotEqualExpr:
            return relationalNotEqualExpr(static_cast<RelationalNotEqualExpr *>(node), methodId);
            break;
        case NodeKind::RelationalLessThanExpr:
            return relationalLessThanExpr(static_cast<RelationalLessThanExpr *>(node), methodId);
            break;
        case NodeKind::RelationalLessEqualExpr:
            return relationalLessEqualExpr(static_cast<RelationalLessEqualExpr *>(node), methodId);
            break; 
        case NodeKind::RelationalGreaterThanExpr:
            return relationalGreaterThanExpr(static_cast<RelationalGreaterThanExpr *>(node), methodId);
            break;  
        case NodeKind::RelationalGreaterEqualExpr:
            return relationalGreaterEqualExpr(static_cast<RelationalGreaterEqualExpr *>(node), methodId);
            break;  
        case NodeKind::ArithmeticExpr:
            return arithmeticExpr(static_cast<ArithmeticExpr *>(node), methodId);
            break;  
        case NodeKind::ArithAddExpr:
            return std::to_string(arithAddExpr(static_cast<ArithAddExpr *>(node), methodId));
            break;
        case NodeKind::ArithSubExpr:
            return std::to_string(arithSubExpr(static_cast<ArithSubExpr *>(node), methodId));
            break;
        case NodeKind::Term:
            return term(static_cast<Term *>(node), methodId);
            break;
        case NodeKind::MulTerm:
            return std::to_string(mulTerm(static_cast<MulTerm *>(node), methodId));
            break;
        case NodeKind::DivTerm:
            return std::to_string(divTerm(static_cast<DivTerm *>(node), methodId));
            break;
        case NodeKind::ModTerm:
            return std::to_string(modTerm(static_cast<ModTerm *>(node), methodId));
            break;
        case NodeKind::Factor:
            return factor(static_cast<Factor *>(node), methodId);
            break;
        case NodeKind::Primary:
            return primaryNode(static_cast<PrimaryNode *>(node), methodId);
            break;
        case NodeKind::PrimaryConst:
            return std::to_string(primaryConst(static_cast<PrimaryConst *>(node), methodId));
            break;
        case NodeKind::PrimaryArray:
             primaryArray(static_cast<PrimaryArray *>(node), methodId);
            break;
        case NodeKind::PrimaryFuncCall:
            return primaryFuncCall(static_cast<PrimaryFuncCall *>(node), methodId);
            break;   
        case NodeKind::ExprNode:
            //std::cout << "ExprNode\n";
            break; 
        case NodeKind::PrimaryIdentifier:
            return primaryIdentifier(static_cast<PrimaryIdentifier *>(node), methodId);
            break;
        default:
            throw std::runtime_error("Unknown node type '" + node->toString() + "'\n");
            break;
    }
    return "";
}

bool ExprEvaluator::isInt(std::string input) {
    try {
        std::size_t pos;
        std::stoi(input, &pos); // Convierte y obtiene la posición donde termina el número
        return pos == input.length(); // Verifica que toda la cadena sea un número
    } catch (const std::invalid_argument&) {
        return false; // No se pudo convertir, no es un número
    } catch (const std::out_of_range&) {
        return false; // Está fuera del rango para un int
    }
}

int ExprEvaluator::searchVariableInMethod(std::string id, std::string methodId)
{
    if(id[0] == '0' && id[1] == 'x')
    {
        return std::stoi(id, nullptr, 16);
    }else if(id[0] == '0' && id[1] == 'b')
    {
        return std::stoi(id, nullptr, 2);
    }else if(isInt(id))
    {
        return std::stoi(id);
    }
    for(method& method : methods)
    {
        if(method.methodId == methodId)
        {
            for(auto& variable : method.variables)
            {
                if(variable.first == id)
                {
                    return variable.second;
                }
            }
        }
    }
    throw std::runtime_error("Variable '" + id + "' not found in method '" + methodId + "'");
}

bool ExprEvaluator::variableExists(std::string id)
{
    //std::cout << "VariableExists\n";
    return variables.find(id) != variables.end();
}   

bool ExprEvaluator::variableExistsInMethod(std::string id, std::string methodId)
{
    //std::cout << "VariableExistsInMethod\n";
    for(method& method : methods)
    {
        if(method.methodId == methodId)
        {
            return method.variables.find(id) != method.variables.end();
        }
    }
    return false;
}

bool ExprEvaluator::methodExists(std::string id)
{
    //Informacion del vector de parametros
    //std::cout << "MethodExists\n";
    for(method& method : methods)
    {
        if(method.methodId == id)
        {
            return true;
        }
    }
    return false;
}

std::string ExprEvaluator::runMethods(std::string methodId)
{
    //std::cout << "====RUNNING METHOD - " + methodId + "========\n";
    std::string result="";
    for(method& method : methods)
    {
        if(method.methodId == methodId)
        {
            for(AstNode *decl : method.variableDecls)
            {
                variableDeclNode(static_cast<VariableDeclNode *>(decl), true, method.methodId);
            }
            for(AstNode *stmt : method.stmts)
            {   
                result = evaluate(stmt, method.methodId); 
                //std::cout<< "result of stmt: " << result << std::endl;
                if(result != ""){
                    //std::cout << "====END METHOD - " + methodId + "======== " + result + "\n";
                    return result;
                }
            }
            //std::cout << "====END METHOD - " + methodId + "======== " + result + "\n";
            
        }
    }
    return result;
}

void ExprEvaluator::runVoidMethod(std::string methodId)
{
    
    //std::cout << "====RUNNING VOID METHOD - " + methodId + "========\n";
    bool found = false;
    for(method& method : methods)
    {
        if(method.methodId == methodId)
        {
            found = true;
            for(AstNode *decl : method.variableDecls)
            {
                variableDeclNode(static_cast<VariableDeclNode *>(decl), true, method.methodId);
            }
            for(AstNode *stmt : method.stmts)
            {
                evaluate(stmt, method.methodId);
            }
        }
    }
    if(!found)
    {
        throw std::runtime_error("Method 'main' not found\n");
    }
}

void ExprEvaluator::programNode(ProgramNode *node)
{
    //std::cout << "ProgramNode\n";
    for (AstNode *varDecl : node->variableDecls)
    {
        evaluate(varDecl);
    }

    for (AstNode *methodDecl : node->methodDecls)
    {
        evaluate(methodDecl);
    }

    runVoidMethod("main");
}

void ExprEvaluator::variableDeclNode(VariableDeclNode *node, bool isMethod, std::string methodId)
{
    //std::cout << "VariableDeclNode\n";
    for (std::string id : node->ids)
    {
        if(!variableExists(id) && !isMethod){
            variables[id] = 0;
        }else if(!variableExists(id) && !variableExistsInMethod(id, methodId) && isMethod){
            //std::cout << "Variable Declaration in Method\n";
            for(method& method : methods)
            {
                if(method.methodId == methodId)
                {
                    method.variables[id] = 0;
                    //std::cout << "Variable '" + id + "' declared in method '" + methodId + "'\n";
                }
            }
        }else{
            throw std::runtime_error("Variable '" + id + "' already exists\n");
            break;
        }
    }
}

int ExprEvaluator::constant(Constant *node)
{
    //std::cout << "Constant\n";
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
    //std::cout << "MethodDeclNode\n";
    std::string methodId = node->name;
    if(methodExists(methodId))
    {
        throw std::runtime_error("Method '" + methodId + "' already exists\n");
    }else{
        method method;
        method.methodId = methodId;
        method.returnType = node->rtype;
        method.paramTypes = node->params;
        method.variableDecls = node->variableDecls;
        method.stmts = node->stmts;
        methods.push_back(method);
    }
}

std::string ExprEvaluator::paramDeclNode(ParamDeclNode *node, std::string methodId)
{
    //std::cout << "ParamDeclNode\n";
    return node->toString();
}

void ExprEvaluator::assignStmtNode(AssignStmt *node, std::string methodId){
    //std::cout << "AssignStmtNode\n";
    std::string id = node->identifier;
    if(methodId == ""){
        if(variableExists(id))
        {
            if(node->indexExpr != nullptr)
            {
                std::string index = evaluate(node->indexExpr);
                if(variableExists(id + "[" + index+ "]"))
                {
                    variables[id + "[" + index + "]"] = std::stoi(node->value->toString());
                }else{
                    throw std::runtime_error("Variable '" + index + "' does not exist in method " + methodId + "\n");
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
    }else{
        for(method& method : methods)
        {
            if(method.methodId == methodId)
            {
                //std::cout << "AssignStmtNode in Method" + methodId + id + "\n";
                if(variableExists(id))
                {
                    if(node->indexExpr != nullptr)
                    {
                        std::string index = evaluate(node->indexExpr);
                        if(variableExists(id + "[" + index+ "]"))
                        {
                            variables[id + "[" + index + "]"] = std::stoi(evaluate(node->value, methodId));
                        }else{
                            throw std::runtime_error("Variable '" + index + "' does not exist\n");
                        }
                    }else{
                        if(variableExists(id))
                        {
                            int value = std::stoi(evaluate(node->value, methodId));
                            method.variables[id] = value;
                        }else{
                            throw std::runtime_error("Variable '" + id + "' does not exist\n");
                        }
                    }
                }else if(variableExistsInMethod(id, methodId))
                {   
                    if(node->indexExpr != nullptr)
                    {
                        std::string index = evaluate(node->indexExpr);
                        if(variableExistsInMethod(id + "[" + index+ "]", methodId))
                        {
                            method.variables[id + "[" + index + "]"] = std::stoi(evaluate(node->value, methodId));
                        }else{
                            throw std::runtime_error("Variable '" + index + "' does not exist\n");
                        }
                    }else{
                        if(variableExistsInMethod(id, methodId))
                        {
                            int value = std::stoi(evaluate(node->value, methodId));
                            method.variables[id] = value;
                        }else{
                            throw std::runtime_error("Variable '" + id + "' does not exist\n");
                        }
                    }
                }else{
                    throw std::runtime_error("Variable '" + id + "' does not exist in main class and in method " + methodId + "\n");
                }
            }
        }
    }
}
//TODO:
std::string ExprEvaluator::returnStmtNode(ReturnStmt *node, std::string methodId)
{
    //std::cout << "ReturnStmtNode in method " + methodId + "\n";
    std::string result = evaluate(node->expr, methodId);
    //std::cout << "ReturnStmtNode Final in method " + methodId + " with result: " + result+ "\n";
    return result;
}
//TODO:
std::string ExprEvaluator::ifStmtNode(IfStmt *node, std::string methodId)
{
    //std::cout << "IfStmtNode - " + methodId + "\n";
    std::string result;
    if(evaluate(node->expr, methodId) == "True")
    {
        for(AstNode *stmt : node->stmts)
        {
            result = evaluate(stmt, methodId);
            if(result != ""){
                //std::cout << "IfStmtNode Final - " + methodId + " with result: " + result+ "\n";
                return result;
            }
        }
    }else if(!node->elseStmts.empty()){
        for(AstNode *stmt : node->elseStmts)
        {
             result = evaluate(stmt, methodId);
            if(result != ""){return result;}
        }
    }
    //std::cout << "IfStmtNode Final - " + methodId + " with result vacio\n";
    return "";
}
void ExprEvaluator::whileStmtNode(WhileStmt *node, std::string methodId)
{
    //std::cout << "WhileStmtNode\n";
    while(evaluate(node->expr, methodId) == "True")
    {
        ////std::cout << "CANTIDADES DE WHILESSSS" << std::endl;
        for(AstNode *stmt : node->stmts)
        {
            ////std::cout << std::to_string(static_cast<int>(stmt->kind())) << std::endl;
            evaluate(stmt, methodId);
        }
    }
}

std::string ExprEvaluator::callStmtNode(CallStmt *node, std::string methodId)
{
    //std::cout << "CallStmtNode\n";
    method* methodFunc = new method();
    methodFunc->methodId = "";
    std::string id = node->id;
    std::vector<AstNode *> args = node->args;
    std::string value = "";
    for(method& n : methods)
    {
        if(n.methodId == id)
        {
            //std::cout << "Method '" + id + "' FOUND\n";
            methodFunc = &n; 
            break;
        }   
    }  
    

    if(methodFunc->methodId == "")
    {
        throw std::runtime_error("Method '" + id + "' does not exist\n");
    }else{
        if(methodFunc->paramTypes.size() != args.size()){
            throw std::runtime_error("Invalid number of parameters on method call " + id + "\n");
        }else if(methodFunc->returnType->toString() == "void"){
            if (!methodFunc->variables.empty()) {
                 //std::cout << "Limpieza del contexto previo en " << methodFunc->methodId << "\n";
                methodFunc->variables.clear(); // Limpia variables del método anterior.
            }
            for(int i = methodFunc->paramTypes.size()-1; i >= 0 ; i--)
            {
                //std::cout << "PARAMETROSs DEL METODO = " << args[i]->toString()<< " de tipo " << static_cast<int>(args[i]->kind()) << methodFunc->paramTypes[i]->toString() << searchVariableInMethod(args[i]->toString(),methodId) << std::endl;
                
                if(variableExists(id))
                {
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] = variables[args[i]->toString()];
                }else if(methodFunc->paramTypes[i]->kind() == NodeKind::ExprNode)
                {
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] = std::stoi(evaluate(args[i], methodId));
                }else if((variableExistsInMethod(args[i]->toString(),methodId)) )
                {   
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] 
                        = searchVariableInMethod(args[i]->toString(),methodId);
                }else if(evaluate(args[i]).find("[") && evaluate(args[i]).find("]")){
                    std::string id = evaluate(args[i]);
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] 
                        = searchVariableInMethod(args[i]->toString(),methodId);
                    
                }else{
                    throw std::runtime_error("Invalid parameters on method call " + id + "\n");
                }
            }
            runVoidMethod(id);
        }else{
            if (!methodFunc->variables.empty()) {
                 //std::cout << "Limpieza del contexto previo en " << methodFunc->methodId << "\n";
                methodFunc->variables.clear(); // Limpia variables del método anterior.
            }
            for(int i = methodFunc->paramTypes.size()-1; i >= 0 ; i--)
            {
                //std::cout << "PARAMETROSs DEL METODO = " << args[i]->toString()<< " de tipo " << static_cast<int>(args[i]->kind()) << methodFunc->paramTypes[i]->toString() << searchVariableInMethod(args[i]->toString(),methodId) << std::endl;
                
                if(variableExists(id))
                {
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] = variables[args[i]->toString()];
                }else if(methodFunc->paramTypes[i]->kind() == NodeKind::ExprNode)
                {
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] = std::stoi(evaluate(args[i], methodId));
                }else if((variableExistsInMethod(args[i]->toString(),methodId)) )
                {   
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] 
                        = searchVariableInMethod(args[i]->toString(),methodId);
                }else if(evaluate(args[i]).find("[") && evaluate(args[i]).find("]")){
                    std::string id = evaluate(args[i]);
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] 
                        = searchVariableInMethod(args[i]->toString(),methodId);
                    
                }else{
                    throw std::runtime_error("Invalid parameters on method call " + id + "\n");
                }
            }
            
            value = runMethods(id);
            //std::cout << "VARIABLES ASGINADAS EN EL METODO = "  << methodFunc->methodId << std::endl;

        }

        // for(auto const& x : methodFunc->variables)
        // {
        //     //std::cout << x.first << " " << x.second << std::endl;
        // }
        
        // //std::cout << "VALOR DE LA FUNCION = " << value << std::endl;
        return value;
    }
}

std::string ExprEvaluator::typeNode(TypeNode *node)
{   
    //std::cout << "TypeNode\n";
    return node->type;
}

std::string ExprEvaluator::methodTypeNode(MethodType *node)
{
    //std::cout << "MethodTypeNode\n";
    return node->type;
}

std::string ExprEvaluator::printStmt(PrintStmt *node, std::string methodId)
{
    //std::cout << "Print\n";
    if(node->printExpr != nullptr)
    {
        ////std::cout << std::to_string(static_cast<int>(node->printExpr->kind())) << std::endl;
        std::string result = evaluate(node->printExpr, methodId);
        std::cout << result;
        return result;  
    }else{
        std::cout << node->printStr;
        return node->printStr;
    }
    return "";
}

void ExprEvaluator::readStmtNode(ReadStmt *node, std::string methodId)
{
    //std::cout << "ReadStmt\n";
    std::string id = node->id;
    if(variableExists(id))
    {
        std::cin >> variables[id];
    }else{
        throw std::runtime_error("Variable '" + id + "' does not exist\n");
    }
}

std::string ExprEvaluator::booleanExpr(BooleanExpr *node, std::string methodId)
{
    //std::cout << "BooleanExpr - " + methodId + "\n";
    for(AstNode *term : node->terms)
    {  
       return evaluate(term, methodId);
    }
    return "";
}

std::string ExprEvaluator::booleanTerm(BooleanTerm *node, std::string methodId)
{
    //std::cout << "BooleanTerm - " + methodId + "\n";
    for(AstNode *factor : node->factors)
    {
        ////std::cout << "BooleanFactor\n";
        ////std::cout << std::to_string(static_cast<int>(factor->kind())) << std::endl;
        return evaluate(factor, methodId);
    }
    return "";
}

std::string ExprEvaluator::booleanFactor(BooleanFactor *node, std::string methodId)
{
    //std::cout << "BooleanFactor\n";
    return node->factor->toString();
}

std::string ExprEvaluator::relationalExpr(RelationalExpr *node, std::string methodId)
{
    //std::cout << "RelationalExpr - " + methodId + "\n";
    return evaluate(node->ariths, methodId);
}

std::string ExprEvaluator::relationalEqualExpr(RelationalEqualExpr *node, std::string methodId)
{
    //std::cout << "RelationalEqualExpr\n";
    bool value = std::stoi(evaluate(node->leftArith, methodId)) == std::stoi(evaluate(node->rightArith, methodId));
    if(value)
    {
        return "True";
    }
    return "False";
}

std::string ExprEvaluator::relationalNotEqualExpr(RelationalNotEqualExpr *node, std::string methodId)
{
    //std::cout << "RelationalNotEqualExpr\n";
    bool value = std::stoi(evaluate(node->leftArith, methodId)) != std::stoi(evaluate(node->rightArith, methodId));
    if(value)
    {
        return "True";
    }
    return "False";
}

std::string ExprEvaluator::relationalLessThanExpr(RelationalLessThanExpr *node, std::string methodId)
{
    //std::cout << "RelationalLessThanExpr\n";
    bool value = std::stoi(evaluate(node->leftArith, methodId)) < std::stoi(evaluate(node->rightArith, methodId));
    if(value)
    {
        return "True";
    }
    return "False";
}

std::string ExprEvaluator::relationalLessEqualExpr(RelationalLessEqualExpr *node, std::string methodId)
{
    //std::cout << "RelationalLessEqualExpr\n";
    bool value = std::stoi(evaluate(node->leftArith, methodId)) <= std::stoi(evaluate(node->rightArith, methodId));
    if(value)
    {
        //std::cout << "True\n";
        return "True";
    }
    //std::cout << "False\n";
    return "False";

}

std::string ExprEvaluator::relationalGreaterThanExpr(RelationalGreaterThanExpr *node, std::string methodId)
{
    //std::cout << "RelationalGreaterThanExpr - " << methodId << std::endl;
    bool value =  std::stoi(evaluate(node->leftArith, methodId)) > std::stoi(evaluate(node->rightArith, methodId));
    if(value)
    {
        ////std::cout << std::stoi(evaluate(node->leftArith, methodId)) << " > " << std::stoi(evaluate(node->rightArith, methodId)) << std::endl;
        return "True";
    }
    return "False";
}

std::string ExprEvaluator::relationalGreaterEqualExpr(RelationalGreaterEqualExpr *node, std::string methodId)
{
    //std::cout << "RelationalGreaterEqualExpr\n";
    bool value = std::stoi(evaluate(node->leftArith, methodId)) >= std::stoi(evaluate(node->rightArith, methodId));
    if(value)
    {
        return "True";
    }
    return "False";
}

std::string ExprEvaluator::arithmeticExpr(ArithmeticExpr *node, std::string methodId)
{   
    //std::cout << "ArithmeticExpr - " + methodId + "Tamano" + std::to_string(node->terms.size()) + "\n";
    for(AstNode *term : node->terms)
    {   
        return evaluate(term, methodId);
    }
    return "";
}

int ExprEvaluator::arithAddExpr(ArithAddExpr *node, std::string methodId)
{
    //std::cout << "Evaluating ArithAddExpr: " << node->toString() << std::endl;
    std::string t1 = evaluate(node->t1, methodId);
    std::string t2 = evaluate(node->t2, methodId);
    //std::cout << "TERMINOS T1 = " << t1 << " T2 = " << t2 << " de metodo " << methodId << std::endl;
    try{
        int value =std::stoi(t1) + std::stoi(t2);
        //std::cout << "VALOR DE LA SUMA = " << value << std::endl;
        return value;
    }catch(const std::invalid_argument& e){
        throw std::runtime_error("Invalid argument on method call " + t1 +" - "+t2+ " - " + methodId + '\n');
    }
    
    
    
    
}


int ExprEvaluator::arithSubExpr(ArithSubExpr *node, std::string methodId)
{
    //std::cout << "ArithSubExpr\n";
    return std::stoi(evaluate(node->t1, methodId)) - std::stoi(evaluate(node->t2, methodId));
}

std::string ExprEvaluator::term(Term *node, std::string methodId)
{
    //std::cout << "Term - " + methodId + "\n";
    std::string result;
    for(AstNode *factor : node->factors)
    {
        result = evaluate(factor, methodId);
        //std::cout << "Term FInal = " + result + "\n";
        return result;
    }
    return "";

}

int ExprEvaluator::mulTerm(MulTerm *node, std::string methodId)
{   
    //std::cout << "MulTerm\n";
    return std::stoi(evaluate(node->f1, methodId)) * std::stoi(evaluate(node->f2, methodId));
}

int ExprEvaluator::divTerm(DivTerm *node, std::string methodId)
{
    //std::cout << "DivTerm\n";
    return std::stoi(evaluate(node->f1, methodId)) / std::stoi(evaluate(node->f2, methodId));
}

int ExprEvaluator::modTerm(ModTerm *node, std::string methodId)
{
    //std::cout << "ModTerm\n";
    return std::stoi(evaluate(node->f1, methodId)) % std::stoi(evaluate(node->f2, methodId));
}

std::string ExprEvaluator::factor(Factor *node, std::string methodId)
{
    //std::cout << "Factor - " + methodId + "\n";
    std::string result = evaluate(node->primary, methodId);
    //std::cout << "Factor Final- " + result + "\n";
    return result;
}

std::string ExprEvaluator::primaryNode(PrimaryNode *node, std::string methodId)
{       
    //std::cout << "PrimaryNode - "+ methodId+"\n";
    //std::cout << static_cast<int>(node->args->kind()) << std::endl;
    std::string name = evaluate(node->args, methodId);
    //std::cout << "PrimaryNode Final- "+ name + "\n"  << std::endl;
    return name;
}

int ExprEvaluator::primaryConst(PrimaryConst *node, std::string methodId)
{
    //std::cout << "PrimaryConst - " + node->value->toString() + "\n";
    return std::stoi(node->value->toString());
}

void ExprEvaluator::primaryArray(PrimaryArray *node, std::string methodId)
{
    //std::cout << "PrimaryArray\n";
    if(variableExists(node->identifier))
    {
        std::string index = evaluate(node->indexExpr);
        if(variableExists(node->identifier + "[" + index + "]"))
        {
            evaluate(node->indexExpr, methodId);
        }else{
            throw std::runtime_error("Variable '" + index + "' does not exist\n");
        }
    }else{
        throw std::runtime_error("Variable '" + node->identifier + "' does not exist\n");
    }
}

std::string ExprEvaluator::primaryFuncCall(PrimaryFuncCall *node, std::string methodId)
{
    //std::cout << "PrimaryFuncCall - " + methodId + "\n";
    method* methodFunc = new method();
    methodFunc->methodId = "";
    std::string id = node->identifier;
    std::vector<AstNode *> args = node->args;
    std::string value = "";
    
    for(method& n : methods)
    {
        if(n.methodId == id)
        {
            //std::cout << "Method '" + id + "' FOUND\n";
            methodFunc = &n; 
            break;
        }   
    }  
    

    if(methodFunc->methodId == "")
    {
        throw std::runtime_error("Method '" + id + "' does not exist\n");
    }else{
        if(methodFunc->paramTypes.size() != args.size()){
            throw std::runtime_error("Invalid number of parameters on method call " + id + "\n");
        }else if(methodFunc->returnType->toString() == "void"){
            if (!methodFunc->variables.empty()) {
                 //std::cout << "Limpieza del contexto previo en " << methodFunc->methodId << "\n";
                methodFunc->variables.clear(); // Limpia variables del método anterior.
            }
            for(int i = methodFunc->paramTypes.size()-1; i >= 0 ; i--)
            {
                //std::cout << "PARAMETROSs DEL METODO = " << args[i]->toString()<< " de tipo " << static_cast<int>(args[i]->kind()) << methodFunc->paramTypes[i]->toString() << searchVariableInMethod(args[i]->toString(),methodId) << std::endl;
                
                if(variableExists(id))
                {
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] = variables[args[i]->toString()];
                }else if(methodFunc->paramTypes[i]->kind() == NodeKind::ExprNode)
                {
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] = std::stoi(evaluate(args[i], methodId));
                }else if((variableExistsInMethod(args[i]->toString(),methodId)) )
                {   
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] 
                        = searchVariableInMethod(args[i]->toString(),methodId);
                }else if(evaluate(args[i]).find("[") && evaluate(args[i]).find("]")){
                    std::string id = evaluate(args[i]);
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] 
                        = searchVariableInMethod(args[i]->toString(),methodId);
                }else{
                    throw std::runtime_error("Invalid parameters on method call " + id + "\n");
                }
            }
            runVoidMethod(id);
        }else{
            // if (!methodFunc->variables.empty()) {
            //      //std::cout << "Limpieza del contexto previo en " << methodFunc->methodId << "\n";
            //     methodFunc->variables.clear(); // Limpia variables del método anterior.
            // }
            for(int i = methodFunc->paramTypes.size()-1; i >= 0 ; i--)
            {
               //std::cout << "PARAMETROSss DEL METODO = " << args[i]->toString()<< " de tipo " << static_cast<int>(methodFunc->paramTypes[i]->kind()) << methodFunc->paramTypes[i]->toString()<< std::endl;
                
                if(variableExists(id))
                {
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] = variables[args[i]->toString()];
                }else if(methodFunc->paramTypes[i]->kind() == NodeKind::BooleanExpr)
                {
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] = std::stoi(evaluate(args[i], methodId));
                }else if((variableExistsInMethod(args[i]->toString(),methodId)) )
                {   
                                        //std::cout << "Hola" << std::endl;

                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] 
                        = searchVariableInMethod(args[i]->toString(),methodId);
                }else if(evaluate(args[i]).find("[") && evaluate(args[i]).find("]")){
                    std::string id = evaluate(args[i]);
                    methodFunc->variables[methodFunc->paramTypes[i]->toString()] 
                        = searchVariableInMethod(args[i]->toString(),methodId);
                    
                }else{
                    throw std::runtime_error("Invalid parameters on method call " + id + "\n");
                }
            }
            //std::cout << "VARIABLES ASGINADAS EN EL METODO = "  << methodFunc->methodId << std::endl;
                for(auto const& x : methodFunc->variables)
                {
                    //std::cout << x.first << " " << x.second << std::endl;
                }
                value = runMethods(id);
        }
        // //std::cout << "VALOR DE LA FUNCION = " << value << std::endl;
        return value;
    }
}

std::string ExprEvaluator::primaryIdentifier(PrimaryIdentifier *node, std::string methodId)
{
    //std::cout << "PrimaryIdentifier - " + methodId + "\n";
    std::string id = node->identifier;
    if(methodId == "")
    {    if(variableExists(id))
        {
            //std::cout << "Variable '" + id + std::to_string(variables[id]) + "' exists\n";
            return std::to_string(variables[id]);
        }else{
            throw std::runtime_error("Variables '" + id + "' does not exist\n");
        }
    }else
    {
        for(method& method : methods)
        {
            if(method.methodId == methodId)
            {
                        

                if(variableExistsInMethod(id, methodId))
                {
                    //std::cout << "Variable '" + id + std::to_string(method.variables[id]) + "' exists\n";
                    return std::to_string(method.variables[id]);
                }else{
                    throw std::runtime_error("Variable '" + id + "' does not exist in method" + methodId + "\n");
                }
            }
        }
    }
    return "";
}