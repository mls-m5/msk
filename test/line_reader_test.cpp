#include "linereader.h"
#include "mls-unit-test/unittest.h"
#include <fstream>

TEST_SUIT_BEGIN

TEST_CASE("read test file") {
    const auto filename = filesystem::path{"line_reader_test.txt"};
    std::ofstream{filename} << "Hello\n"
                               "there\n"
                               "you\n";

    size_t count = 0;

    auto consume = [&count](Line line) {
        std::cout << line.content << "\n";
        ++count;
    };

    read(filename, consume);

    EXPECT_EQ(count, 3);
}

TEST_SUIT_END
