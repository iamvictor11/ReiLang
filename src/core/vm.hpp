#pragma once
#include <string>
#include "util/error.hpp"
#include "lexer.hpp"

namespace luna
{
    class VirtualMachine final
    {
    private:
        Error::Reporter _error_reporter;
        Lexer lexer;
    public:
        VirtualMachine() = default;
        ~VirtualMachine() = default;
    public:
        void runSimple(const std::string& source);
        void runFile(const std::string& path);
    };

    using VM = VirtualMachine;
}
