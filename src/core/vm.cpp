#include "vm.hpp"
#include "util/file.hpp"

namespace luna
{
    void VM::runSimple(const std::string& source)
    {
        auto s = util::fileToString(source);
    }
    void VM::runFile(const std::string& path)
    {
        auto s = util::fileToString(path);
    }
}
