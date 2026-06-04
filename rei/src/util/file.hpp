#pragma once
#include <string>

namespace rei::util
{
    [[nodiscard]] bool fileToString(const std::string& path, std::string* out);
}
