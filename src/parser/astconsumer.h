#pragma once

#include "parser/ast.h"
#include <functional>

using AstConsumer = std::function<void(Ast)>;
