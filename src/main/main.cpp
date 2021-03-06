#include "filesystem.h"
#include "parser/grouper.h"
#include "parser/linereader.h"
#include "parser/parser.h"
#include "parser/tokenizer.h"
#include <iostream>
#include <string>
#include <vector>

struct Settings {
    enum Operation {
        Tokenize,
        Group,
        PrintAst,
    };

    Operation operation;

    std::vector<filesystem::path> inputFiles;

    Settings(int argc, char **argv) {
        auto args = std::vector<std::string_view>{argv + 1, argv + argc};

        for (size_t i = 0; i < args.size(); ++i) {
            auto arg = args.at(i);

            if (arg == "--tokenize") {
                operation = Operation::Tokenize;
            }
            else if (arg == "--group") {
                operation = Operation::Group;
            }
            else if (arg == "--print-ast") {
                operation = Operation::PrintAst;
            }
            else {
                inputFiles.push_back(arg);
            }
        }
    }
};

int main(int argc, char **argv) {
    const auto settings = Settings{argc, argv};

    switch (settings.operation) {
    case Settings::Tokenize: {
        auto f = [](Token token) {
            std::cout << token.content << "\t";
            std::cout << token.type << "\n";
        };
        read(settings.inputFiles.front(), tokenize(f));
        break;
    }
    case Settings::Group: {
        auto f = [](Ast ast) { std::cout << ast << std::endl; };
        read(settings.inputFiles.front(), tokenize(grouper(f)));
        break;
    }
    case Settings::PrintAst: {
        auto f = [](Ast ast) { std::cout << ast << std::endl; };
        read(settings.inputFiles.front(), tokenize(grouper(parser(f))));
        break;
    }
    }
}
