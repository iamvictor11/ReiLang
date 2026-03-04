#include "vm.hpp"
#include "luna/config.hpp"
#include "util/file.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.RD.hpp"
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
        std::cout << "词法分析：" << std::endl;
        if (!_error_reporter.empty()) return;
    #ifdef LUNA_DEBUG_ENABLE
        auto& tokens = lexer.start();
        for (const auto& token : tokens)
            std::cout << token.toString() << std::endl;
    #endif
        std::cout << "语法分析：" << std::endl;
        Parser<PT_RD> parser {std::move(tokens), &_error_reporter};
        parser.start();
        if (!_error_reporter.empty()) return;
    }
}
