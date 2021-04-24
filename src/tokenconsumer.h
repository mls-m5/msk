#pragma once

#include "token.h"
#include <functional>

using TokenConsumer = std::function<void(Token)>;
