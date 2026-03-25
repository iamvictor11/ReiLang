#pragma once
#include <string>
#include <memory>
#include "util/error.hpp"
#include "base/env.complie.hpp"
#include "base/env.runtime.hpp"
#include "vvmidi/vvmidi.hpp"

namespace rei
{
    struct CallFrame final
    {
    public:
        Ref<Function> func_ref;
        Bytecode* save_ip;
    public:
        CallFrame(Ref<Function> f, Bytecode* sip) : func_ref(f), save_ip(sip) {}
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
        struct
        {
            vvmidi::MidiOut out {};
            vvmidi::Channel::Index channel = vvmidi::Channel::One;
            uint8_t velocity = 0x7F;
            vvmidi::Tick::Val bpm = vvmidi::Tick::BPM;
        } midi_;
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
        Value::Data& peek_();
        Value::Data& peek_(int distance);
    public:
        bool hasError();
        Error::Msg popError();
    };
}
