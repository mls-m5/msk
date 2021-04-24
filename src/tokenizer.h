#pragma once

#include "lineconsumer.h"
#include "tokenconsumer.h"

//! Create a function that consumes lines and calls a function that consumes
//! tokens
LineConsumer tokenize(TokenConsumer);
