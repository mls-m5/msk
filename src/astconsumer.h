#pragma once

#include "ast.h"
#include <functional>

using AstConsumer = std::function<void(Ast)>;
