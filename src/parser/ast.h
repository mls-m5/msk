#pragma once

#include "parser/token.h"
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
    Token endToken; // end-token if any

    static auto fromToken(Token token) {
        return Ast{
            .token = std::move(token),
        };
    }

    //! Change type from one child with one specified type to another type
    //! @return true if found and false
    std::list<Ast>::iterator changeType(Token::Type from, Token::Type to) {
        if (auto it = find(from); it != children.end()) {
            it->token.type = to;
            return it;
        }

        return children.end();
    }

    std::list<Ast>::iterator find(Token::Type type) {
        for (auto it = children.begin(); it != children.end(); ++it) {
            if (it->token.type == type) {
                return it;
            }
        }
        return children.end();
    }

    auto begin() {
        return children.begin();
    }

    auto end() {
        return children.end();
    }

    auto &front() {
        return children.front();
    }

    auto &back() {
        return children.back();
    }

    Token::Type type() {
        return token.type;
    }

    void type(Token::Type value) {
        token.type = value;
    }
};

std::ostream &operator<<(std::ostream &stream, const Ast &ast);
