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
        void _jump(Bytecode offset);
    private:
        void _push(Value::Data value);
        Value::Data _pop();
        Value::Data _peek();
        Value::Data _peek(int distance);
    public:
        bool hasError();
        Error::Msg popError();
    };
}
