#pragma once

#include "filesystem.h"
#include "lineconsumer.h"

// This is where it all starts. This outputs to a "LineConsumer" ie a function
// that eats lines
void read(filesystem::path, LineConsumer);
