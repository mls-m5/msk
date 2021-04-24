#define DO_NOT_CATCH_ERRORS

#include "filesystem.h"
#include "mls-unit-test/unittest.h"
#include "tokenizer.h"

TEST_SUIT_BEGIN

TEST_CASE("read test file") {
    auto count = 0;
    auto consumer = [&count](Token token) {
        std::cout << token.content << "\n";
        ++count;
    };

    auto f = tokenize(consumer);

    f({"hello there"});
    EXPECT_EQ(count, 2);

    count = 0;
    f({"hello+there"});
    EXPECT_EQ(count, 3);
}

TEST_CASE("semicolon") {
    {
        auto consumer = [](Token token) { EXPECT_EQ(token.content, ";"); };

        auto f = tokenize(consumer);
        f({";"});
    }

    {
        size_t count = 0;
        auto consumer = [&](Token token) {
            std::cout << token.content << "\n";
            if (count == 1) {
                EXPECT_EQ(token.content, ";");
            }
            ++count;
        };

        auto f = tokenize(consumer);
        f({"x;"});

        // Test1
        count = 0;
        f({"};"});
    }
}

TEST_CASE("numeric literal") {
    {
        auto consumer = [](Token token) {
            EXPECT_EQ(token.content, "0.32");
            EXPECT_EQ(token.type, Token::NumericLiteral);
        };

        auto f = tokenize(consumer);
        f({"0.32"});
    }
}

TEST_SUIT_END
