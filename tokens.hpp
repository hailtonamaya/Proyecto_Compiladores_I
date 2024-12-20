#ifndef __TOKENS_H__
#define __TOKENS_H__

enum class Token {
    OpAssign,
    OpGreaterThan,
    OpGreaterEqual,
    OpNotEqual,
    OpEqual,
    OpLessThan,
    OpAdd,
    OpMod,
    OpBoolAnd,
    CloseBracket,
    Eof,
    OpLessEqual,
    OpSub,
    IntConst,
    Binary,
    HexConst,
    OpenPar,
    Comma,
    OpenBracket,
    StringLiteral,
    Ident,
    Semicolon,
    OpBoolNot,
    CloseCurly,
    OpBoolOr,
    OpDiv,
    OpMul,
    OpenCurly,
    ClosePar,
};
#endif