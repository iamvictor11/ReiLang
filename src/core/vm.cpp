#include "vm.hpp"
#include "util/file.hpp"
#include "lexer.hpp"
#include <iostream>

namespace luna
{
    void VM::runSimple(const std::string& source)
    {
        auto s = util::fileToString(source);
    }
    void VM::runFile(const std::string& path)
    {
        Lexer lexer {util::fileToString(path), &_error_reporter};
        const auto& tokens = lexer.start();
        // for (const auto& token : tokens)
        //     std::cout << token.toString() << std::endl;
    }
}
