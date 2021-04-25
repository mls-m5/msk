#include "parser.h"
#include <memory>

namespace {

struct Parser {
    enum class State {
        Default,
        VariableDeclaration,
        Function,
        Import,
    };

    Parser(AstConsumer _consumer)
        : _consumer(std::move(_consumer)) {}

    void stateDefault(Ast ast) {
        switch (ast.token.type) {
        case Token::Let:
            _current = std::move(ast);
            _current.token.type = Token::VariableDeclaration;
            _state = State::VariableDeclaration;
            break;
        case Token::Func:
            _current = std::move(ast);
            _current.token.type = Token::Function;
            _state = State::Function;
            break;
        case Token::Import:
            _current = std::move(ast);
            _current.token.type = Token::ImportStatement;
            _state = State::Import;
            break;
        default:
            _consumer(std::move(ast));
        }
    }

    // And statement and use the last ast-node as end token
    void endStatement(Ast ast) {
        _current.end = std::move(ast.token);
        endStatement();
    }

    void endStatement() {
        _consumer(std::move(_current));
        _state = State::Default;
        _current = {};
    }

    void stateVariableDeclaration(Ast ast) {
        if (ast.token.type == Token::Semicolon) {
            endStatement(std::move(ast));
        }
        else {
            _current.children.push_back(std::move(ast));
        }
    }

    void stateFunction(Ast ast) {
        _current.children.push_back(std::move(ast));
        switch (ast.token.type) {
        case Token::BraceGroup:
            _current.children.push_back(std::move(ast));
            endStatement();
            break;
        case Token::Semicolon:
            _current.token.type = Token::FunctionDefinition;
            endStatement(std::move(ast));
            break;
        default:
            _current.children.push_back(std::move(ast));
        }
    }

    void statetImport(Ast ast) {
        if (ast.token.type == Token::Semicolon) {
            endStatement(std::move(ast));
        }
        else {
            _current.children.push_back(std::move(ast));
        }
    }

    //! Todo: Rewrite
    void operator()(Ast ast) {
        if (ast.token.type == Token::None) {
            return;
        }
        switch (_state) {
        case State::Default:
            stateDefault(std::move(ast));
            break;
        case State::VariableDeclaration:
            stateVariableDeclaration(std::move(ast));
            break;
        case State::Function:
            stateFunction(std::move(ast));
            break;
        case State::Import:
            statetImport(std::move(ast));
            break;
        }
    }

private:
    AstConsumer _consumer;
    State _state = State::Default;
    Ast _current;
};

} // namespace

AstConsumer parser(AstConsumer consumer) {
    auto parser = std::make_shared<Parser>(consumer);

    return [parser](Ast ast) { (*parser)(std::move(ast)); };
}
