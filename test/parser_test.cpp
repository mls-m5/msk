#include "grouper.h"
#include "linereader.h"
#include "mls-unit-test/unittest.h"
#include "parser.h"
#include "tokenizer.h"
#include <string_view>

using namespace std::literals;

namespace {

void consumeString(std::string_view str, AstConsumer consumer) {
    readString(str, tokenize(grouper(parser(consumer))));
}

} // namespace

TEST_SUIT_BEGIN

TEST_CASE("variable declaration") {
    auto consumer = [](Ast ast) {
        std::cout << ast << "\n";
        EXPECT_FALSE(ast.children.empty());
        EXPECT_EQ(ast.children.front().token.type, Token::VariableDeclaration);
    };

    const auto str = std::string_view{R"_(
 let x = 10;
)_"sv};

    consumeString(str, consumer);
}

TEST_CASE("function declaration") {
    auto consumer = [](Ast ast) {
        std::cout << ast << "\n";
        EXPECT_FALSE(ast.children.empty());
        auto &first = ast.children.front();
        EXPECT_EQ(first.token.type, Token::Function);
        EXPECT_FALSE(first.children.empty());
        EXPECT_TRUE(first.find(Token::FunctionArguments) !=
                    first.children.end());
    };

    const auto str = std::string_view{R"_(
 func apa() { let x = 10; }
)_"sv};

    consumeString(str, consumer);
}

TEST_CASE("function definition") {
    auto consumer = [](Ast ast) {
        std::cout << ast << "\n";
        EXPECT_FALSE(ast.children.empty());
        EXPECT_EQ(ast.children.front().token.type, Token::FunctionDefinition);
    };

    const auto str = std::string_view{R"_(
 func apa(x: int, y: int);
)_"sv};

    consumeString(str, consumer);
}

TEST_CASE("import statement") {
    auto consumer = [](Ast ast) {
        std::cout << ast << "\n";
        EXPECT_FALSE(ast.children.empty());
        EXPECT_EQ(ast.children.front().token.type, Token::ImportStatement);
    };

    const auto str = std::string_view{R"_(
 import apa;
)_"sv};

    consumeString(str, consumer);
}

TEST_SUIT_END
