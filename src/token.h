#pragma once

#include <string>

struct Token {
    enum Type : short {
        None,
        Word,
        StringLiteral,
        Operator,

        // Keywords
        Func,
        Var,
        Let,
        Const,

        BraceBegin,
        BraceEnd,
        BraceGroup,

        ParenBegin,
        ParenEnd,
        ParenGroup,

        BracketBegin,
        BracketEnd,
        BracketGroup,

        Semicolon,
        Coma,

        NumericLiteral,
    };

    std::string content;

    std::string leadingSpace;
    std::string trailingSpace;

    size_t row, col;

    Type type;
};