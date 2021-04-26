#include "token.h"
#include <string_view>
#include <vector>

namespace {

auto createVector() {
    auto v = std::vector<std::string_view>(Token::Last);

    v[Token::Word] = "word";
    v[Token::Let] = "let";
    v[Token::Return] = "return";

    v[Token::Name] = "name";

    v[Token::StringLiteral] = "string";
    v[Token::Operator] = "operator";
    v[Token::Function] = "function";
    v[Token::Semicolon] = "semicolon";
    v[Token::Coma] = "coma";
    v[Token::NumericLiteral] = "numeric";
    v[Token::FunctionDefinition] = "func def";
    v[Token::FunctionArguments] = "func args";
    v[Token::FunctionBody] = "func body";
    v[Token::VariableDeclaration] = "var decl";
    v[Token::BraceGroup] = "brace group";
    v[Token::ParenGroup] = "paren group";
    v[Token::BracketGroup] = "bracket group";
    v[Token::ImportStatement] = "import statement";
    v[Token::ExportStatement] = "export statement";
    v[Token::Import] = "import";
    v[Token::Export] = "export";

    v[Token::TranslationUnit] = "translation unit";

    return v;
}

const auto map = createVector();

} // namespace

std::string_view typeName(Token::Type type) {
    return map.at(type);
}
