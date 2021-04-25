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
        Import,
        Export,
        Return,

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
        FunctionDefinition,
        ForLoop,
        WhileLoop,
        IfStatement,
        IfElseStatement,
        SwitchStatement,

        ImportStatement,
        ExportStatement,

        Expression,

        VariableDeclaration,
        Name,

        TranslationUnit,

        Eof,

        Last,
    };

    std::string content;

    std::string leadingSpace;
    std::string trailingSpace;

    size_t row, col;

    Type type;
};

std::string_view typeName(Token::Type);
