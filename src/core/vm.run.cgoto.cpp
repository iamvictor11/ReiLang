#include "rei/config.hpp"
#if REI_COMPUTED_GOTO_OPT == 1 && defined(__GNUC__) && !defined(__clang__)
#include "vm.hpp"
#include "vm.expr.hpp"
#include <iostream>
#include <format>
#include <thread>
#include <chrono>

using namespace vvmidi;

namespace rei
{
    void VM::run()
    {
    #if REI_DEBUG_ENABLE >= 1
        std::cout << "\033[1m\033[38;2;255;105;180m运行结果：\033[0m" << std::endl; 
    #endif
        if (!error_reporter_.empty()) return;
        ip_ = chunk_.codes.data();
        #define REI_LABEL(op) REI_LABEL_##op
        static const void* dispatch_table[] =
        {
            &&REI_LABEL(OP_CONSTANT),
            &&REI_LABEL(OP_CLONE),
            &&REI_LABEL(OP_POP),
            &&REI_LABEL(OP_KEEP),
            &&REI_LABEL(OP_NIL),
            &&REI_LABEL(OP_TRUE),
            &&REI_LABEL(OP_FALSE),
            &&REI_LABEL(OP_NEG),
            &&REI_LABEL(OP_BIT_NOT),
            &&REI_LABEL(OP_NOT),
            &&REI_LABEL(OP_ADD),
            &&REI_LABEL(OP_SUB),
            &&REI_LABEL(OP_MUL),
            &&REI_LABEL(OP_DIV),
            &&REI_LABEL(OP_MOD),
            &&REI_LABEL(OP_POW),
            &&REI_LABEL(OP_BIT_AND),
            &&REI_LABEL(OP_BIT_OR),
            &&REI_LABEL(OP_BIT_XOR),
            &&REI_LABEL(OP_BIT_XNOR),
            &&REI_LABEL(OP_BIT_SHL),
            &&REI_LABEL(OP_BIT_SHR),
            &&REI_LABEL(OP_EQ),
            &&REI_LABEL(OP_NE),
            &&REI_LABEL(OP_LT),
            &&REI_LABEL(OP_LE),
            &&REI_LABEL(OP_GT),
            &&REI_LABEL(OP_GE),
            &&REI_LABEL(OP_AND),
            &&REI_LABEL(OP_OR),
            &&REI_LABEL(OP_INIT_ARRAY),
            &&REI_LABEL(OP_ENTER),
            &&REI_LABEL(OP_EXIT),
            &&REI_LABEL(OP_PRINT),
            &&REI_LABEL(OP_PRINTLN),
            &&REI_LABEL(OP_JUMP),
            &&REI_LABEL(OP_JMPT),
            &&REI_LABEL(OP_JMPF),
            &&REI_LABEL(OP_HOST_GET),
            &&REI_LABEL(OP_HOST_SET),
            &&REI_LABEL(OP_HOST_SSET),
            &&REI_LABEL(OP_GLOBAL_DEF),
            &&REI_LABEL(OP_GLOBAL_GET),
            &&REI_LABEL(OP_GLOBAL_SET),
            &&REI_LABEL(OP_GLOBAL_SSET),
            &&REI_LABEL(OP_LOCAL_DEF),
            &&REI_LABEL(OP_LOCAL_GET),
            &&REI_LABEL(OP_LOCAL_SET),
            &&REI_LABEL(OP_LOCAL_SSET),
            &&REI_LABEL(OP_ONCE_GET),
            &&REI_LABEL(OP_ONCE_SET),
            &&REI_LABEL(OP_ONCE_SSET),
            &&REI_LABEL(OP_CALL),
            &&REI_LABEL(OP_RETURN),
            &&REI_LABEL(OP_INDEX_GET),
            &&REI_LABEL(OP_INDEX_SET),
            &&REI_LABEL(OP_INDEX_SSET),
            &&REI_LABEL(OP_BPM),
            &&REI_LABEL(OP_BEAT),
            &&REI_LABEL(OP_CHANNEL),
            &&REI_LABEL(OP_PROGRAM),
            &&REI_LABEL(OP_VOLUME),
            &&REI_LABEL(OP_VELOCITY),
            &&REI_LABEL(OP_PLAY),
            &&REI_LABEL(OP_UNPLAY),
            &&REI_LABEL(OP_WAIT),
            &&REI_LABEL(OP_HALT)
        };
        #define REI_DISPATCH goto *dispatch_table[readByte_()]
        #define REI_IP ip_[-1]
        REI_DISPATCH;
        REI_LABEL(OP_CONSTANT):
        {
            push_(readConstant_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_CLONE):
        {
            push_(pop_().clone());
            REI_DISPATCH;
        }
        REI_LABEL(OP_POP):
        {
            pop_();
            REI_DISPATCH;
        }
        REI_LABEL(OP_KEEP):
        {
            push_(static_cast<Integer>(readByte_()));
            REI_DISPATCH;
        }
        REI_LABEL(OP_NIL):
        {
            push_(Nil{});
            REI_DISPATCH;
        }
        REI_LABEL(OP_TRUE):
        {
            push_(Boolean(true));
            REI_DISPATCH;
        }
        REI_LABEL(OP_FALSE):
        {
            push_(Boolean(false));
            REI_DISPATCH;
        }
        REI_LABEL(OP_NEG):
        REI_LABEL(OP_BIT_NOT):
        REI_LABEL(OP_NOT):
        {
        {
            Value::Data tempVal = pop_();
            push_(dispatchUnary_(tempVal, static_cast<Opcode>(REI_IP)));
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_ADD):
        REI_LABEL(OP_SUB):
        REI_LABEL(OP_MUL):
        REI_LABEL(OP_DIV):
        REI_LABEL(OP_MOD):
        REI_LABEL(OP_POW):
        REI_LABEL(OP_BIT_AND):
        REI_LABEL(OP_BIT_OR):
        REI_LABEL(OP_BIT_XOR):
        REI_LABEL(OP_BIT_XNOR):
        REI_LABEL(OP_BIT_SHL):
        REI_LABEL(OP_BIT_SHR):
        REI_LABEL(OP_EQ):
        REI_LABEL(OP_NE):
        REI_LABEL(OP_LT):
        REI_LABEL(OP_LE):
        REI_LABEL(OP_GT):
        REI_LABEL(OP_GE):
        {
        {
            Value::Data tempR = pop_();
            Value::Data tempL = pop_();
            push_(dispatchBinary_(tempL, tempR, static_cast<Opcode>(REI_IP)));
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_AND):
        {
        {
            Bytecode offset = readByte_();
            if (!(pop_().toBoolean()))
            {
                jump_(offset);
                push_(false);
            }
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_OR):
        {
        {
            Bytecode offset = readByte_();
            if (pop_().toBoolean())
            {
                jump_(offset);
                push_(true);
            }
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_INIT_ARRAY):
        {
        {
            Bytecode elemc = readByte_();
            auto array_ref = std::make_shared<Array>(elemc);
            for (size_t i = 0; i < elemc; i++)
                array_ref->data[i] = (peek_(elemc - 1 - i));
            for (size_t i = 0; i < elemc; i++)
                pop_();
            push_(array_ref);
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_ENTER):
        {
            env_r_.enter();
            REI_DISPATCH;
        }
        REI_LABEL(OP_EXIT):
        {
            env_r_.exit();
            REI_DISPATCH;
        }
        REI_LABEL(OP_PRINT):
        {
            std::cout << pop_().toString();
            REI_DISPATCH;
        }
        REI_LABEL(OP_PRINTLN):
        {
            std::cout << pop_().toString() << std::endl;
            REI_DISPATCH;
        }
        REI_LABEL(OP_JUMP):
        {
            jump_(readByte_());
            REI_DISPATCH;
        }
        
        REI_LABEL(OP_JMPT):
        {
        {
            Bytecode offset = readByte_();
            if (pop_().toBoolean())
                jump_(offset);
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_JMPF):
        {
        {
            Bytecode offset = readByte_();
            if (!(pop_().toBoolean()))
                jump_(offset);
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_HOST_GET):
        {
            push_(env_r_.getHost(readByte_()));
            REI_DISPATCH;
        }
        REI_LABEL(OP_HOST_SET):
        {
            env_r_.setHost(readByte_(), peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_HOST_SSET):
        {
        {
            Bytecode slot = readByte_();
            auto op = static_cast<Opcode>(pop_().asInteger());
            auto value = dispatchBinary_(env_r_.getHost(slot), pop_(), op);
            env_r_.setHost(slot, value);
            push_(value);
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_GLOBAL_DEF):
        {
            env_r_.defGlobal(peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_GLOBAL_GET):
        {
            push_(env_r_.getGlobal(readByte_()));
            REI_DISPATCH;
        }
        REI_LABEL(OP_GLOBAL_SET):
        {
            env_r_.setGlobal(readByte_(), peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_GLOBAL_SSET):
        {
        {
            Bytecode slot = readByte_();
            auto op = static_cast<Opcode>(pop_().asInteger());
            auto value = dispatchBinary_(env_r_.getGlobal(slot), pop_(), op);
            env_r_.setGlobal(slot, value);
            push_(value);
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_LOCAL_DEF):
        {
            env_r_.defLocal(peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_LOCAL_GET):
        {
        {
            Bytecode uplevel = readByte_();
            Bytecode slot = readByte_();
            push_(env_r_.getLocal(uplevel, slot));
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_LOCAL_SET):
        {
        {
            Bytecode uplevel = readByte_();
            Bytecode slot = readByte_();
            env_r_.setLocal(uplevel, slot, peek_());
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_LOCAL_SSET):
        {
        {
            Bytecode uplevel = readByte_();
            Bytecode slot = readByte_();
            auto op = static_cast<Opcode>(pop_().asInteger());
            auto value = dispatchBinary_(env_r_.getLocal(uplevel, slot), pop_(), op);
            env_r_.setLocal(uplevel, slot, value);
            push_(value);
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_ONCE_GET):
        {
            push_(frames_.back().func_ref->onces[readByte_()]);
            REI_DISPATCH;
        }
        REI_LABEL(OP_ONCE_SET):
        {
            frames_.back().func_ref->onces[readByte_()] = peek_();
            REI_DISPATCH;
        }
        REI_LABEL(OP_ONCE_SSET):
        {
        {
            Value::Data& target = frames_.back().func_ref->onces[readByte_()];
            auto op = static_cast<Opcode>(pop_().asInteger());
            target = dispatchBinary_(target, pop_(), op);
            push_(target);
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_CALL):
        {
        {
            Bytecode argc = readByte_();
            auto callee = peek_(argc);
            switch (callee.tag)
            {
            case Value::VT_FUNCTION:
            {
                auto func_ref = callee.asFunction();
                auto& call_frame = frames_.emplace_back(func_ref, ip_);
                ip_ = func_ref->chunk.codes.data();
                env_r_.enter();
                if (argc > func_ref->argc)
                {
                    for (size_t argi = 1; argi <= argc; argi++)
                    {
                        if (argi <= func_ref->argc)
                            env_r_.defLocal(peek_(argc - argi));
                        else
                            break;
                    }
                }
                else
                {
                    Bytecode loop_count = func_ref->argc;
                    for (size_t argi = 1; argi <= loop_count; argi++)
                    {
                        if (argi <= argc)
                            env_r_.defLocal(peek_(argc - argi));
                        else
                            env_r_.defLocal(Nil{});
                    }
                }
                for (size_t argi = 0; argi < argc + 1; argi++)
                    pop_();
                break;
            }
            case Value::VT_NATIVE:
            {
                auto native = callee.asNative();
                Value::Data* argv = argc == 0 ? nullptr : &stack_[stack_.size() - argc];
                Value::Data result = native(argc, argv);
                for (size_t i = 0; i < argc + 1; i++)
                    pop_();
                push_(result);
                break;
            }
            default:
            {
                error_reporter_.report(std::format("尝试调用非可调用对象 {}", callee.dump()), {0, 0});
                for (size_t argi = 0; argi < argc; argi++)
                    pop_();
                break;
            }
            }
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_RETURN):
        {
        {
            auto& call_frame = frames_.back();
            ip_ = call_frame.save_ip;
            frames_.pop_back();
            env_r_.exit();
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_INDEX_GET):
        {
        {
            Integer index = pop_().asInteger();
            auto indexee = pop_();
            switch (indexee.tag)
            {
            case Value::VT_ARRAY:
            {
                auto array_ref = indexee.array;
                if (index < array_ref->size)
                    push_(array_ref->data[index]);
                else
                    push_(Nil{});
                break;
            }
            default:
            {
                error_reporter_.report(std::format("尝试索引非可索引对象 {}", indexee.dump()), {0, 0});
                break;
            }
            }
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_INDEX_SET):
        {
        {
            auto value = pop_();
            Integer index = pop_().toInteger();
            auto indexee = pop_();
            switch (indexee.tag)
            {
            case Value::VT_ARRAY:
            {
                auto array_ref = indexee.array;
                if (index < array_ref->size)
                    array_ref->data[index] = value;
                break;
            }
            default:
            {
                error_reporter_.report(std::format("尝试索引非可索引对象 {}", indexee.dump()), {0, 0});
                break;
            }
            }
            push_(value);
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_INDEX_SSET):
        {
        {
            auto op = static_cast<Opcode>(pop_().toInteger());
            auto value = pop_();
            Integer index = pop_().toInteger();
            auto indexee = pop_();
            switch (indexee.tag)
            {
            case Value::VT_ARRAY:
            {
                auto array_ref = indexee.array;
                if (index < array_ref->size)
                {
                    auto& target = array_ref->data[index];
                    target = dispatchBinary_(target, value, op);
                    push_(target);
                }
                break;
            }
            default:
            {
                error_reporter_.report(std::format("尝试索引非可索引对象 {}", indexee.dump()), {0, 0});
                push_(value);
                break;
            }
            }
        }
            REI_DISPATCH;
        }
        REI_LABEL(OP_BPM):
        {
            midi_.bpm = pop_().toFloat();
            midi_.factor = (60.0f / midi_.bpm) * midi_.beat * 1000;
            REI_DISPATCH;
        }
        REI_LABEL(OP_BEAT):
        {
            midi_.beat = pop_().toFloat();
            midi_.factor = (60.0f / midi_.bpm) * midi_.beat * 1000;
            REI_DISPATCH;
        }
        REI_LABEL(OP_CHANNEL):
        {
            midi_.channel = static_cast<Channel::Index>(pop_().toInteger() - 1);
            REI_DISPATCH;
        }
        REI_LABEL(OP_PROGRAM):
        {
            {
                Value::Data val = pop_();
                Instrument::Type program;
                if (val.isString())
                    program = Instrument::Map::fromString(val.asString());
                else
                    program = static_cast<Instrument::Type>(val.toInteger());
                midi_.out.programChange(midi_.channel, program);
            }
            REI_DISPATCH;
        }
        REI_LABEL(OP_VOLUME):
        {
            midi_.out.volumeControl(midi_.channel, static_cast<uint8_t>(pop_().toInteger()));
            REI_DISPATCH;
        }
        REI_LABEL(OP_VELOCITY):
        {
            midi_.velocity = static_cast<uint8_t>(pop_().toInteger());
            REI_DISPATCH;
        }
        REI_LABEL(OP_PLAY):
        {
            {
                Value::Data val = pop_();
                if (val.isArray())
                {
                    size_t size = val.array->size;
                    for (size_t i = 0; i < size; i++)
                    {
                        Integer note = val.array->data[i].toInteger();
                        if (note != -1)
                            midi_.out.playNote(static_cast<Note::Val>(val.array->data[i].toInteger()), midi_.channel, true, midi_.velocity);
                    }
                }
                else
                {
                    Integer note = val.toInteger();
                    if (note != -1)
                        midi_.out.playNote(static_cast<Note::Val>(val.toInteger()), midi_.channel, true, midi_.velocity);
                }
            }
            REI_DISPATCH;
        }
        REI_LABEL(OP_UNPLAY):
        {
            {
                Value::Data val = pop_();
                if (val.isArray())
                {
                    size_t size = val.array->size;
                    for (size_t i = 0; i < size; i++)
                    {
                        Integer note = val.array->data[i].toInteger();
                        if (note != -1)
                            midi_.out.playNote(static_cast<Note::Val>(val.array->data[i].toInteger()), midi_.channel, false, midi_.velocity);
                    }
                }
                else
                {
                    Integer note = val.toInteger();
                    if (note != -1)
                        midi_.out.playNote(static_cast<Note::Val>(val.toInteger()), midi_.channel, false, midi_.velocity);
                }
            }
            REI_DISPATCH;
        }
        REI_LABEL(OP_WAIT):
        {
            {
                int64_t time = static_cast<int64_t>(pop_().toFloat() * midi_.factor);
                auto target = std::chrono::steady_clock::now() + std::chrono::milliseconds(time);
                std::this_thread::sleep_until(target);
            }
            REI_DISPATCH;
        }
        REI_LABEL(OP_HALT):
        #undef REI_LABEL
        #undef REI_DISPATCH
        #undef REI_IP
    #if REI_DEBUG_ENABLE >= 1
        std::cout << std::endl;
        std::cout << "\033[1m\033[38;2;255;105;180m内存检查：\033[0m" << std::endl;
        std::cout << "stack: size " << stack_.size() << std::endl;
        for (size_t i = 0; i < stack_.size(); i++)
            std::cout << stack_[i].dump() << std::endl;
        std::cout << "env: depth " << env_r_.currLocalDepth() << std::endl;
    #endif
        for (uint8_t i = 0; i < static_cast<uint8_t>(Channel::Count); i++)
            midi_.out.allNotesOffMsg(static_cast<Channel::Index>(i), true);
    }
}
#endif
