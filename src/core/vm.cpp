#include "vm.hpp"
#include "util/file.hpp"
#include "lexer.hpp"
#include <iostream>

namespace luna
{
    VM::VM() : _lexer(std::make_unique<Lexer>()) {}
    VM::~VM() = default;
    void VM::runSimple(const std::string& source)
    {
        auto s = util::fileToString(source);
    }
    void VM::runFile(const std::string& path)
    {
        const auto& tokens = _lexer->tokenize(util::fileToString(path), &_error_reporter);
        for (const auto& token : tokens)
            std::cout << token.toString() << std::endl;
    }
}
