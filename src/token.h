#pragma once

#include <string>

//! Next after a character, the Token is the smallest unit of code
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

        // Composite values
        Function,
        ForLoop,
        WhileLoop,
        IfStatement,
        IfElseStatement,
        SwitchStatement,

        ExportStatement,

        Expression,

        VariableDeclaration,
        Name,

    };

    std::string content;

    std::string leadingSpace;
    std::string trailingSpace;

    size_t row, col;

    Type type;
};
