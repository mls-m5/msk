#pragma once

#include "parser/token.h"
#include <functional>

using TokenConsumer = std::function<void(Token)>;
