#pragma once

#include "parser/astconsumer.h"
#include "parser/tokenconsumer.h"

//! Group ast based on paranthesis braces and brackets
TokenConsumer grouper(AstConsumer);
