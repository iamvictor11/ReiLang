#pragma once
#include <string>
#include <memory>
#include "util/error.hpp"
#include "base/env.complie.hpp"
#include "base/env.runtime.hpp"

namespace rei
{
    struct CallFrame final
    {
        enum Tag
        {
            CFT_FUNC,
            CFT_CLOS
        };
        Tag tag;
        union
        {
            Ref<Function> func;
            Ref<Closure> clos;
        };
        Bytecode* save_ip;
        CallFrame(Ref<Function> f, Bytecode* sip) : tag(CFT_FUNC), func(f), save_ip(sip) {}
        CallFrame(Ref<Closure> c, Bytecode* sip) : tag(CFT_FUNC), clos(c), save_ip(sip) {}
        CallFrame(const CallFrame& other) : tag(other.tag)
        {
            switch (tag)
            {
            case CFT_FUNC: new (&func) Ref<Function>(other.func); break;
            case CFT_CLOS: new (&clos) Ref<Closure>(other.clos); break;
            }
        }
        CallFrame& operator=(const CallFrame& other)
        {
            if (this == &other) return *this;
            this->~CallFrame();
            new (this) CallFrame(other);
            return *this;
        }
        ~CallFrame()
        {
            switch (tag)
            {
            case CFT_FUNC:   func.~shared_ptr(); break;
            case CFT_CLOS: clos.~shared_ptr(); break;
            }
        }
    };
    class VM final
    {
    private:
        Chunk chunk_;
        Env<PS_COMPILE> env_c_;
        Env<PS_RUNTIME> env_r_;
        Bytecode* ip_;
        std::vector<Value::Data> stack_;
        std::vector<CallFrame> frames_;
        Error::Reporter error_reporter_;
    public:
        VM();
        ~VM() = default;
    public:
        Bytecode bind(const std::string& name, const Value::Data& val);
    private:
        void bindSTL_();
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
