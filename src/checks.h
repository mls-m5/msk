#pragma once

#include "ast.h"

void expectType(Ast &ast, Token::Type type) {
    if (ast.token.type != type) {
        throw std::runtime_error{std::string{"wrong type expected "} +
                                 std::string{typeName(type)} + " got " +
                                 std::string{typeName(ast.token.type)}};
    }
}
