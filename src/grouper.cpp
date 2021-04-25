#include "grouper.h"
#include <list>
#include <memory>
#include <stack>
#include <stdexcept>

namespace {

struct Grouper {
    Grouper(const Grouper &) = delete;
    Grouper(Grouper &&) = delete;
    Grouper &operator=(const Grouper &) = delete;
    Grouper &operator=(Grouper &&) = delete;

    Grouper(AstConsumer consumer)
        : _consumer{consumer} {
        _translationUnit.token.type = Token::TranslationUnit;
    }

    void beginGroup(Token &&token) {
        if (_stack.empty()) {
            _root = Ast::fromToken(std::move(token));
            _stack.push_back(&_root);
        }
        else {
            _stack.back()->children.emplace_back(
                Ast::fromToken(std::move(token)));
            _stack.push_back(&_stack.back()->children.back());
        }
    }

    void endGroup(Token &&token, Token::Type type) {
        if (_stack.empty()) {
            if (_root.token.content.empty()) {
                throw std::runtime_error{"no matching found for '" +
                                         token.content + "'"};
            }
            _root.end = std::move(token);
            _root.token.type = type;
            _translationUnit.children.push_back(std::move(_root));
        }
        else {
            _stack.back()->end = std::move(token);
            _stack.back()->token.type = type;
            _stack.pop_back();

            if (_stack.empty()) {
                _translationUnit.children.push_back(std::move(_root));
            }
        }
    }

    void operator()(Token token) {
        switch (token.type) {
        case Token::ParenBegin:
            beginGroup(std::move(token));
            break;
        case Token::ParenEnd:
            endGroup(std::move(token), Token::ParenGroup);
            break;
        case Token::BraceBegin:
            beginGroup(std::move(token));
            break;
        case Token::BraceEnd:
            endGroup(std::move(token), Token::BraceGroup);
            break;
        case Token::BracketBegin:
            beginGroup(std::move(token));
            break;
        case Token::BracketEnd:
            endGroup(std::move(token), Token::BracketGroup);
            break;
        case Token::Eof:
            if (!_stack.empty()) {
                throw std::runtime_error{"unexpected end of file"};
            }
            _consumer(std::move(_translationUnit));
            break;
        default:
            if (_stack.empty()) {
                _translationUnit.children.push_back(
                    Ast::fromToken(std::move(token)));
            }
            else {
                _stack.back()->children.emplace_back(
                    Ast::fromToken(std::move(token)));
            }
        }
    }

private:
    AstConsumer _consumer;

    Ast _root;
    std::list<Ast *> _stack;

    Ast _translationUnit;
};

} // namespace

TokenConsumer grouper(AstConsumer consumer) {
    auto grouper = std::make_shared<Grouper>(std::move(consumer));
    return [grouper](Token token) { (*grouper)(std::move(token)); };
}
