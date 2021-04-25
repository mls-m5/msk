#include "parser.h"
#include <memory>

namespace {

struct Parser {
    enum class State {
        Default,
        VariableDeclaration,
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
        default:
            _consumer(std::move(ast));
        }
    }

    void stateVariableDeclaration(Ast ast) {
        if (ast.token.type == Token::Semicolon) {
            _consumer(std::move(_current));
        }
        else {
            _current.children.push_back(std::move(ast));
        }
    }

    void operator()(Ast ast) {
        switch (_state) {
        case State::Default:
            stateDefault(std::move(ast));
            break;
        case State::VariableDeclaration:
            stateVariableDeclaration(std::move(ast));
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
