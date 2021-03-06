#include "parser/parser.h"
#include "parser/checks.h"
#include <memory>

namespace {

using iterator = std::list<Ast>::iterator;

void handle(Ast &ast, iterator &it);

//! Return iterator with result
iterator handleExpression(Ast &ast, iterator begin, iterator end) {
    Ast expression;
    expression.type(Token::Expression);

    expression.children.splice(
        expression.children.end(), ast.children, begin, end);
    return ast.children.insert(end, std::move(expression));
}

void reduce(Ast &ast,
            iterator begin,
            iterator end,
            Token::Type type,
            bool hasEndToken) {
    if (begin == end || std::next(begin) == end) {
        return;
    }

    else {
        Ast &first = *begin;
        first.type(type);
        if (hasEndToken) {
            auto endIt = std::prev(end);
            first.endToken = std::move(endIt->token);
            ast.children.erase(endIt);
        }
        first.children.splice(
            first.children.begin(), ast.children, std::next(begin), end);
    }
}

void handleFunction(Ast &ast, iterator &it) {
    for (auto end = it; end != ast.children.end(); ++end) {
        if (end->token.type == Token::BraceGroup) {
            reduce(ast, it, std::next(end), Token::Function, false);

            expectType(it->front(), Token::Word);
            it->front().type(Token::Name);

            it->changeType(Token::ParenGroup, Token::FunctionArguments);
            {
                auto group =
                    it->changeType(Token::BraceGroup, Token::FunctionBody);
                auto begin = group->begin();
                handle(*group, begin);
            }
            return;
        }
        else if (end->token.type == Token::Semicolon) {
            reduce(ast, it, std::next(end), Token::FunctionDefinition, true);

            expectType(it->front(), Token::Word);
            it->front().type(Token::Name);

            it->changeType(Token::ParenGroup, Token::FunctionArguments);
            return;
        }
    }
    parseError(*it, "expected ; or {");
}

void handleImport(Ast &ast, iterator &it) {
    for (auto end = it; end != ast.children.end(); ++end) {
        if (end->type() == Token::Semicolon) {
            reduce(ast, it, std::next(end), Token::ImportStatement, true);
            return;
        }
    }
    parseError(*it, "expected ;");
}

void handleVariableDeclaration(Ast &ast, iterator &it) {
    for (auto end = it; end != ast.children.end(); ++end) {
        if (end->type() == Token::Semicolon) {
            reduce(ast, it, std::next(end), Token::VariableDeclaration, true);
            expectType(it->front(), Token::Word);
            it->front().type(Token::Name);

            auto assignmentOperator = std::next(it->begin());
            expectType(*assignmentOperator, Token::Operator);
            handleExpression(*it, std::next(assignmentOperator), it->end());

            return;
        }
    }
    parseError(*it, "expected ;");
}

void handleExport(Ast &ast, iterator &it) {
    auto next = std::next(it);

    if (next == ast.end()) {
        std::runtime_error{"export expect statement"};
    }

    handle(ast, next); // Group forward
    reduce(ast, it, std::next(it, 2), Token::ExportStatement, false);
}

void handle(Ast &ast, iterator &it) {
    switch (it->token.type) {
    case Token::Func:
        handleFunction(ast, it);
        ++it;
        break;
    case Token::Import:
        handleImport(ast, it);
        ++it;
        break;
    case Token::Let:
    case Token::Var:
        handleVariableDeclaration(ast, it);
        ++it;
        break;
    case Token::Export:
        handleExport(ast, it);
        ++it;
        break;
    default:
        //        if (ast.type() != Token::Expression) {
        //            handleExpression(ast, );
        //        }
        ++it;
        parseError(*it, "unexpected token");
    }
}

//! Recursively group ast nodes to more complex structures
void group(Ast &ast) {
    for (auto it = ast.children.begin(); it != ast.children.end();) {
        handle(ast, it);
    }
}

struct Parser {
    Parser(AstConsumer consumer)
        : _consumer(consumer) {}

    //! Main entrypoint will only be called once (hopefully
    void operator()(Ast ast) {
        group(ast);
        _consumer(std::move(ast));
    }

private:
    AstConsumer _consumer;
};

} // namespace

AstConsumer parser(AstConsumer consumer) {
    auto parser = std::make_shared<Parser>(consumer);

    return [parser](Ast ast) { (*parser)(std::move(ast)); };
}
