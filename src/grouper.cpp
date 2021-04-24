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
        : _consumer{consumer} {}

    ~Grouper() {
        flush();
    }

    void flush() {
        _consumer(Ast{
            .token = {},
            .children = {},
        });
    }

    void beginGroup(Token &&token) {
        if (stack.empty()) {
            root = Ast::fromToken(std::move(token));
            stack.push_back(&root);
        }
        else {
            stack.back()->children.emplace_back(
                Ast::fromToken(std::move(token)));
            stack.push_back(&stack.back()->children.back());
        }
    }

    void endGroup(Token &&token, Token::Type type) {
        if (stack.empty()) {
            if (root.token.content.empty()) {
                throw std::runtime_error{"no matching found for '" +
                                         token.content + "'"};
            }
            root.end = std::move(token);
            root.token.type = type;
            _consumer(std::move(root));
        }
        else {
            stack.back()->end = std::move(token);
            stack.back()->token.type = type;
            stack.pop_back();

            if (stack.empty()) {
                _consumer(std::move(root));
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
        default:
            if (stack.empty()) {
                _consumer(Ast::fromToken(std::move(token)));
            }
            else {
                stack.back()->children.emplace_back(
                    Ast::fromToken(std::move(token)));
            }
        }
    }

private:
    AstConsumer _consumer;

    Ast root;
    std::list<Ast *> stack;
};

} // namespace

TokenConsumer grouper(AstConsumer consumer) {
    auto grouper = std::make_shared<Grouper>(std::move(consumer));
    return [grouper](Token token) { (*grouper)(std::move(token)); };
}
