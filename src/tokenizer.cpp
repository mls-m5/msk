// Really messy tokenizer.. consider rewriting sometime

#include "tokenizer.h"
#include <string_view>
#include <vector>

namespace {

auto keywordMap = std::vector<std::pair<std::string_view, Token::Type>>{
    {"func", Token::Func},
    {"var", Token::Var},
    {"let", Token::Let},
    {"const", Token::Const},
};

bool isAlpha(char c) {
    return isalpha(c) || c == '_';
}

bool isAlphaNum(char c) {
    return isalnum(c) || c == '_';
}

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
        Word,
        Operator,
        Space,
        Numeric,
    };

    Tokenizer(TokenConsumer consumer)
        : _consumer(consumer) {}
    Tokenizer(const Tokenizer &) = default;
    Tokenizer &operator=(const Tokenizer &) = default;
    Tokenizer(Tokenizer &&) = default;
    Tokenizer &operator=(Tokenizer &&) = default;

    inline void wordState(char c) {
        if (isAlphaNum(c)) {
            _content += c;
        }
        else {
            sendToken();

            defaultState(c);
        }
    }

    //! Entry state for all tokens
    inline void defaultState(char c) {
        if (isAlpha(c)) {
            startNewToken(c, State::Word);
            return;
        }

        switch (c) {
        case '{':
            sendWithChar(c, Token::BraceBegin);
            break;
        case '}':
            sendWithChar(c, Token::BraceEnd);
            break;
        case '(':
            sendWithChar(c, Token::ParenBegin);
            break;
        case ')':
            sendWithChar(c, Token::ParenEnd);
            break;
        case '[':
            sendWithChar(c, Token::BracketBegin);
            break;
        case ']':
            sendWithChar(c, Token::BracketEnd);
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

        case ';':
            sendWithChar(c, Token::Semicolon);
            break;

        case ',':
            sendWithChar(c, Token::Coma);
            break;

        case ' ':
        case '\r':
        case '\n':
        case '\t':
            _leadingSpace += c;
            startNewToken(0, State::Space);
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            _content += c;
            _state = State::Numeric;

            break;
        default:
            _content += c;
            break;
        }
    }

    inline void operatorState(char c) {
        // Handle multiple character operators in the future

        startNewToken(c);
    }

    inline void spaceState(char c) {
        if (isspace(c)) {
            if (_content.empty()) {
                _leadingSpace += c;
            }
            else {
                _trailingSpace += c;
            }
        }
        else {
            if (_content.empty()) {
                _state = State::Default;
                handleCharacter(c);
            }
            else {
                sendToken();
                startNewToken(c);
            }
        }
    }

    inline void numericState(char c) {
        if (isdigit(c) || c == '.') {
            _content += c;
        }
        else {
            startNewToken(c);
        }
    }

    Token::Type getType() {
        switch (_state) {
        case State::Operator:
            return Token::Operator;
        case State::Space:
            return Token::None; // Should not happend
        case State::Numeric:
            return Token::NumericLiteral;
        case State::Word:
            break;
        case State::Default:
            break;
        }
        if (auto k = getKeyword(_content); k == Token::None) {
            return Token::Word;
        }
        else {
            return k;
        }
    }

    void sendWithChar(char c, Token::Type type) {
        _content += c;
        sendToken(type);
        ;
    }

    void sendToken(Token::Type type = Token::None) {
        if (_content.empty()) {
            return;
        }
        type = (type == Token::None) ? getType() : type;
        _consumer(Token{
            .content = std::move(_content),
            .leadingSpace = std::move(_leadingSpace),
            .trailingSpace = std::move(_trailingSpace),
            .row = _row,
            .col = _colStart,
            .type = type,
        });

        _colStart = _col + 1;
        _state = State::Default;

        _leadingSpace.clear();
        _trailingSpace.clear();
        _content.clear();
    }

    //! Send current token and begin new state
    void startNewToken(char c, State state = State::Default) {
        sendToken();
        _state = state;
        if (c) {
            if (state == State::Default) {
                handleCharacter(c);
            }
            else {
                _content += c;
            }
        }
    }

    void handleCharacter(char c) {
        switch (_state) {
        case State::Default:
            defaultState(c);
            break;
        case State::Word:
            wordState(c);
            break;
        case State::Operator:
            operatorState(c);
            break;
        case State::Space:
            spaceState(c);
            break;
        case State::Numeric:
            numericState(c);
        }
    }

    // The enty point
    void operator()(Line line) {
        _colStart = _col = 1;
        _row = line.row;
        _content.clear();

        for (auto c : line.content) {
            handleCharacter(c);
            ++_col;
        }

        sendToken();
    }

private:
    TokenConsumer _consumer;

    size_t _row = 0;
    size_t _col = 1;
    size_t _colStart = 1;
    std::string _content;
    std::string _trailingSpace;
    std::string _leadingSpace;
    State _state = State::Default;
};

} // namespace

LineConsumer tokenize(TokenConsumer consumer) {
    return Tokenizer{std::move(consumer)};
}
