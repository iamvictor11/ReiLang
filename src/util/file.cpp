#include "file.hpp"
#include <fstream>
#include <sstream>
#include "debug/log.hpp"

namespace vic::util
{
    bool fileToString(const std::string& path, std::string* out)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            VIC_DEBUG_LOG_ASSERT(false, "文件 {} 打不开 或 不存在", path.c_str());
            return false;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        *out = buffer.str();
        return true;
    }
}
