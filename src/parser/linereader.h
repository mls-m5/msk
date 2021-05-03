#pragma once

#include "filesystem.h"
#include "parser/lineconsumer.h"

// This is where it all starts. This outputs to a "LineConsumer" ie a function
// that eats lines
void read(filesystem::path, LineConsumer);

void readString(std::string_view, LineConsumer);
