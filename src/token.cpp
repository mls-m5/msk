#include "token.h"
#include <string_view>
#include <vector>

namespace {

auto createVector() {
    auto v = std::vector<std::string_view>(Token::Last);

    v[Token::Word] = "word";
    v[Token::Let] = "let";
    v[Token::Return] = "return";

    v[Token::Operator] = "operator";
    v[Token::Function] = "function";
    v[Token::Semicolon] = "semicolon";
    v[Token::Coma] = "coma";
    v[Token::NumericLiteral] = "numeric";
    v[Token::FunctionDefinition] = "func def";
    v[Token::VariableDeclaration] = "var decl";
    v[Token::BraceGroup] = "brace group";
    v[Token::ParenGroup] = "paren group";
    v[Token::BracketGroup] = "bracket group";
    v[Token::ImportStatement] = "import statement";
    v[Token::Import] = "import";

    return v;
}

const auto map = createVector();

} // namespace

std::string_view typeName(Token::Type type) {
    return map.at(type);
}
