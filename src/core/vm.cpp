#include "vm.hpp"
#include "luna/config.hpp"
#include "util/file.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.RD.hpp"
#include <iostream>
#include "parser/visitor.hpp"
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
        auto& tokens = lexer.start();
        if (!_error_reporter.empty()) return;
    #ifdef LUNA_DEBUG_ENABLE
        for (const auto& token : tokens)
            std::cout << token.toString() << std::endl;
    #endif
        std::cout << "语法分析：" << std::endl;
        Parser<PT_RD> parser {std::move(tokens), &_error_reporter};
        auto nodeRef = parser.start();
        if (!_error_reporter.empty()) return;
    #ifdef LUNA_DEBUG_ENABLE
        luna::ast::Printer printer{};
        printer(nodeRef->tempRef());
        luna::ast::Evaluator evaluator{};
        std::cout << Value::toString(evaluator(nodeRef->tempRef())) << std::endl;
    #endif
    }
}
