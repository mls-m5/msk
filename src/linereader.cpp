#include "linereader.h"
#include <fstream>
#include <sstream>

void read(filesystem::path path, LineConsumer consumer) {
    auto file = std::ifstream{path};
    size_t row = 1;

    for (std::string line; std::getline(file, line); ++row) {
        consumer({std::move(line), row});
    }
}

void readString(std::string_view str, LineConsumer consumer) {
    auto ss = std::istringstream(std::string{str});
    size_t row = 1;

    for (std::string line; std::getline(ss, line); ++row) {
        consumer({std::move(line), row});
    }
}
