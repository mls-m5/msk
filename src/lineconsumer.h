#pragma once

#include <functional>
#include <string>

struct Line {
    std::string content;
    size_t row = 0;
};

using LineConsumer = std::function<void(Line)>;
