#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.hpp"
/*AGREGAR VARIABLES HEXADECIMALES, DECIMAL,
MODIFICAR PARTE DE CONSTANT DE LA GRAMATICA,
MODIFICAR EL AUTOMATA*/
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
    void parse();
    void updateText(Keyword kw);

private:
    void program();
    void variable_decl();
    void type();
    void method_decl();
    void method_type();
    void opt_param_decl_list();
    void param_decl();
    void stmt();
    void assign_stmt();
    void return_stmt();
    void if_stmt();
    void block();
    void while_stmt();
    void call_stmt();
    void print_stmt();
    void read_stmt();
    void expression();
    void boolean_expression();
    void boolean_term();
    void boolean_factor();
    void relational_expression();
    void arithmetic_expression();
    void term();
    void factor();
    void primary();

private:
    Lexer& lex;
    Token curr_tk;
    Lexer::tkAhead tokenAhead1, tokenAhead2, tokenAhead3;
};

#endif