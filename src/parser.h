#pragma once

#include "astconsumer.h"

//! Returns a function that consumes grouped ast-nodes and pass on to function
//! given in arguments
AstConsumer parser(AstConsumer);
