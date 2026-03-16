#pragma once
#include <string>

namespace vic::util
{
    [[nodiscard]] bool fileToString(const std::string& path, std::string* out);
}
