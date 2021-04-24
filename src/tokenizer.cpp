#include "tokenizer.h"
#include <string_view>
#include <vector>

namespace {

auto keywordMap = std::vector<std::pair<std::string_view, Token::Type>>{
    {"func", Token::Func},
};

Token::Type getKeyword(std::string_view str) {
    for (auto s : keywordMap) {
        if (s.first == str) {
            return s.second;
        }
    }
    return Token::None;
}

struct Tokenizer {
    enum class State {
        Default,
        Operator,
        Brace,
        Space,
    };

    Tokenizer(TokenConsumer consumer)
        : _consumer(consumer) {}
    Tokenizer(const Tokenizer &) = default;
    Tokenizer &operator=(const Tokenizer &) = default;
    Tokenizer(Tokenizer &&) = default;
    Tokenizer &operator=(Tokenizer &&) = default;

    inline void defaultState(char c) {
        switch (c) {
        case '{':
            startNewToken(c, State::Brace);
            sendToken();
            break;
        case '}':
            startNewToken(c, State::Brace);
            sendToken();
            break;
        case '=':
        case '+':
        case '-':
        case '*':
        case '/':
        case '<':
        case '>':
            startNewToken(c, State::Operator);
            break;

        case ' ':
        case '\r':
        case '\n':
        case '\t':
            _leadingSpace += c;
            startNewToken(0, State::Space);
            break;
        default:
            _current += c;
            break;
        }
    }

    inline void operatorState(char c) {
        // Handle multiple character operators in the future

        startNewToken(c);
    }

    inline void spaceState(char c) {
        if (isspace(c)) {
            if (_current.empty()) {
                _leadingSpace += c;
            }
            else {
                _trailingSpace += c;
            }
        }
        else {
            if (_current.empty()) {
                _state = State::Default;
            }
            else {
                sendToken();
                startNewToken(c);
            }
        }
    }

    Token::Type getType() {
        switch (_state) {
        case State::Brace:
            return '{' ? Token::BraceBegin : Token::BraceEnd;
            break;
        case State::Operator:
            return Token::Operator;
            break;
        case State::Space:
            return Token::None;
            break;
        case State::Default:
            break;
        }
        return Token::Word;
    }

    void sendToken(char c = 0) {
        if (_current.empty()) {
            return;
        }
        if (c) {
            _current += c;
        }
        _consumer(Token{
            .leadingSpace = _leadingSpace,
            .content = _current,
            .trailingSpace = _trailingSpace,
            .row = _row,
            .col = _colStart,
            .type = getType(),
        });

        _colStart = _col + 1;

        _leadingSpace.clear();
        _trailingSpace.clear();
        _current.clear();
    }

    void startNewToken(char c, State state = State::Default) {
        sendToken();
        _colStart = _col;
        _state = state;
        if (c) {
            _current = {c};
        }
    }

    // The enty point
    void operator()(Line line) {
        _colStart = _col = 1;
        _row = line.row;
        _current.clear();

        for (auto c : line.content) {
            switch (_state) {
            case State::Default:
                defaultState(c);
                break;
            case State::Operator:
                operatorState(c);
                break;
            case State::Space:
                spaceState(c);
                break;
            case State::Brace:
                throw "this should not happend";
                break;
            }
            ++_col;
        }

        sendToken();
    }

private:
    TokenConsumer _consumer;

    size_t _row = 0;
    size_t _col = 1;
    size_t _colStart = 1;
    std::string _current;
    std::string _trailingSpace;
    std::string _leadingSpace;
    State _state = State::Default;
};

} // namespace

LineConsumer tokenize(TokenConsumer consumer) {
    return Tokenizer{consumer};
}
