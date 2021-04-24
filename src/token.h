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

        ParenBegin,
        ParenEnd,

        BracketBegin,
        BracketEnd,

        Semicolon,
        Coma,

        NumericLiteral,
    };

    std::string_view leadingSpace;
    std::string_view content;
    std::string_view trailingSpace;

    size_t row, col;

    Type type;
};
