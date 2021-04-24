#include "linereader.h"
#include <fstream>

void read(filesystem::path path, LineConsumer consumer) {
    auto file = std::ifstream{path};
    size_t row = 1;

    for (std::string line; std::getline(file, line); ++row) {
        consumer({std::move(line), row});
    }
}
