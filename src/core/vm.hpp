#pragma once
#include <string>
#include <memory>
#include "util/error.hpp"
#include "base/env.hpp"

namespace rei
{
    struct Closure final
    {
        Ref<Function> func;
        // std::vector<Upvalue> upvalues
    };
    
    struct CallFrame final
    {
        Closure closure;
        Bytecode* save_ip;
        Bytecode* save_end;
    };
    
    class VM final
    {
    private:
        Chunk _chunk;
        Env _env;
        Bytecode* _ip;
        Bytecode* _end;
        std::vector<Value::Data> _stack;
        std::vector<CallFrame> _frames;
        Error::Reporter _error_reporter;
    public:
        VM();
        ~VM() = default;
    public:
        Value::Coord bind(const std::string& name, const Value::Data& val);
    public:
        void loadSimple(const std::string& source);
        void loadFile(const std::string& path);
    public:
        void run();
    private:
        Bytecode readByte_();
        Value::Data readConstant_();
        void jump_(Bytecode offset);
    private:
        void push_(Value::Data value);
        Value::Data pop_();
        Value::Data peek_();
        Value::Data peek_(int distance);
    public:
        bool hasError();
        Error::Msg popError();
    };
}
