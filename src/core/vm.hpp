#pragma once
#include <string>
#include <memory>
#include "util/error.hpp"
#include "base/env.hpp"

namespace vic
{
    class Lexer;
    class VM final
    {
    private:
        Chunk _chunk;
        Env _env;
        Bytecode* _ip;
        std::vector<Value::Data> _stack;
        Error::Reporter _error_reporter;
    public:
        VM() = default;
        ~VM() = default;
    public:
        void loadSimple(const std::string& source);
        void loadFile(const std::string& path);
    public:
        void run();
    private:
        Bytecode _readByte();
        Value::Data _readConstant();
        void _push(Value::Data value);
        Value::Data _pop();
        Value::Data _peek();
        Value::Data _peek(int distance);
    public:
        bool hasError();
        Error::Msg popError();
    };
}
