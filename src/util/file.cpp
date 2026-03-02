#include "file.hpp"
#include <fstream>
#include <sstream>
#include "debug/log.hpp"

namespace luna::util
{
    std::string fileToString(const std::string& path)
    {
        std::ifstream file(path);
        LUNA_DEBUG_LOG_ASSERT(file.is_open(), "文件 {} 打不开", path.c_str());
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}
