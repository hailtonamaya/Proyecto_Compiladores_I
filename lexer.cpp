#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <sstream>
#include "lexer.hpp"

enum class State {
    Tokens_q0,
    Tokens_q1,
    Tokens_q11,
    Tokens_q13,
    Tokens_q14,
    Tokens_q4,
    Tokens_q5,
    Tokens_q7,
    Tokens_q8,
    Tokens_q9,
    Symbols_q0,
    Operators_q0,
    Operators_q16,
    Operators_q17,
    Operators_q19,
    Operators_q20,
    Operators_q21,
    Operators_q22,
    Operators_q6,
};

void Lexer::ungetToken(Token tk)
{   
    tokenAhead.tk = tk;
    tokenStack.push(tokenAhead);  // Almacenar el token en la pila
}

void Lexer::ungetTokens(const std::vector<tkAhead>& tokens)
{
    for (tkAhead tk : tokens) {
        tokenAhead = tk;
        tokenStack.push(tokenAhead);  // Almacena cada token en la pila
    }
}

Token Lexer::getNextToken()
{
    if (!tokenStack.empty()) {
        tkAhead tk = tokenStack.top();
        tokenStack.pop();  // Elimina el token de la pila
       // std::cout << "Returning token from stack: " << tokenToString(tk.tk) << text << std::endl;
        text = tk.text;
        return tk.tk;
    }
    text.clear();
    State state = State::Tokens_q0;
    int ch = getNextChar();

    while (true) {
        switch (state) {
            // 
            case State::Tokens_q0:
                if ((ch == '\n') || (ch == '\t') || (ch == ' ')) {
                    state = State::Tokens_q0;
                    ch = getNextChar();
                }
                else if (((ch >= 'a') && (ch <= 'z')) || (ch == '_') || ((ch >= 'A') && (ch <= 'Z'))) {
                    text += ch;
                    state = State::Tokens_q9;
                    ch = getNextChar();
                }
                else if (ch == '"') {
                    //text += ch;
                    state = State::Tokens_q4;
                    ch = getNextChar();
                }
                else if ((ch >= '1') && (ch <= '9')) {
                    text += ch;
                    state = State::Tokens_q1;
                    ch = getNextChar();
                }
                else if (ch == '0') {
                    text += ch;
                    state = State::Tokens_q7;
                    ch = getNextChar();
                }
                else if (ch == EOF) {
                    return Token::Eof;
                }
                else {
                    // Trying next automaton '
                    state = State::Symbols_q0;
                }
                break;
            case State::Tokens_q1:
                if ((ch >= '0') && (ch <= '9')) {
                    text += ch;
                    state = State::Tokens_q1;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::IntConst;
                }
                break;
            case State::Tokens_q11:
                if ((ch == '0') || (ch == '1')) {
                    text += ch;
                    state = State::Tokens_q11;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::Binary;
                }
                break;
            case State::Tokens_q13:
                if (((ch >= 'A') && (ch <= 'F')) || ((ch >= 'a') && (ch <= 'f')) || ((ch >= '0') && (ch <= '9'))) {
                    text += ch;
                    state = State::Tokens_q14;
                    ch = getNextChar();
                }
                else {
                    // Trying next automaton '
                    state = State::Symbols_q0;
                }
                break;
            case State::Tokens_q14:
                if (((ch >= 'A') && (ch <= 'F')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'f'))) {
                    text += ch;
                    state = State::Tokens_q14;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::HexConst;
                }
                break;
            case State::Tokens_q4:
                if ((ch == '\n') || (ch == '\t') || (ch == ' ')) {
                    text += ch;
                    state = State::Tokens_q4;
                    ch = getNextChar();
                }
                else if ((ch == '-') || (ch == ')') || (ch == '%') || ((ch >= 'A') && (ch <= 'Z')) || (ch == '!') || (ch == '_') || (ch == '>') || (ch == '[') || (ch == ':') || (ch == '.') || (ch == '*') || (ch == '&') || (ch == '?') || (ch == '}') || (ch == '/') || (ch == '+') || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '@') || (ch == '~') || (ch == ']') || (ch == '<') || (ch == ',') || (ch == '(') || (ch == '$') || (ch == '^') || (ch == '{')) {
                    text += ch;
                    state = State::Tokens_q4;
                    ch = getNextChar();
                }
                else if (ch == '"') {
                    //text += ch;
                    state = State::Tokens_q5;
                    ch = getNextChar();
                }
                else if (ch == EOF) {
                    return Token::Eof;
                }
                else {
                    // Trying next automaton '
                    state = State::Symbols_q0;
                }
                break;
            case State::Tokens_q5:
                ungetChar(ch);
                return Token::StringLiteral;
                break;
            case State::Tokens_q7:
                if ((ch == 'x') || (ch == 'X')) {
                    text += ch;
                    state = State::Tokens_q13;
                    ch = getNextChar();
                }
                else if ((ch == 'B') || (ch == 'b')) {
                    text += ch;
                    state = State::Tokens_q8;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::IntConst;
                }
                break;
            case State::Tokens_q8:
                if ((ch == '0') || (ch == '1')) {
                    text += ch;
                    state = State::Tokens_q11;
                    ch = getNextChar();
                }
                else {
                    // Trying next automaton '
                    state = State::Symbols_q0;
                }
                break;
            case State::Tokens_q9:
                if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_')) {
                    text += ch;
                    state = State::Tokens_q9;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::Ident;
                }
                break;
            // 
            case State::Symbols_q0:
                if (ch == '[') {
                    text += ch;
                    return Token::OpenBracket;
                }
                else if ((ch == '\n') || (ch == ' ') || (ch == '\t')) {
                    state = State::Symbols_q0;
                    ch = getNextChar();
                }
                else if (ch == ';') {
                    text += ch;
                    return Token::Semicolon;
                }
                else if (ch == '(') {
                    text += ch;
                    return Token::OpenPar;
                }
                else if (ch == '{') {
                    text += ch;
                    return Token::OpenCurly;
                }
                else if (ch == '}') {
                    text += ch;
                    return Token::CloseCurly;
                }
                else if (ch == ',') {
                    text += ch;
                    return Token::Comma;
                }
                else if (ch == ')') {
                    text += ch;
                    return Token::ClosePar;
                }
                else if (ch == ']') {
                    text += ch;
                    return Token::CloseBracket;
                }
                else {
                    // Trying next automaton '
                    state = State::Operators_q0;
                }
                break;
            // 
            case State::Operators_q0:
                if ((ch == '\n') || (ch == ' ') || (ch == '\t')) {
                    state = State::Operators_q0;
                    ch = getNextChar();
                }
                else if (ch == '<') {
                    text += ch;
                    state = State::Operators_q17;
                    ch = getNextChar();
                }
                else if (ch == '|') {
                    text += ch;
                    state = State::Operators_q22;
                    ch = getNextChar();
                }
                else if (ch == '!') {
                    text += ch;
                    state = State::Operators_q19;
                    ch = getNextChar();
                }
                else if (ch == '&') {
                    text += ch;
                    state = State::Operators_q16;
                    ch = getNextChar();
                }
                else if (ch == '-') {
                    text += ch;
                    return Token::OpSub;
                }
                else if (ch == '%') {
                    text += ch;
                    return Token::OpMod;
                }
                else if (ch == '+') {
                    text += ch;
                    return Token::OpAdd;
                }
                else if (ch == '=') {
                    text += ch;
                    state = State::Operators_q6;
                    ch = getNextChar();
                }
                else if (ch == '*') {
                    text += ch;
                    return Token::OpMul;
                }
                else if (ch == '/') {
                    text += ch;
                    return Token::OpDiv;
                }
                else if (ch == '>') {
                    text += ch;
                    state = State::Operators_q21;
                    ch = getNextChar();
                }
                else {
                    reportError(ch);
                    ch = getNextChar();
                    state = State::Tokens_q0;
                }
                break;
            case State::Operators_q16:
                if (ch == '&') {
                    text += ch;
                    return Token::OpBoolAnd;
                }
                else {
                    reportError(ch);
                    ch = getNextChar();
                    state = State::Tokens_q0;
                }
                break;
            case State::Operators_q17:
                if (ch == '=') {
                    text += ch;
                    return Token::OpLessEqual;
                }
                else {
                    ungetChar(ch);
                    return Token::OpLessThan;
                }
                break;
            case State::Operators_q19:
                if (ch == '=') {
                    text += ch;
                    state = State::Operators_q20;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::OpBoolNot;
                }
                break;
            case State::Operators_q20:
                ungetChar(ch);
                return Token::OpNotEqual;
                break;
            case State::Operators_q21:
                if (ch == '=') {
                    text += ch;
                    return Token::OpGreaterEqual;
                }
                else {
                    ungetChar(ch);
                    return Token::OpGreaterThan;
                }
                break;
            case State::Operators_q22:
                if (ch == '|') {
                    text += ch;
                    return Token::OpBoolOr;
                }
                else {
                    reportError(ch);
                    ch = getNextChar();
                    state = State::Tokens_q0;
                }
                break;
            case State::Operators_q6:
                if (ch == '=') {
                    text += ch;
                    return Token::OpEqual;
                }
                else {
                    ungetChar(ch);
                    return Token::OpAssign;
                }
                break;
        }
    }

}

const char *Lexer::tokenToString(Token tk)
{
    switch (tk) {
        case Token::OpAssign: return "OpAssign";
        case Token::OpGreaterThan: return "OpGreaterThan";
        case Token::OpGreaterEqual: return "OpGreaterEqual";
        case Token::OpNotEqual: return "OpNotEqual";
        case Token::OpEqual: return "OpEqual";
        case Token::OpLessThan: return "OpLessThan";
        case Token::OpAdd: return "OpAdd";
        case Token::OpMod: return "OpMod";
        case Token::OpBoolAnd: return "OpBoolAnd";
        case Token::CloseBracket: return "CloseBracket";
        case Token::Eof: return "Eof";
        case Token::OpLessEqual: return "OpLessEqual";
        case Token::OpSub: return "OpSub";
        case Token::IntConst: return "IntConst";
        case Token::Binary: return "Binary";
        case Token::HexConst: return "HexConst";
        case Token::OpenPar: return "OpenPar";
        case Token::Comma: return "Comma";
        case Token::OpenBracket: return "OpenBracket";
        case Token::StringLiteral: return "StringLiteral";
        case Token::Ident: return "Ident";
        case Token::Semicolon: return "Semicolon";
        case Token::OpBoolNot: return "OpBoolNot";
        case Token::CloseCurly: return "CloseCurly";
        case Token::OpBoolOr: return "OpBoolOr";
        case Token::OpDiv: return "OpDiv";
        case Token::OpMul: return "OpMul";
        case Token::OpenCurly: return "OpenCurly";
        case Token::ClosePar: return "ClosePar";
        default: return "Unknown";
    }

}