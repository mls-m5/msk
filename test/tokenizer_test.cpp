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

TEST_SUIT_END
