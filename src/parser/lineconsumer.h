#pragma once

#include "parser/line.h"
#include <functional>

using LineConsumer = std::function<void(Line)>;
