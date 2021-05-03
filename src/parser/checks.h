#pragma once

#include "parser/ast.h"

// Todo: Print line number and stuff
void expectType(const Ast &ast, Token::Type type) {
    if (ast.token.type != type) {
        throw std::runtime_error{std::string{"wrong type expected "} +
                                 std::string{typeName(type)} + " got " +
                                 std::string{typeName(ast.token.type)}};
    }
}

void parseError(const Ast &ast, std::string str) {
    throw std::runtime_error{ast.token.content + ": " + str};
}
