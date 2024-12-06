#pragma once 

#include <string>
#include <unordered_map>
#include "Ast.hpp"

class ExprEvaluator
{
    struct ParamsFromMethod{
        std::string methodId;
        bool isRef;
        std::string id;
        std::string type;
    };

    public:
        ExprEvaluator() = default;

        bool variableExists(std::string id);
        bool methodExists(std::string id);

        std::string evaluate(AstNode *node);
        void programNode(ProgramNode *node);
        void variableDeclNode(VariableDeclNode *node);
        void methodDeclNode(MethodDeclNode *node);
        int constant(Constant *node);
        void paramDeclNode(ParamDeclNode *node, std::string methodId);
        void assignStmtNode(AssignStmt *node);
        int returnStmtNode(ReturnStmt *node);
        void ifStmtNode(IfStmt *node);
        void whileStmtNode(WhileStmt *node);
        void callStmtNode(CallStmt *node);
        std::string typeNode(TypeNode *node);
        std::string methodTypeNode(MethodType *node);
        std::string printStmt(PrintStmt *node);
        void readStmtNode(ReadStmt *node);
        std::string booleanExpr(BooleanExpr *node);
        std::string booleanTerm(BooleanTerm *node); 
        std::string booleanFactor(BooleanFactor *node);
        std::string relationalExpr(RelationalExpr *node);
        bool relationalEqualExpr(RelationalEqualExpr *node);
        bool relationalNotEqualExpr(RelationalNotEqualExpr *node);
        bool relationalLessThanExpr(RelationalLessThanExpr *node);
        bool relationalLessEqualExpr(RelationalLessEqualExpr *node);
        std::string relationalGreaterThanExpr(RelationalGreaterThanExpr *node);
        bool relationalGreaterEqualExpr(RelationalGreaterEqualExpr *node);
        std::string arithmeticExpr(ArithmeticExpr *node);
        int arithAddExpr(ArithAddExpr *node);
        int arithSubExpr(ArithSubExpr *node);
        std::string term(Term *node);
        int mulTerm(MulTerm *node);
        int divTerm(DivTerm *node);
        int modTerm(ModTerm *node);
        std::string factor(Factor *node);
        std::string primaryNode(PrimaryNode *node);
        int primaryConst(PrimaryConst *node);
        void primaryArray(PrimaryArray *node);
        void primaryFuncCall(PrimaryFuncCall *node);
        std::string primaryIdentifier(PrimaryIdentifier *node);
        

    private:
        std::unordered_map<std::string, int> variables;
        std::vector<ParamsFromMethod> paramsFromMethod;
};