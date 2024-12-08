#pragma once 

#include <string>
#include <unordered_map>
#include "Ast.hpp"

class ExprEvaluator
{
    struct method{
        std::string methodId;
        AstNode* returnType;
        std::vector<AstNode*> paramTypes;
        std::vector<AstNode*> variableDecls;
        std::vector<AstNode*> stmts;
        std::unordered_map<std::string, int> variables;
    };

    public:
        ExprEvaluator() = default;

        bool variableExists(std::string id);
        bool variableExistsInMethod(std::string id, std::string methodId);
        bool methodExists(std::string id);
        bool isInt(std::string input);
        int searchVariableInMethod(std::string id, std::string methodId);
        std::string runMethods(std::string methodId);
        void runVoidMethods(std::string methodId);

        std::string evaluate(AstNode *node, std::string methodId = "");
        void programNode(ProgramNode *node);
        void variableDeclNode(VariableDeclNode *node, bool isMethod = false, std::string methodId = "");
        void methodDeclNode(MethodDeclNode *node);
        int constant(Constant *node);
        std::string paramDeclNode(ParamDeclNode *node, std::string methodId);
        void assignStmtNode(AssignStmt *node, std::string methodId = "");
        std::string returnStmtNode(ReturnStmt *node, std::string methodId = "");
        void ifStmtNode(IfStmt *node, std::string methodId = "");
        void whileStmtNode(WhileStmt *node, std::string methodId = "");
        void callStmtNode(CallStmt *node, std::string methodId = "");
        std::string typeNode(TypeNode *node);
        std::string methodTypeNode(MethodType *node);
        std::string printStmt(PrintStmt *node, std::string methodId = "");
        void readStmtNode(ReadStmt *node, std::string methodId = "");
        std::string booleanExpr(BooleanExpr *node, std::string methodId);
        std::string booleanTerm(BooleanTerm *node, std::string methodId); 
        std::string booleanFactor(BooleanFactor *node, std::string methodId);
        std::string relationalExpr(RelationalExpr *node, std::string methodId);
        bool relationalEqualExpr(RelationalEqualExpr *node);
        bool relationalNotEqualExpr(RelationalNotEqualExpr *node);
        bool relationalLessThanExpr(RelationalLessThanExpr *node);
        bool relationalLessEqualExpr(RelationalLessEqualExpr *node);
        std::string relationalGreaterThanExpr(RelationalGreaterThanExpr *node, std::string methodId);
        bool relationalGreaterEqualExpr(RelationalGreaterEqualExpr *node);
        std::string arithmeticExpr(ArithmeticExpr *node, std::string methodId);
        int arithAddExpr(ArithAddExpr *node, std::string methodId);
        int arithSubExpr(ArithSubExpr *node, std::string methodId);
        std::string term(Term *node, std::string methodId);
        int mulTerm(MulTerm *node, std::string methodId);
        int divTerm(DivTerm *node, std::string methodId);
        int modTerm(ModTerm *node, std::string methodId);
        std::string factor(Factor *node, std::string methodId);
        std::string primaryNode(PrimaryNode *node, std::string methodId);
        int primaryConst(PrimaryConst *node, std::string methodId);
        void primaryArray(PrimaryArray *node, std::string methodId);
        std::string primaryFuncCall(PrimaryFuncCall *node, std::string methodId);
        std::string primaryIdentifier(PrimaryIdentifier *node, std::string methodId);
        
        

    private:
        std::unordered_map<std::string, int> variables;
        std::vector<method> methods;
};