#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.hpp"
#include "Ast.hpp"


enum class Keyword{
   Class,
   Int,
   Void,
   Ref,
   If,
   Else,
   While,
   Return,
   Print,
   Read,
   Error
};

class Parser
{
public:
    Parser(Lexer& lex): lex(lex){}
    Keyword getKeyword(const std::string& txt);
    AstNode *parse();
    void updateText(Keyword kw);

private:
    AstNode *program();
    AstNode *variable_decl();
    AstNode *type();
    AstNode *method_decl();
    AstNode *method_type();
    std::vector<AstNode*> opt_param_decl_list();
    AstNode *param_decl();
    AstNode *stmt();
    AstNode *assign_stmt();
    AstNode *return_stmt();
    AstNode *if_stmt();
    std::vector<AstNode*> block();
    AstNode *while_stmt();
    AstNode *call_stmt();
    AstNode *print_stmt();
    AstNode *read_stmt();
    AstNode *expression();
    std::vector<AstNode*> boolean_expression();
    AstNode *boolean_term();
    AstNode *boolean_factor();
    AstNode *relational_expression();
    AstNode *arithmetic_expression();
    AstNode *term();
    AstNode *factor();
    AstNode *primary();
    AstNode *constant();

private:
    Lexer& lex;
    Token curr_tk;
    Lexer::tkAhead tokenAhead1, tokenAhead2, tokenAhead3;
};

#endif