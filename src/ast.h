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

    //! Change type from one child with one specified type to another type
    //! @return true if found and false
    bool changeType(Token::Type from, Token::Type to) {
        if (auto it = find(from); it != children.end()) {
            it->token.type = to;
            return true;
        }

        return false;
    }

    std::list<Ast>::iterator find(Token::Type type) {
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (it->token.type == type) {
                return it;
            }
        }
        return children.end();
    }
};

std::ostream &operator<<(std::ostream &stream, const Ast &ast);
