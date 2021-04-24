#pragma once

#include "token.h"
#include <list>
#include <ostream>

//! A abstract source tre node. These can be added to eachother to create a tree
//! structure
struct Ast {
    Ast() = default;
    Ast(const Ast &) = delete;
    Ast &operator=(const Ast &) = delete;
    Ast(Ast &&) = default;
    Ast &operator=(Ast &&) = default;

    Token token;
    std::list<Ast> children;
    Token end; // end-token if any

    static auto fromToken(Token token) {
        return Ast{
            .token = std::move(token),
        };
    }
};

std::ostream &operator<<(std::ostream &stream, const Ast &ast);
