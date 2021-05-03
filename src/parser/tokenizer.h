#pragma once

#include "parser/lineconsumer.h"
#include "parser/tokenconsumer.h"

//! Create a function that consumes lines and calls a function that consumes
//! tokens
LineConsumer tokenize(TokenConsumer);
