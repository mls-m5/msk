#include "ast.h"
#include <iomanip>

namespace {

void print(std::ostream &stream, const Ast &ast, size_t depth) {
    auto indent = [&stream, depth ]() -> auto & {
        for (size_t i = 0; i < depth; ++i) {
            stream << "  ";
        }
        return stream;
    };

    indent() << ast.token.content << "\t" << ast.token.type << "\n";

    for (auto &c : ast.children) {
        print(stream, c, depth + 1);
    }

    if (!ast.end.content.empty()) {
        indent() << ast.end.content << "\t" << ast.end.type << "\t(end group)"
                 << "\n";
    }
}

} // namespace

std::ostream &operator<<(std::ostream &stream, const Ast &ast) {
    print(stream, ast, 0);
    return stream;
}
