#include <stdexcept>
#include "parser.hpp"

Keyword Parser::getKeyword(const std::string& txt)
{
    if (txt == "class") {
        return Keyword::Class;
    }else if (txt == "int") {
        return Keyword::Int;
    }else if (txt == "void") {
        return Keyword::Void;
    }else if (txt == "ref") {
        return Keyword::Ref;
    }else if (txt == "if") {
        return Keyword::If;
    }else if (txt == "else") {
        return Keyword::Else;
    }else if (txt == "while") {
        return Keyword::While;
    }else if (txt == "return") {
        return Keyword::Return;
    }else if (txt == "print") {
        return Keyword::Print;
    }else if (txt == "read") {
        return Keyword::Read;
    }else {
        return Keyword::Error;
    }
}
void Parser::parse()
{
    curr_tk = lex.getNextToken();
    //programa();
    if (curr_tk != Token::Eof) {
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                 ": Invalid expression");
    }
}
