#include "vm.hpp"
#include "luna/config.hpp"
#include "util/file.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.RD.hpp"
#include <iostream>
#include "parser/visitor.hpp"
namespace luna
{
    void VM::loadSimple(const std::string& source)
    {
        auto s = util::fileToString(source);
    }
    void VM::loadFile(const std::string& path)
    {
        Lexer lexer {util::fileToString(path), &_error_reporter};
        auto& tokens = lexer.start();
        if (!_error_reporter.empty()) return;
    #ifdef LUNA_DEBUG_ENABLE
        std::cout << "词法分析：" << std::endl;
        for (const auto& token : tokens)
            std::cout << token.toString() << std::endl;
    #endif
        Parser<PT_RD> parser {std::move(tokens), &_error_reporter};
        _program = parser.start();
        if (!_error_reporter.empty()) return;
    #ifdef LUNA_DEBUG_ENABLE
        std::cout << "语法分析：" << std::endl;
        luna::ast::Printer printer{};
        printer(_program->tempRef());
    #endif
    }

    void VM::run()
    {
        luna::ast::Evaluator evaluator{};
        evaluator(_program->tempRef());
    }

    bool VM::hasError()
    {
        return _error_reporter.empty();
    }
    Error::Msg VM::popError()
    {
        return _error_reporter.pop();
    }
}
