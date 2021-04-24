#include "filesystem.h"
#include <iostream>
#include <linereader.h>
#include <string>
#include <tokenizer.h>
#include <vector>

struct Settings {
    enum Operation {
        Tokenize,
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
            else {
                inputFiles.push_back(arg);
            }
        }
    }
};

int main(int argc, char **argv) {
    const auto settings = Settings{argc, argv};

    if (settings.operation == Settings::Tokenize) {
        auto f = [](Token token) { std::cout << token.content << "\n"; };
        read(settings.inputFiles.front(), tokenize(f));
    }
}
