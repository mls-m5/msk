#pragma once

#include "astconsumer.h"
#include "tokenconsumer.h"

//! Group ast based on paranthesis braces and brackets
TokenConsumer grouper(AstConsumer);
