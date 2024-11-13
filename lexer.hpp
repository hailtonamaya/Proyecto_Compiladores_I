#ifndef _EXPR_LEXER_H
#define _EXPR_LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "tokens.hpp"

class Lexer {
public:
    Lexer(std::ifstream& in): in(in), line(1) {}
    struct tkAhead
    {
        Token tk;
        std::string text;
    };
    tkAhead tokenAhead;
    Token getNextToken();
    unsigned getLineNo() { return line; }
    std::string getText() { return text; }
    static const char *tokenToString(Token tk);
    void ungetToken(Token tk);  
    void ungetTokens(const std::vector<tkAhead>& tokens); 
    void updateText(const std::string& txt) { text = txt; }
    
    
private:
    /*Token findKw(const std::string& txt) {
        if (txt == "print")
            return Token::KwPrint;
        else
            return Token::Ident;
    }*/

    int getNextChar() {
        int ch = in.get();
        if (ch == '\n') {
            line++;
        }
        return ch;
    }

    void ungetChar(int ch) {
        if (ch == '\n') {
            line--;
        }
        in.unget();
    }

    void reportError(int ch) {
        std::cerr << "Line " << line << ": Invalid Token '" << (char)ch << "'\n";
    }

private:
    std::stack<tkAhead> tokenStack;    
    std::string text;
    std::ifstream& in;
    int line;
    
};
#endif
