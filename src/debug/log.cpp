#include "log.hpp"
#include <iostream>
#include <cassert>

namespace vic::Log
{
    static void _PrintFileAndLine(const char *file, int line)
    {
        std::cout << "<path file=" << file << ", line=" << line << ">" << std::endl;
    }
    void Info(const std::string &message, const char *file, int line)
    {
        std::cout << "[Info] " << message << std::endl;
        _PrintFileAndLine(file, line);
    }
    void Warning(const std::string &message, const char *file, int line)
    {
        std::cout << "[Warning] " << message << std::endl;
        _PrintFileAndLine(file, line);
    }
    void Error(const std::string &message, const char *file, int line)
    {
        std::cout << "[Error] " << message << std::endl;
        _PrintFileAndLine(file, line);
    }
    void Assert(bool ok, const std::string &message, const char *file, int line)
    {
        if (ok) return;
        std::cout << "[Assert] " << message << std::endl;
        _PrintFileAndLine(file, line);
        assert(false);
    }
    void Success(const std::string &message, const char *file, int line)
    {
        std::cout << "[Success] " << message << std::endl;
        _PrintFileAndLine(file, line);
    }
}
