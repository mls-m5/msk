#pragma once

#include "line.h"
#include <functional>

using LineConsumer = std::function<void(Line)>;
