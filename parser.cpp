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
            //std::cout << lex.getText() << std::endl;
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
                    while(curr_tk == Token::Ident || getKeyword(lex.getText()) == Keyword::Int || getKeyword(lex.getText()) == Keyword::While || getKeyword(lex.getText()) == Keyword::Return || getKeyword(lex.getText()) == Keyword::Print || getKeyword(lex.getText()) == Keyword::Read){
                        stmt();
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
                                ": Expected ')', but found '"
                                + lex.getText() + "'");
            }
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected '(', but found '"
                                + lex.getText() + "'");
        }
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
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
    if(getKeyword(lex.getText()) == Keyword::Ref || getKeyword(lex.getText()) == Keyword::Int){
        param_decl();
        while(curr_tk == Token::Comma){
            curr_tk = lex.getNextToken();
            param_decl();
        }
    }
}

void Parser::param_decl()
{
    if(getKeyword(lex.getText()) == Keyword::Ref){
        curr_tk = lex.getNextToken();
        type();
        if(curr_tk == Token::Ident){
            curr_tk = lex.getNextToken();
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
        }
   }else{
        type();
        if(curr_tk == Token::Ident){
            curr_tk = lex.getNextToken();
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
        }
   }
}

void Parser::stmt()
{
    if(curr_tk == Token::Ident && lex.getText() != "if" && lex.getText() != "while" && lex.getText() != "return" && lex.getText() != "print" && lex.getText() != "read"){
        tokenAhead1.tk = curr_tk;
        std::string tokenAhead1Text = lex.getText();  
        tokenAhead1.text = tokenAhead1Text;

        tokenAhead2.tk = lex.getNextToken();
        std::string tokenAhead2Text = lex.getText();  
        tokenAhead2.text = tokenAhead2Text;
        //std::cout << "Token1" << tokenAhead1.text  << std::endl;

        //std::cout << "Token2" <<tokenAhead2.text  << std::endl;

        if(lex.getText() == "("){
            lex.ungetTokens({tokenAhead2, tokenAhead1});
            curr_tk = lex.getNextToken();
            //std::cout << "Call" << lex.getText()<<std::endl;
            call_stmt();
        }else{
            lex.ungetTokens({tokenAhead2, tokenAhead1});
            curr_tk = lex.getNextToken();
            assign_stmt();
        }
    }else if(getKeyword(lex.getText()) == Keyword::If){
        if_stmt();
    }else if(getKeyword(lex.getText()) == Keyword::While){
        while_stmt();
    }else if(getKeyword(lex.getText()) == Keyword::Return){
        return_stmt();
    }else if(getKeyword(lex.getText()) == Keyword::Print){
        print_stmt();
    }else if(getKeyword(lex.getText()) == Keyword::Read){
        read_stmt();
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, 'if', 'while', 'return', 'print' or 'read', but found '"
                                + lex.getText() + "'");
    }
}

void Parser::assign_stmt()
{   
    if(curr_tk == Token::Ident){
        curr_tk = lex.getNextToken();
        if(curr_tk ==Token::OpenBracket){
            curr_tk = lex.getNextToken();
            expression();
            if(curr_tk == Token::CloseBracket){
                curr_tk = lex.getNextToken();
                if(curr_tk == Token::OpAssign){
                curr_tk = lex.getNextToken();
                expression();
                if(curr_tk == Token::Semicolon){
                    curr_tk = lex.getNextToken();
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ';', but found '"
                                + lex.getText() + "'");
                }
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected '=', but found '"
                                + lex.getText() + "'");
            }
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ']', but found '"
                                + lex.getText() + "'");
            }
        }else{
            if(curr_tk == Token::OpAssign){
                curr_tk = lex.getNextToken();
                expression();
                if(curr_tk == Token::Semicolon){
                    curr_tk = lex.getNextToken();
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ';', but found '"
                                + lex.getText() + "'");
                }
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected '=', but found '"
                                + lex.getText() + "'");
            }
        }
    }
}

void Parser::return_stmt()
{
    if(getKeyword(lex.getText()) == Keyword::Return){
        curr_tk = lex.getNextToken();
        expression();
        if(curr_tk == Token::Semicolon){
            curr_tk = lex.getNextToken();
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ';', but found '"
                                + lex.getText() + "'");
        }
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected 'return', but found '"
                                + lex.getText() + "'");
    }
}

void Parser::if_stmt()
{
    if(getKeyword(lex.getText()) == Keyword::If){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            expression();
            if(curr_tk == Token::ClosePar){
                curr_tk = lex.getNextToken();
                block();
                if(getKeyword(lex.getText()) == Keyword::Else){
                    curr_tk = lex.getNextToken();
                    block();
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
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected 'if', but found '"
                                + lex.getText() + "'");
    }
}

void Parser::block()
{
    if(curr_tk == Token::OpenCurly){
        curr_tk = lex.getNextToken();
        while(curr_tk == Token::Ident || getKeyword(lex.getText()) == Keyword::Int || getKeyword(lex.getText()) == Keyword::While || getKeyword(lex.getText()) == Keyword::Return || getKeyword(lex.getText()) == Keyword::Print || getKeyword(lex.getText()) == Keyword::Read){
            stmt();
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
}

void Parser::while_stmt()
{
    if(getKeyword(lex.getText()) == Keyword::While){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            expression();
            if(curr_tk == Token::ClosePar){
                curr_tk = lex.getNextToken();
                block();
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
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected 'while', but found '"
                                + lex.getText() + "'");
    }
}

void Parser::call_stmt()
{
    if(curr_tk == Token::Ident){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            while(curr_tk == Token::IntConst || curr_tk == Token::Ident || curr_tk == Token::OpenPar 
                    || curr_tk == Token::OpSub || curr_tk == Token::OpAdd){ 
                expression();
                while(curr_tk == Token::Comma){
                    curr_tk = lex.getNextToken();
                    expression();
                }
            }
            if(curr_tk == Token::ClosePar){
                curr_tk = lex.getNextToken();
                if(curr_tk == Token::Semicolon){
                    curr_tk = lex.getNextToken();
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ';', but found '"
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
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
    }
}

void Parser::print_stmt()
{
    if(getKeyword(lex.getText()) == Keyword::Print){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::IntConst || curr_tk == Token::Ident || curr_tk == Token::OpenPar 
                                          || curr_tk == Token::OpSub || curr_tk == Token::OpAdd){
                expression();
            }else if(curr_tk == Token::StringLiteral){
                curr_tk = lex.getNextToken();
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected INT CONST, IDENTIFIER, '(', '+', '-', STRING LITERAL but found '"
                                + lex.getText() + "'");
            }
            if(curr_tk == Token::ClosePar){
                curr_tk = lex.getNextToken();
                if(curr_tk == Token::Semicolon){
                    curr_tk = lex.getNextToken();
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ';', but found '"
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
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected 'print', but found '"
                                + lex.getText() + "'");
    }
}

void Parser::read_stmt()
{
    if(getKeyword(lex.getText()) == Keyword::Read){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::Ident){
                curr_tk = lex.getNextToken();
                if(curr_tk == Token::ClosePar){
                    curr_tk = lex.getNextToken();
                    if(curr_tk == Token::Semicolon){
                        curr_tk = lex.getNextToken();
                    }else{
                        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ';', but found '"
                                + lex.getText() + "'");
                    }
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ')', but found '"
                                + lex.getText() + "'");
                }
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
            }
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected '(', but found '"
                                + lex.getText() + "'");
        }
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected 'read', but found '"
                                + lex.getText() + "'");
    }
}

void Parser::expression()
{
    boolean_expression();
}

void Parser::boolean_expression()
{
    boolean_term();
    while(curr_tk == Token::OpBoolOr){
        curr_tk = lex.getNextToken();
        boolean_term();
    }
}

void Parser::boolean_term()
{
    boolean_factor();
    while(curr_tk == Token::OpBoolAnd){
        curr_tk = lex.getNextToken();
        boolean_factor();
    }
}

void Parser::boolean_factor()
{
    if(curr_tk == Token::IntConst || curr_tk == Token::Ident || curr_tk == Token::OpenPar 
                                          || curr_tk == Token::OpSub || curr_tk == Token::OpAdd){
        relational_expression();
    }else if(curr_tk == Token::OpBoolNot){
        curr_tk = lex.getNextToken();
        boolean_factor();
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected '!', INT CONST, IDENTIFIER, '(', '+', '-' but found '"
                                + lex.getText() + "'");
    }
}

void Parser::relational_expression()
{
    arithmetic_expression();
    if(curr_tk == Token::OpLessThan || curr_tk == Token::OpLessEqual || curr_tk == Token::OpGreaterThan || curr_tk == Token::OpGreaterEqual || curr_tk == Token::OpEqual || curr_tk == Token::OpNotEqual){
        curr_tk = lex.getNextToken();
        arithmetic_expression();
    }
}

void Parser::arithmetic_expression()
{
    term();
    while(curr_tk == Token::OpAdd || curr_tk == Token::OpSub){
        curr_tk = lex.getNextToken();
        term();
    }
}

void Parser::term()
{
    factor();
    while(curr_tk == Token::OpMul || curr_tk == Token::OpDiv || curr_tk == Token::OpMod){
        curr_tk = lex.getNextToken();
        factor();
    }
}

void Parser::factor()
{
    if(curr_tk == Token::OpAdd){
        curr_tk = lex.getNextToken();
        primary();
    }else if(curr_tk == Token::OpSub){
        curr_tk = lex.getNextToken();
        primary();
    }else{
        primary();
    }
}

void Parser::primary()
{
    if(curr_tk == Token::IntConst){
        curr_tk = lex.getNextToken();
    }else if(curr_tk == Token::Ident){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenBracket){
            curr_tk = lex.getNextToken();
            expression();
            if(curr_tk == Token::CloseBracket){
                curr_tk = lex.getNextToken();
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ']', but found '"
                                + lex.getText() + "'");
            }
        }else if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::IntConst || curr_tk == Token::Ident || curr_tk == Token::OpenPar 
                                          || curr_tk == Token::OpSub || curr_tk == Token::OpAdd){
                expression();
                while(curr_tk == Token::Comma){
                    curr_tk = lex.getNextToken();
                    expression();
                }
            }
            if(curr_tk == Token::ClosePar){
                curr_tk = lex.getNextToken();
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ')', but found '"
                                + lex.getText() + "'");
            }
        }
    }else if(curr_tk == Token::OpenPar){
        curr_tk = lex.getNextToken();
        expression();
        if(curr_tk == Token::ClosePar){
            curr_tk = lex.getNextToken();
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected ')', but found '"
                                + lex.getText() + "'");
        }
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected INT CONST, IDENTIFIER, '(', but found '"
                                + lex.getText() + "'");
    }
}