#include "log.hpp"
#include <iostream>
#include <cassert>

namespace rei::Log
{
    static void printFileAndLine_(const char *file, int line)
    {
        std::cout << "<path file=" << file << ", line=" << line << ">" << std::endl;
    }
    void echoInfo(const std::string &message, const char *file, int line)
    {
        std::cout << "[Info] " << message << std::endl;
        printFileAndLine_(file, line);
    }
    void echoWarning(const std::string &message, const char *file, int line)
    {
        std::cout << "[Warning] " << message << std::endl;
        printFileAndLine_(file, line);
    }
    void echoError(const std::string &message, const char *file, int line)
    {
        std::cout << "[Error] " << message << std::endl;
        printFileAndLine_(file, line);
    }
    void echoAssert(bool ok, const std::string &message, const char *file, int line)
    {
        if (ok) return;
        std::cout << "[Assert] " << message << std::endl;
        printFileAndLine_(file, line);
        assert(false);
    }
    void echoSuccess(const std::string &message, const char *file, int line)
    {
        std::cout << "[Success] " << message << std::endl;
        printFileAndLine_(file, line);
    }
}
