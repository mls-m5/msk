#pragma once

#include <string>

//! Just keep track of which line it is and what was on it
struct Line {
    std::string content;
    size_t row = 0;
    //! Todo: Add pointer to file name
};
