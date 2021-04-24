
#define DO_NOT_CATCH_ERRORS

#include "filesystem.h"
#include "grouper.h"
#include "mls-unit-test/unittest.h"

TEST_SUIT_BEGIN

TEST_CASE("basic parenthesis") {
    size_t count = 0;
    Ast result;
    auto consumer = [&](Ast ast) {
        std::cout << ast.token.content << ast.end.content << std::endl;
        result = std::move(ast);
        ++count;
    };

    auto f = grouper(consumer);

    f({
        .content = "(",
        .type = Token::ParenBegin,
    });

    f({
        .content = ")",
        .type = Token::ParenEnd,
    });

    EXPECT_EQ(count, 1);
    EXPECT_EQ(result.token.type, Token::ParenGroup);
}

TEST_CASE("basic braces") {
    size_t count = 0;
    Ast result;
    auto consumer = [&](Ast ast) {
        std::cout << ast.token.content << ast.end.content << std::endl;
        result = std::move(ast);
        ++count;
    };

    auto f = grouper(consumer);

    f({
        .content = "{",
        .type = Token::BraceBegin,
    });

    f({
        .content = "}",
        .type = Token::BraceEnd,
    });

    EXPECT_EQ(count, 1);
    EXPECT_EQ(result.token.type, Token::BraceGroup);
}

TEST_CASE("basic brackets") {
    size_t count = 0;
    Ast result;
    auto consumer = [&](Ast ast) {
        std::cout << ast.token.content << ast.end.content << std::endl;
        result = std::move(ast);
        ++count;
    };

    auto f = grouper(consumer);

    f({
        .content = "[",
        .type = Token::BracketBegin,
    });

    f({
        .content = "}",
        .type = Token::BracketEnd,
    });

    EXPECT_EQ(count, 1);
    EXPECT_EQ(result.token.type, Token::BracketGroup);
}

TEST_CASE("nested braces") {
    size_t count = 0;
    Ast result;
    auto consumer = [&](Ast ast) {
        std::cout << ast.token.content << ast.end.content << std::endl;
        result = std::move(ast);
        ++count;
    };

    auto f = grouper(consumer);

    f({
        .content = "{",
        .type = Token::BraceBegin,
    });

    f({
        .content = "{",
        .type = Token::BraceBegin,
    });

    f({
        .content = "}",
        .type = Token::BraceEnd,
    });
    f({
        .content = "}",
        .type = Token::BraceEnd,
    });

    EXPECT_EQ(count, 1);
    EXPECT_EQ(result.token.type, Token::BraceGroup);
}

TEST_SUIT_END