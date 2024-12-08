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

AstNode* Parser::parse()
{
    curr_tk = lex.getNextToken();
    AstNode* progr = program();
    if (curr_tk != Token::Eof) {
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                 ": Invalid expression");
    }
    return progr;
}

AstNode* Parser::program()
{
    std::vector<AstNode*> variableDecls;
    std::vector<AstNode*> methodDecls;
    std::string prgName;
    if(getKeyword(lex.getText()) == Keyword::Class){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::Ident){
            prgName = lex.getText();
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::OpenCurly){
                curr_tk = lex.getNextToken();
                while(getKeyword(lex.getText()) == Keyword::Int){
                        tokenAhead1.tk = curr_tk;
                        std::string tokenAhead1Text = lex.getText();  // Guardar texto en una variable temporal
                        tokenAhead1.text = tokenAhead1Text;

                        tokenAhead2.tk = lex.getNextToken();
                        std::string tokenAhead2Text = lex.getText();  // Guardar texto en una variable temporal
                        tokenAhead2.text = tokenAhead2Text;

                        tokenAhead3.tk = lex.getNextToken();
                        std::string tokenAhead3Text = lex.getText();  // Guardar texto en una variable temporal
                        tokenAhead3.text = tokenAhead2Text;
                        //std::cout << tokenAhead3 << std::endl;
                    if(lex.getText() != "("){
                        lex.ungetTokens({tokenAhead3, tokenAhead2, tokenAhead1});
                        curr_tk = lex.getNextToken();
                        variableDecls.push_back(variable_decl());
                    }else{
                        lex.ungetTokens({tokenAhead3, tokenAhead2, tokenAhead1});
                        curr_tk = lex.getNextToken();
                        break;
                    }
                }
                while(getKeyword(lex.getText()) == Keyword::Int || getKeyword(lex.getText()) == Keyword::Void){
                    methodDecls.push_back(method_decl());
                }
                if(curr_tk == Token::CloseCurly){
                    curr_tk = lex.getNextToken();
                    return new ProgramNode(prgName, variableDecls, methodDecls);
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

AstNode* Parser::variable_decl()
{
    AstNode* variabletype = type();
    std::vector<std::string> ids;
    std::string id;
    if(curr_tk == Token::Ident){
        id = lex.getText();
        // std::cout<< "ID1: " << id << std::endl;
        ids.push_back(id);
        curr_tk = lex.getNextToken();
        // std::cout << "SiguienteID1:" << lex.getText() << std::endl;
        while(curr_tk == Token::Comma){
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::Ident){
                id = lex.getText();
                // std::cout<< "ID2: " << id << std::endl;

                ids.push_back(id);
                curr_tk = lex.getNextToken();
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
            }
        }
        if(curr_tk == Token::Semicolon){
            curr_tk = lex.getNextToken();
            return new VariableDeclNode(variabletype, ids);
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

AstNode* Parser::type()
{   
    std::string type;
    if(getKeyword(lex.getText()) == Keyword::Int){
        curr_tk = lex.getNextToken();
        type = "int";
        if(curr_tk == Token::OpenBracket){
            type.clear();
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::IntConst || curr_tk == Token::HexConst || curr_tk == Token::Binary){
                constant();
                type = "int[]";
                if(curr_tk == Token::CloseBracket){
                    curr_tk = lex.getNextToken();
                    return new TypeNode(type);
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                    ": Expected ']', but found '"
                                    + lex.getText() + "'");
                }
            }else{
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                         ": Expected Keyword INT, but found '"
                                + lex.getText() + "'");
            }
            
        }
        return new TypeNode(type);
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected 'int', but found '"
                                + lex.getText() + "'");
    }
}

AstNode* Parser::constant()
{
    std::string value;
    if(curr_tk == Token::IntConst || curr_tk == Token::HexConst || curr_tk == Token::Binary){
        value = lex.getText();
        curr_tk = lex.getNextToken();
        return new Constant(value);
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected INTCONST, HEXCONST, BINARY, but found '"
                                + lex.getText() + "'");
    }
}

AstNode* Parser::method_decl()
{
    AstNode* mtype = method_type();
    std::string id;
    std::vector<AstNode*> params, variableDecls, stmts;
    if(curr_tk == Token::Ident){
        id = lex.getText();
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            params = opt_param_decl_list();
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
                        tokenAhead2.text = tokenAhead2Text;

                        tokenAhead3.tk = lex.getNextToken();
                        std::string tokenAhead3Text = lex.getText();  // Guardar texto en una variable temporal
                        tokenAhead3.text = tokenAhead3Text;
                        if(lex.getText() != "("){
                            lex.ungetTokens({tokenAhead3, tokenAhead2, tokenAhead1});
                            curr_tk = lex.getNextToken();
                            variableDecls.push_back(variable_decl());
                        }else{
                            lex.ungetTokens({tokenAhead3, tokenAhead2, tokenAhead1});
                            curr_tk = lex.getNextToken();
                            break;
                        }
                    }
                    while(curr_tk == Token::Ident || getKeyword(lex.getText()) == Keyword::Int || getKeyword(lex.getText()) == Keyword::While || getKeyword(lex.getText()) == Keyword::Return || getKeyword(lex.getText()) == Keyword::Print || getKeyword(lex.getText()) == Keyword::Read){
                        stmts.push_back(stmt());
                    }
                    if(curr_tk == Token::CloseCurly){
                        curr_tk = lex.getNextToken();
                        return new MethodDeclNode(mtype, id, params, variableDecls, stmts);
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

AstNode* Parser::method_type()
{
    std::string type;
    if(getKeyword(lex.getText()) == Keyword::Int || getKeyword(lex.getText()) == Keyword::Void){
        type = lex.getText();
        curr_tk = lex.getNextToken();
        return new TypeNode(type);
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected 'int' or 'void', but found '"
                                + lex.getText() + "'");
    }
}

std::vector<AstNode*> Parser::opt_param_decl_list()
{
    std::vector<AstNode*> paramDecls;
    if(getKeyword(lex.getText()) == Keyword::Ref || getKeyword(lex.getText()) == Keyword::Int){
        paramDecls.push_back(param_decl());
        while(curr_tk == Token::Comma){
            curr_tk = lex.getNextToken();
            paramDecls.push_back(param_decl());
        }
    }
    return paramDecls;
}

AstNode* Parser::param_decl()
{
    bool isReference;
    std::string name;
    AstNode* Ptype;
    if(getKeyword(lex.getText()) == Keyword::Ref){
        curr_tk = lex.getNextToken();
        isReference = true;
        Ptype = type();
        if(curr_tk == Token::Ident){
            name = lex.getText();
            curr_tk = lex.getNextToken();
            return new ParamDeclNode(isReference, Ptype, name);
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
        }
   }else{
        isReference = false;
        Ptype = type();
        if(curr_tk == Token::Ident){
            name = lex.getText();
            curr_tk = lex.getNextToken();
            return new ParamDeclNode(isReference, Ptype, name);
        }else{
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, but found '"
                                + lex.getText() + "'");
        }
   }
}

AstNode* Parser::stmt()
{
    AstNode *stmt;
    if(curr_tk == Token::Ident && lex.getText() != "if" && lex.getText() != "while" && lex.getText() != "return" && lex.getText() != "print" && lex.getText() != "read"){
        tokenAhead1.tk = curr_tk;
        std::string tokenAhead1Text = lex.getText();  
        tokenAhead1.text = tokenAhead1Text;

        tokenAhead2.tk = lex.getNextToken();
        std::string tokenAhead2Text = lex.getText();  
        tokenAhead2.text = tokenAhead2Text;


        if(lex.getText() == "("){
            lex.ungetTokens({tokenAhead2, tokenAhead1});
            curr_tk = lex.getNextToken();
            stmt = call_stmt();
        }else{
            lex.ungetTokens({tokenAhead2, tokenAhead1});
            curr_tk = lex.getNextToken();
            stmt = assign_stmt();
        }
    }else if(getKeyword(lex.getText()) == Keyword::If){
        stmt = if_stmt();
    }else if(getKeyword(lex.getText()) == Keyword::While){
        stmt = while_stmt();
    }else if(getKeyword(lex.getText()) == Keyword::Return){
        stmt = return_stmt();
    }else if(getKeyword(lex.getText()) == Keyword::Print){
        stmt = print_stmt();
    }else if(getKeyword(lex.getText()) == Keyword::Read){
        stmt = read_stmt();
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected IDENTIFIER, 'if', 'while', 'return', 'print' or 'read', but found '"
                                + lex.getText() + "'");
    }
    return stmt;
}

AstNode* Parser::assign_stmt()
{   
    std::string id;
    AstNode *indexExpr = nullptr;
    AstNode *value = nullptr;
    AstNode *assign_stmt = nullptr;
    
    if(curr_tk == Token::Ident){
        id = lex.getText();
        curr_tk = lex.getNextToken();
        if(curr_tk ==Token::OpenBracket){
            curr_tk = lex.getNextToken();
            indexExpr = expression();
            if(curr_tk == Token::CloseBracket){
                curr_tk = lex.getNextToken();
                if(curr_tk == Token::OpAssign){
                curr_tk = lex.getNextToken();
                value = expression();
                if(curr_tk == Token::Semicolon){
                    curr_tk = lex.getNextToken();
                    assign_stmt = new AssignStmt(id, indexExpr, value);
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
                value = expression();
                if(curr_tk == Token::Semicolon){
                    curr_tk = lex.getNextToken();
                    assign_stmt = new AssignStmt(id, indexExpr, value);
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
    return assign_stmt;

}

AstNode* Parser::return_stmt()
{
    AstNode *return_stmt;
    if(getKeyword(lex.getText()) == Keyword::Return){
        curr_tk = lex.getNextToken();
        return_stmt = expression();
        if(curr_tk == Token::Semicolon){
            curr_tk = lex.getNextToken();
            return new ReturnStmt(return_stmt);
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

AstNode* Parser::if_stmt()
{
    AstNode *condition;
    std::vector<AstNode*> stmts, elseStmts;
    if(getKeyword(lex.getText()) == Keyword::If){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            condition = expression();
            if(curr_tk == Token::ClosePar){
                curr_tk = lex.getNextToken();
                stmts = block();
                if(getKeyword(lex.getText()) == Keyword::Else){
                    curr_tk = lex.getNextToken();
                    elseStmts = block();
                }
                return new IfStmt(condition, stmts, elseStmts);
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

std::vector<AstNode*> Parser::block()
{
    std::vector<AstNode*> stmts;
    if(curr_tk == Token::OpenCurly){
        curr_tk = lex.getNextToken();
        while(curr_tk == Token::Ident || getKeyword(lex.getText()) == Keyword::Int 
                    || getKeyword(lex.getText()) == Keyword::While || getKeyword(lex.getText()) == Keyword::Return 
                    || getKeyword(lex.getText()) == Keyword::Print || getKeyword(lex.getText()) == Keyword::Read){
            stmts.push_back(stmt());
        }  
        if(curr_tk == Token::CloseCurly){
            curr_tk = lex.getNextToken();
            return stmts;
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

AstNode* Parser::while_stmt()
{
    AstNode* condition;
    std::vector<AstNode*> stmts;
    if(getKeyword(lex.getText()) == Keyword::While){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            condition = expression();
            if(curr_tk == Token::ClosePar){
                curr_tk = lex.getNextToken();
                stmts = block();
                return new WhileStmt(condition, stmts);
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

AstNode* Parser::call_stmt()
{
    std::string id;
    std::vector<AstNode*> args;
    if(curr_tk == Token::Ident){
        id = lex.getText();
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            while(curr_tk == Token::IntConst || curr_tk == Token::HexConst || curr_tk == Token::Binary || curr_tk == Token::Ident || curr_tk == Token::OpenPar 
                    || curr_tk == Token::OpSub || curr_tk == Token::OpAdd){ 
                args.push_back(expression());
                while(curr_tk == Token::Comma){
                    curr_tk = lex.getNextToken();
                    args.push_back(expression());
                }
            }
            if(curr_tk == Token::ClosePar){
                curr_tk = lex.getNextToken();
                if(curr_tk == Token::Semicolon){
                    curr_tk = lex.getNextToken();
                    return new CallStmt(id, args);
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

AstNode* Parser::print_stmt()
{
    AstNode* printExpr = nullptr;
    std::string printStr;
    if(getKeyword(lex.getText()) == Keyword::Print){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::IntConst || curr_tk == Token::HexConst || curr_tk == Token::Binary || curr_tk == Token::Ident || curr_tk == Token::OpenPar 
                                          || curr_tk == Token::OpSub || curr_tk == Token::OpAdd){
                printExpr = expression();
            }else if(curr_tk == Token::StringLiteral){
                printStr = lex.getText();
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
                    return new PrintStmt(printExpr, printStr);
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

AstNode* Parser::read_stmt()
{
    std::string id;
    if(getKeyword(lex.getText()) == Keyword::Read){
        curr_tk = lex.getNextToken();
        if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
            if(curr_tk == Token::Ident){
                id = lex.getText();
                curr_tk = lex.getNextToken();
                if(curr_tk == Token::ClosePar){
                    curr_tk = lex.getNextToken();
                    if(curr_tk == Token::Semicolon){
                        curr_tk = lex.getNextToken();
                        return new ReadStmt(id);
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

AstNode* Parser::expression()
{
    return new BooleanExpr(boolean_expression());
}

std::vector<AstNode*> Parser::boolean_expression()
{
    std::vector<AstNode*> booleanExpr;
    booleanExpr.push_back(boolean_term());
    while(curr_tk == Token::OpBoolOr){
        curr_tk = lex.getNextToken();
        booleanExpr.push_back(boolean_term());
    }
    return booleanExpr;
}

AstNode* Parser::boolean_term()
{   
    std::vector<AstNode*> booleanTerm;
    booleanTerm.push_back(boolean_factor());
    while(curr_tk == Token::OpBoolAnd){
        curr_tk = lex.getNextToken();
        booleanTerm.push_back(boolean_factor());
    }
    return new BooleanTerm(booleanTerm);
}

AstNode* Parser::boolean_factor()
{
    if(curr_tk == Token::IntConst || curr_tk == Token::HexConst || curr_tk == Token::Binary || curr_tk == Token::Ident || curr_tk == Token::OpenPar 
                                          || curr_tk == Token::OpSub || curr_tk == Token::OpAdd){
        AstNode* relationalExpression = relational_expression();
        return relationalExpression;
        
    }else if(curr_tk == Token::OpBoolNot){
        curr_tk = lex.getNextToken();
        AstNode* negatedFactor = boolean_factor(); // Parsea el siguiente factor
        return new BooleanFactor(true, negatedFactor);
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected '!', INT CONST, IDENTIFIER, '(', '+', '-' but found '"
                                + lex.getText() + "'");
    }
}

AstNode* Parser::relational_expression()
{
    AstNode* left = arithmetic_expression();
    if(curr_tk == Token::OpEqual){
        curr_tk = lex.getNextToken();
        AstNode* right = arithmetic_expression();
        return new RelationalEqualExpr(left, right);
    }else if(curr_tk == Token::OpLessThan){
        curr_tk = lex.getNextToken();
        AstNode* right = arithmetic_expression();
        return new RelationalLessThanExpr(left, right);
    }else if(curr_tk == Token::OpLessEqual){
        curr_tk = lex.getNextToken();
        AstNode* right = arithmetic_expression();
        return new RelationalLessEqualExpr(left, right);
    } else if(curr_tk == Token::OpGreaterThan){
        curr_tk = lex.getNextToken();
        AstNode* right = arithmetic_expression();
        return new RelationalGreaterThanExpr(left, right);
    } else if(curr_tk == Token::OpGreaterEqual){
        curr_tk = lex.getNextToken();
        AstNode* right = arithmetic_expression();
        return new RelationalGreaterEqualExpr(left, right);
    } else if(curr_tk == Token::OpNotEqual){
        curr_tk = lex.getNextToken();
        AstNode* right = arithmetic_expression();
        return new RelationalNotEqualExpr(left, right);
    }
    return new RelationalExpr(left);
}

AstNode* Parser::arithmetic_expression() {
    AstNode* t1 = term();  // Procesa el primer término (puede ser un número o una expresión más compleja)
    
    // Itera mientras haya operadores de suma o resta
    while (curr_tk == Token::OpAdd || curr_tk == Token::OpSub) {
        if (curr_tk == Token::OpAdd) {
            curr_tk = lex.getNextToken();  // Consume el operador '+'
            AstNode* t2 = term();  // Procesa el siguiente término
            t1 = new ArithAddExpr(t1, t2);  // Construye el árbol acumulativo
        } else if (curr_tk == Token::OpSub) {
            curr_tk = lex.getNextToken();  // Consume el operador '-'
            AstNode* t2 = term();  // Procesa el siguiente término
            t1 = new ArithSubExpr(t1, t2);  // Construye el árbol acumulativo
        }
    }
    
    return t1;  // Devuelve el árbol construido
}


AstNode* Parser::term()
{
    AstNode *f1 = factor();
    std::vector<AstNode*> factors;
    while(curr_tk == Token::OpMul || curr_tk == Token::OpDiv || curr_tk == Token::OpMod){
        if(curr_tk == Token::OpMul){
            curr_tk = lex.getNextToken();
            AstNode *f2 = factor();
            factors.push_back(new MulTerm(f1, f2));
        }else if(curr_tk == Token::OpDiv){
            curr_tk = lex.getNextToken();
            AstNode *f2 = factor();
            factors.push_back(new DivTerm(f1, f2));
        }else if(curr_tk == Token::OpMod){
            curr_tk = lex.getNextToken();
            AstNode *f2 = factor();
            factors.push_back(new ModTerm(f1, f2));
        }
    }
    if(factors.size() == 0){
        factors.push_back(f1);
    }
    return new Term(factors);
}

AstNode* Parser::factor()
{
    AstNode* primarys;
    bool isNegated = false;
    if(curr_tk == Token::OpAdd){
        curr_tk = lex.getNextToken();
        primarys = primary();
    }else if(curr_tk == Token::OpSub){
        curr_tk = lex.getNextToken();
        primarys = primary();
        isNegated = true;
    }else{
        primarys = primary();
    }
    return new Factor(primarys, isNegated);
}

AstNode* Parser::primary() {
    AstNode* args = nullptr;
    std::vector<AstNode*> params;
    if (curr_tk == Token::IntConst || curr_tk == Token::HexConst || curr_tk == Token::Binary) {
        // Caso: constante
        AstNode* constantNode = constant(); // Genera el nodo de constante
        args=new PrimaryConst(constantNode);

    } else if (curr_tk == Token::Ident) {
        // Caso: identificador (variable, acceso a array, o llamada a función)
        std::string identifier = lex.getText();
        curr_tk = lex.getNextToken();

        if (curr_tk == Token::OpenBracket) {
            // Caso: IDENTIFIER[expression]
            curr_tk = lex.getNextToken();
            AstNode* indexExpr = expression();
            if (curr_tk != Token::CloseBracket) {
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                         ": Expected ']', but found '" + lex.getText() + "'");
            }
            curr_tk = lex.getNextToken();
            args = new PrimaryArray(identifier, indexExpr);

        } else if (curr_tk == Token::OpenPar) {
            // Caso: IDENTIFIER(expression, ...)
            curr_tk = lex.getNextToken();
            

            if (curr_tk != Token::ClosePar) { // Si no está vacío, procesar argumentos
                do {
                    params.push_back(expression());
                    if (curr_tk == Token::Comma) {
                        curr_tk = lex.getNextToken();
                    } else {
                        break;
                    }
                } while (true);

                if(curr_tk == Token::ClosePar){
                    curr_tk = lex.getNextToken();
                    args = new PrimaryFuncCall(identifier, params);
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                             ": Expected ')', but found '" + lex.getText() + "'");
                }
            }else{
                curr_tk = lex.getNextToken();
                args = new PrimaryFuncCall(identifier, params);
                std::cout << static_cast<int>(args->kind()) << args->toString()<< std::endl;

            }

        } else {
            // Caso: IDENTIFIER (variable simple)
            args=new PrimaryIdentifier(identifier);
        }

    } else if (curr_tk == Token::OpenPar) {
        // Caso: (expression)
        curr_tk = lex.getNextToken();
        AstNode* exprNode = expression();
        if (curr_tk != Token::ClosePar) {
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                     ": Expected ')', but found '" + lex.getText() + "'");
        }
        curr_tk = lex.getNextToken();
        //args = new PrimaryNode(exprNode);

    } else {
        // Error: no coincide con ninguna opción válida
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                 ": Expected INT CONST, IDENTIFIER, '(', but found '" +
                                 lex.getText() + "'");
    }    
    return new PrimaryNode(args);
}
