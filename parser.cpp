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

void Parser::updateText(Keyword kw)
{
    switch (kw) {
        case Keyword::Class:
            lex.updateText("class");
            break;
        case Keyword::Int:
            lex.updateText("int");
            break;
        case Keyword::Void:
            lex.updateText("void");
            break;
        case Keyword::Ref:
            lex.updateText("ref");
            break;
        case Keyword::If:
            lex.updateText("if");
            break;
        case Keyword::Else:
            lex.updateText("else");
            break;
        case Keyword::While:
            lex.updateText("while");
            break;
        case Keyword::Return:
            lex.updateText("return");
            break;
        case Keyword::Print:
            lex.updateText("print");
            break;
        case Keyword::Read:
            lex.updateText("read");
            break;
        default:
            break;
    }
}

void Parser::parse()
{
    curr_tk = lex.getNextToken();
    program();
    if (curr_tk != Token::Eof) {
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                 ": Invalid expression");
    }
}

void Parser::program()
{
    if(getKeyword(lex.getText()) == Keyword::Class){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::Ident){
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::OpenCurly){
                curr_tk = lex.getNextToken();
                while(getKeyword(lex.getText()) == Keyword::Int){
                        tokenAhead1.tk = curr_tk;
                        std::string tokenAhead1Text = lex.getText();  // Guardar texto en una variable temporal
                        tokenAhead1.text = tokenAhead1Text;

                        tokenAhead2.tk = lex.getNextToken();
                        std::string tokenAhead2Text = lex.getText();  // Guardar texto en una variable temporal
                        tokenAhead2.text = tokenAhead1Text;

                        tokenAhead3.tk = lex.getNextToken();
                        std::string tokenAhead3Text = lex.getText();  // Guardar texto en una variable temporal
                        tokenAhead3.text = tokenAhead1Text;
                        //std::cout << lex.getText() << std::endl;
                    if(lex.getText() != "("){
                        lex.ungetTokens({tokenAhead3, tokenAhead2, tokenAhead1});
                        curr_tk = lex.getNextToken();
                        variable_decl();
                    }else{
                        lex.ungetTokens({tokenAhead3, tokenAhead2, tokenAhead1});
                        curr_tk = lex.getNextToken();
                        break;
                    }
                }
                while(getKeyword(lex.getText()) == Keyword::Int || getKeyword(lex.getText()) == Keyword::Void){
                    method_decl();
                }
                if(curr_tk == Token::CloseCurly){
                    curr_tk = lex.getNextToken();
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected '}', but found '"
                                + lex.getText() + "'");
                } 
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected '{', but found '"
                                + lex.getText() + "'");
            }
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
        }
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected 'class', but found '"
                                + lex.getText() + "'");
    }
}

void Parser::variable_decl()
{
    type();
    if(curr_tk == Token::Ident){
        curr_tk = lex.getNextToken();
        while(curr_tk == Token::Comma){
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::Ident){
                curr_tk = lex.getNextToken();
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
            }
        }
        if(curr_tk == Token::Semicolon){
            curr_tk = lex.getNextToken();
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ';', but found '"
                                + lex.getText() + "'");
        }
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
    }
}

void Parser::type()
{
    //sstd::cout << "Type: " << Lexer::tokenToString(curr_tk) << lex.getText()<< std::endl;
    if(getKeyword(lex.getText()) == Keyword::Int){
        curr_tk = lex.getNextToken();
        //std::cout << "Type: " << Lexer::tokenToString(curr_tk) << lex.getText()<< std::endl;
        if(curr_tk == Token::OpenBracket){
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::IntConst){
                curr_tk = lex.getNextToken();
                //std::cout << "Type: " << lex.getText() << std::endl;
                if(curr_tk == Token::CloseBracket){
                    curr_tk = lex.getNextToken();
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                    ": Expected ']', but found '"
                                    + lex.getText() + "'");
                }
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
               /*modificar para hexa*/                 ": Expected INT CONST, but found '"
                                + lex.getText() + "'");
            }
            
        }
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected 'int', but found '"
                                + lex.getText() + "'");
    }
}

void Parser::method_decl()
{
    method_type();
    if(curr_tk == Token::Ident){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            opt_param_decl_list();
            if(curr_tk == Token::ClosePar){
                curr_tk = lex.getNextToken();
                if(curr_tk == Token::OpenCurly){
                    curr_tk = lex.getNextToken();
                    /*aqui me quede*/
                    if(curr_tk == Token::CloseCurly){
                        curr_tk = lex.getNextToken();
                    }else{
                        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                    ": Expected '}', but found '"
                                    + lex.getText() + "'");
                    }
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                    ": Expected '{', but found '"
                                    + lex.getText() + "'");
                }
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ')', but found '"
                                + lex.getText() + "'");
            }
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected '(', but found '"
                                + lex.getText() + "'");
        }
    }
}

void Parser::method_type()
{
    if(getKeyword(lex.getText()) == Keyword::Int || getKeyword(lex.getText()) == Keyword::Void){
        curr_tk = lex.getNextToken();
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected 'int' or 'void', but found '"
                                + lex.getText() + "'");
    }
}

void Parser::opt_param_decl_list()
{
   
}
