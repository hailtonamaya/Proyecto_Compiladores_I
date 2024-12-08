#pragma once

#include <vector>
#include <string>
#include <iostream>

enum class NodeKind
{
    ProgramNode, VariableDeclNode, Constant, MethodDeclNode,
    ParamDeclNode, StmtNode, AssignStmt,
    ReturnStmt,
    IfStmt,
    WhileStmt,
    CallStmt,
    PrintStmt,
    ReadStmt,
    TypeNode,
    MethodType,
    ExprNode,
    BooleanExpr,
    BooleanTerm,
    BooleanFactor,
    RelationalExpr,
    ArithmeticExpr,
    ArithAddExpr,
    ArithSubExpr,
    Term,
    MulTerm,
    DivTerm,
    ModTerm,
    Factor,
    Primary,
    PrimaryConst,
    PrimaryArray,
    PrimaryFuncCall,
    PrimaryIdentifier,
    RelationalEqualExpr,
    RelationalNotEqualExpr,
    RelationalLessThanExpr,
    RelationalLessEqualExpr,
    RelationalGreaterThanExpr,
    RelationalGreaterEqualExpr
};

class AstNode
{
    public:
        AstNode() = default;
        virtual ~AstNode() = default;

        virtual NodeKind kind() const = 0;
        virtual std::string toString() const = 0;
};

class ProgramNode: public AstNode
{
    public: 
        ProgramNode(std::string prgName, const std::vector<AstNode *>& variableDecls, const std::vector<AstNode *>& methodDecls)
            : prgName(prgName), variableDecls(variableDecls), methodDecls(methodDecls)
        {}

        NodeKind kind() const override { return NodeKind::ProgramNode; }
        std::string toString() const override;

        std::string prgName;
        std::vector<AstNode *> variableDecls, methodDecls;
        

        
};

class VariableDeclNode: public AstNode
{
    public:
        VariableDeclNode(AstNode* type, const std::vector<std::string>& ids)
            : type(type), ids(ids)
        {}

        AstNode* type;
        std::vector<std::string> ids;

        NodeKind kind() const override { return NodeKind::VariableDeclNode; }
        std::string toString() const override;
};

class Constant: public AstNode
{
    public:
        Constant(std::string value)
            : value(value)
        {}

        std::string value;

        NodeKind kind() const override { return NodeKind::Constant; }
        std::string toString() const override { return value; }
};

class MethodDeclNode: public AstNode
{
    public:
        MethodDeclNode(AstNode* rtype, std::string name, const std::vector<AstNode *>& params, 
                        const std::vector<AstNode *>& variableDecls, const std::vector<AstNode *>& stmts)
                        : rtype(rtype), name(name), params(params), variableDecls(variableDecls), stmts(stmts)
                        {}
        
        AstNode* rtype;
        std::string name;
        std::vector<AstNode *> params, variableDecls, stmts;

        NodeKind kind() const override { return NodeKind::MethodDeclNode; }
        std::string toString() const override;
};

class ParamDeclNode: public AstNode
{
    public:
        ParamDeclNode(bool isReference, AstNode* type, std::string name)
            : isReference(isReference), type(type), name(name)
        {}

        bool isReference;
        AstNode* type;
        std::string name;

        NodeKind kind() const override { return NodeKind::ParamDeclNode; }
        std::string toString() const override {return name;}

};

class AssignStmt: public AstNode
{
    public:
        AssignStmt(std::string identifier, AstNode *indexExpr, AstNode *value)
            : identifier(identifier), indexExpr(indexExpr), value(value)
        {}

        std::string identifier;
        AstNode *indexExpr; 
        AstNode *value;

        NodeKind kind() const override { return NodeKind::AssignStmt; }
        std::string toString() const override;
};

class ReturnStmt: public AstNode
{
    public:
        ReturnStmt(AstNode *expr)
            : expr(expr)
        {}

        AstNode *expr;

        NodeKind kind() const override { return NodeKind::ReturnStmt; }
        std::string toString() const override;
};

class IfStmt: public AstNode
{
    public:
        IfStmt(AstNode *expr, const std::vector<AstNode *>& stmts, const std::vector<AstNode *>& elseStmts)
            : expr(expr), stmts(stmts), elseStmts(elseStmts)
        {}

        AstNode *expr;
        std::vector<AstNode *> stmts, elseStmts;

        NodeKind kind() const override { return NodeKind::IfStmt; }
        std::string toString() const override;
};

class WhileStmt: public AstNode
{
    public:
        WhileStmt(AstNode *expr, const std::vector<AstNode *>& stmts)
            : expr(expr), stmts(stmts)
        {}

        AstNode *expr;
        std::vector<AstNode *> stmts;

        NodeKind kind() const override { return NodeKind::WhileStmt; }
        std::string toString() const override;
};

class CallStmt: public AstNode
{
    public:
        CallStmt(std::string id, const std::vector<AstNode *>& args)
            : id(id), args(args)
        {}

        std::string id;
        std::vector<AstNode *> args;

        NodeKind kind() const override { return NodeKind::CallStmt; }
        std::string toString() const override;
};

class TypeNode: public AstNode
{
    public:
        TypeNode(std::string type)
            : type(type)
        {}

        std::string type;

        NodeKind kind() const override { return NodeKind::TypeNode; }
        std::string toString() const override { return type; }
};

class MethodType: public AstNode
{
    public:
        MethodType(std::string type)
            : type(type)
        {}

        std::string type;

        NodeKind kind() const override { return NodeKind::MethodType; }
        std::string toString() const override { return type; }
};

class PrintStmt: public AstNode
{
    public:
        PrintStmt(AstNode *printExpr, std::string printStr)
            : printExpr(printExpr), printStr(printStr)
        {}

        AstNode *printExpr;
        std::string printStr;

        NodeKind kind() const override { return NodeKind::PrintStmt; }
        std::string toString() const override;
};

class ReadStmt: public AstNode
{
    public:
        ReadStmt(std::string id)
            : id(id)
        {}

        std::string id;

        NodeKind kind() const override { return NodeKind::ReadStmt; }
        std::string toString() const override;
};

class ExprNode: public AstNode
{
    public:
        ExprNode() = default;

        NodeKind kind() const override { return NodeKind::ExprNode; }
        std::string toString() const override { return "ExprNode"; }
};

class BooleanExpr: public ExprNode
{
    public:
        BooleanExpr(const std::vector<AstNode *>& terms)
            : terms(terms){};
        

        std::vector<AstNode *> terms;

        NodeKind kind() const override { return NodeKind::BooleanExpr; }
        std::string toString() const override;
};

class BooleanTerm: public ExprNode
{
    public:
        BooleanTerm(const std::vector<AstNode *>& factors)
            : factors(factors)
        {}

        std::vector<AstNode *> factors;

        NodeKind kind() const override { return NodeKind::BooleanTerm; }
        std::string toString() const override;
};

class BooleanFactor: public AstNode
{
    public:
        BooleanFactor(bool negated, AstNode *factor)
            : negated(negated), factor(factor)
        {}

        bool negated;
        AstNode *factor;

        NodeKind kind() const override { return NodeKind::BooleanFactor; }
        std::string toString() const override;
};

class RelationalExpr: public AstNode
{
    public:
        RelationalExpr(AstNode * ariths)
            : ariths(ariths)
        {}

        AstNode * ariths;
       
        NodeKind kind() const override { return NodeKind::RelationalExpr; }
        std::string toString() const override;
};

class RelationalEqualExpr: public AstNode
{
    public:
        RelationalEqualExpr(AstNode* leftArith, AstNode* rightArith)
            : leftArith(leftArith), rightArith(rightArith)
        {}

        AstNode* leftArith;
        AstNode* rightArith;

        NodeKind kind() const override { return NodeKind::RelationalEqualExpr; }
        std::string toString() const override;
};

class RelationalNotEqualExpr: public AstNode
{
    public:
        RelationalNotEqualExpr(AstNode* leftArith, AstNode* rightArith)
            : leftArith(leftArith), rightArith(rightArith)
        {}

        AstNode* leftArith;
        AstNode* rightArith;

        NodeKind kind() const override { return NodeKind::RelationalNotEqualExpr; }
        std::string toString() const override;
};

class RelationalLessThanExpr: public AstNode
{
    public:
        RelationalLessThanExpr(AstNode* leftArith, AstNode* rightArith)
            : leftArith(leftArith), rightArith(rightArith)
        {}

        AstNode* leftArith;
        AstNode* rightArith;

        NodeKind kind() const override { return NodeKind::RelationalLessThanExpr; }
        std::string toString() const override;
};

class RelationalLessEqualExpr: public AstNode
{
    public:
        RelationalLessEqualExpr(AstNode* leftArith, AstNode* rightArith)
            : leftArith(leftArith), rightArith(rightArith)
        {}

        AstNode* leftArith;
        AstNode* rightArith;

        NodeKind kind() const override { return NodeKind::RelationalLessEqualExpr; }
        std::string toString() const override;
};

class RelationalGreaterThanExpr: public AstNode
{
    public:
        RelationalGreaterThanExpr(AstNode* leftArith, AstNode* rightArith)
            : leftArith(leftArith), rightArith(rightArith)
        {}

        AstNode* leftArith;
        AstNode* rightArith;

        NodeKind kind() const override { return NodeKind::RelationalGreaterThanExpr; }
        std::string toString() const override;
};

class RelationalGreaterEqualExpr: public AstNode
{
    public:
        RelationalGreaterEqualExpr(AstNode* leftArith, AstNode* rightArith)
            : leftArith(leftArith), rightArith(rightArith)
        {}

        AstNode* leftArith;
        AstNode* rightArith;

        NodeKind kind() const override { return NodeKind::RelationalGreaterEqualExpr; }
        std::string toString() const override;
};

class ArithmeticExpr: public ExprNode
{
    public:
        ArithmeticExpr(const std::vector<AstNode *>& terms)
            : terms(terms)
        {};

        std::vector<AstNode *> terms;

        NodeKind kind() const override { return NodeKind::ArithmeticExpr; }
        std::string toString() const override;
};

class ArithAddExpr: public ExprNode
{
    public:
        ArithAddExpr(AstNode* t1 , AstNode * t2)
            : t1(t1), t2(t2)
        {};

        AstNode* t1;
        AstNode* t2;

        NodeKind kind() const override { return NodeKind::ArithAddExpr; }
        std::string toString() const override;
};

class ArithSubExpr: public ExprNode
{
    public:
        ArithSubExpr(AstNode* t1 , AstNode* t2)
            : t1(t1), t2(t2)
        {};

        AstNode* t1;
        AstNode* t2;

        NodeKind kind() const override { return NodeKind::ArithSubExpr; }
        std::string toString() const override;
};

class Term: public AstNode
{
    public:
        Term(const std::vector<AstNode *>& factors)
            : factors(factors)
        {}

        std::vector<AstNode *> factors;

        NodeKind kind() const override { return NodeKind::Term; }
        std::string toString() const override;
};

class MulTerm: public AstNode
{
    public:
        MulTerm(AstNode* f1, AstNode* f2)
            : f1(f1), f2(f2)
        {}

        AstNode* f1;
        AstNode* f2;

        NodeKind kind() const override { return NodeKind::MulTerm; }
        std::string toString() const override;
};

class DivTerm: public AstNode
{
    public:
        DivTerm(AstNode* f1, AstNode* f2)
            : f1(f1), f2(f2)
        {}

        AstNode* f1;
        AstNode* f2;

        NodeKind kind() const override { return NodeKind::DivTerm; }
        std::string toString() const override;
};

class ModTerm: public AstNode
{
    public:
        ModTerm(AstNode* f1, AstNode* f2)
            : f1(f1), f2(f2)
        {}

        AstNode* f1;
        AstNode* f2;

        NodeKind kind() const override { return NodeKind::ModTerm; }
        std::string toString() const override;
};

class Factor: public AstNode
{
    public:
        Factor(AstNode* primary, bool isNegative )
            : primary(primary), isNegative(isNegative)
        {}

        AstNode* primary;     // Nodo primario
        bool isNegative = false;

        NodeKind kind() const override { return NodeKind::Factor; }
        std::string toString() const override;
};

class PrimaryNode: public AstNode
{
    public:
        PrimaryNode(AstNode * args)
            : args(args)
        {}

        AstNode * args;

        NodeKind kind() const override { return NodeKind::Primary; }
        std::string toString() const override;
};

class PrimaryConst: public AstNode
{
    public:
        PrimaryConst(AstNode* value)
            : value(value)
        {}

        AstNode* value;

        NodeKind kind() const override { return NodeKind::PrimaryConst; }
        std::string toString() const override;
};

class PrimaryIdentifier: public AstNode
{
    public:
        PrimaryIdentifier(std::string identifier)
            : identifier(identifier)
        {}

        std::string identifier;

        NodeKind kind() const override { return NodeKind::PrimaryIdentifier; }
        std::string toString() const override {return identifier;};
};

class PrimaryArray: public AstNode
{
    public:
        PrimaryArray(std::string identifier, AstNode* indexExpr)
            : identifier(identifier), indexExpr(indexExpr)
        {}

        std::string identifier;
        AstNode* indexExpr;

        NodeKind kind() const override { return NodeKind::PrimaryArray; }
        std::string toString() const override;
};

class PrimaryFuncCall: public AstNode
{
    public:
        PrimaryFuncCall(std::string identifier, const std::vector<AstNode *>& args)
            : identifier(identifier), args(args)
        {}

        std::string identifier;
        std::vector<AstNode *> args;

        NodeKind kind() const override { return NodeKind::PrimaryFuncCall; }
        std::string toString() const override {return identifier;};
};

