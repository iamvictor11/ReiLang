#pragma once
#include <string>
#include <memory>
#include "util/error.hpp"
#include "parser/node.hpp"
#include "base/environment.hpp"

namespace luna
{
    class Lexer;
    class VM final
    {
    private:
        ast::NRef _program;
        Environment _env;
        Error::Reporter _error_reporter;
    public:
        VM() = default;
        ~VM() = default;
    public:
        void loadSimple(const std::string& source);
        void loadFile(const std::string& path);
    public:
        void run();
    public:
        bool hasError();
        Error::Msg popError();
    };
}
