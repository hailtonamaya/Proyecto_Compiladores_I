#pragma once

#include <vector>
#include <string>
#include <iostream>

enum class NodeKind
{
    ProgramNode,
    VariableDeclNode,
    Constant,
    MethodDeclNode,
    ParamDeclNode,
    StmtNode,
    AssignStmt,
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
    Term,
    Factor,
    Primary
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
        std::string toString() const override { return "Constant"; }
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
        std::string toString() const override { return "ParamDeclNode"; }

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
        std::string toString() const override { return "ReturnStmt"; } 
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
        std::string toString() const override { return "IfStmt"; } 
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
        std::string toString() const override { return "WhileStmt"; } 
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
        std::string toString() const override { return "CallStmt"; }
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
        std::string toString() const override { return "MethodType"; }
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
        std::string toString() const override { return "PrintStmt"; }
};

class ReadStmt: public AstNode
{
    public:
        ReadStmt(std::string id)
            : id(id)
        {}

        std::string id;

        NodeKind kind() const override { return NodeKind::ReadStmt; }
        std::string toString() const override { return "ReadStmt"; }
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
        BooleanExpr(std::vector<AstNode *> terms)
            : terms(terms){};
        

        std::vector<AstNode *> terms;

        NodeKind kind() const override { return NodeKind::BooleanExpr; }
        std::string toString() const override { return "BooleanExpr"; }
};

class BooleanTerm: public ExprNode
{
    public:
        BooleanTerm(std::vector<AstNode *> factors)
            : factors(factors)
        {}

        std::vector<AstNode *> factors;

        NodeKind kind() const override { return NodeKind::BooleanTerm; }
        std::string toString() const override { return "BooleanTerm"; }
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
        std::string toString() const override { return "BooleanFactor"; }
};

class RelationalExpr: public AstNode
{
    public:
        RelationalExpr(std::vector<AstNode *> ariths)
            : ariths(ariths)
        {}

        std::vector<AstNode *> ariths;
       

        NodeKind kind() const override { return NodeKind::RelationalExpr; }
        std::string toString() const override { return "RelationalExpr"; }
};

class ArithmeticExpr: public ExprNode
{
    public:
        ArithmeticExpr(std::vector<AstNode *> terms)
            : terms(terms)
        {};

        std::vector<AstNode *> terms;

        NodeKind kind() const override { return NodeKind::ArithmeticExpr; }
        std::string toString() const override { return "ArithmeticExpr"; }
};

class Term: public AstNode
{
    public:
        Term(std::vector<AstNode *> factors)
            : factors(factors)
        {}

        std::vector<AstNode *> factors;

        NodeKind kind() const override { return NodeKind::Term; }
        std::string toString() const override { return "Term"; }
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
        std::string toString() const override { return "Factor"; }
};

class PrimaryNode : public AstNode {
public:
    enum class PrimaryType {
        Constant,
        Identifier,
        ArrayAccess,
        FunctionCall,
        Expression
    };

private:
    PrimaryType type;                     // Tipo de nodo primario
    AstNode* value;                       // Nodo asociado (constant, expression, etc.)
    std::string identifier;               // Identificador (para variables o funciones)
    std::vector<AstNode*> arguments;      // Lista de argumentos (para función)
    AstNode* indexExpression;             // Índice del array (para ArrayAccess)

public:
    // Constructor para constantes
    PrimaryNode(AstNode* constantNode)
        : type(PrimaryType::Constant), value(constantNode), indexExpression(nullptr) {}

    // Constructor para identificadores
    PrimaryNode(const std::string& id)
        : type(PrimaryType::Identifier), identifier(id), value(nullptr), indexExpression(nullptr) {}

    // Constructor para acceso a arrays
    PrimaryNode(const std::string& id, AstNode* indexExpr)
        : type(PrimaryType::ArrayAccess), identifier(id), indexExpression(indexExpr) {}

    // Constructor para llamadas a funciones
    PrimaryNode(const std::string& id, const std::vector<AstNode*>& args)
        : type(PrimaryType::FunctionCall), identifier(id), arguments(args) {}

    // Constructor para expresiones entre paréntesis
    PrimaryNode(AstNode* exprNode, bool isExpression)
        : type(PrimaryType::Expression), value(exprNode), indexExpression(nullptr) {}

    PrimaryType getType() const { return type; }
    const std::string& getIdentifier() const { return identifier; }
    const std::vector<AstNode*>& getArguments() const { return arguments; }
    AstNode* getIndexExpression() const { return indexExpression; }
    AstNode* getValue() const { return value; }

    NodeKind kind() const override { return NodeKind::Primary; }
    std::string toString() const override { return "PrimaryNode"; }
};
