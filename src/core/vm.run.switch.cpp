#include "rei/config.hpp"
#if !(defined(REI_COMPUTED_GOTO_OPT) && defined(__GNUC__) && !defined(__clang__))
#include "vm.hpp"
#include "vm.expr.hpp"
#include <iostream>
#include <format>

namespace rei
{
    void VM::run()
    {
    #ifdef REI_DEBUG_ENABLE
        std::cout << "\033[1m\033[38;2;255;105;180m运行结果：\033[0m" << std::endl; 
    #endif
        if (!error_reporter_.empty()) return;
        ip_ = chunk_.codes.data();
        bool runing = true;
        while (runing)
        {
            auto instruction = static_cast<Opcode>(readByte_());
            switch (instruction)
            {
                case OP_CONSTANT:   push_(readConstant_()); break;
                case OP_POP:        pop_(); break;
                case OP_NIL:    push_(Nil{}); break;
                case OP_TRUE:   push_(Boolean(true)); break;
                case OP_FALSE:  push_(Boolean(false)); break;
                case OP_NEG:
                case OP_BIT_NOT:
                case OP_NOT:
                {
                    Value::Data v = pop_();
                    push_(_dispatchUnary(v, instruction));
                    break;
                }
                case OP_ADD:
                case OP_SUB:
                case OP_MUL:
                case OP_DIV:
                case OP_MOD:
                case OP_POW:
                case OP_BIT_AND:
                case OP_BIT_OR:
                case OP_BIT_XOR:
                case OP_BIT_XNOR:
                case OP_BIT_SHL:
                case OP_BIT_SHR:
                case OP_EQ:
                case OP_NE:
                case OP_LT:
                case OP_LE:
                case OP_GT:
                case OP_GE:
                {
                    Value::Data r = pop_();
                    Value::Data l = pop_();
                    push_(_dispatchBinary(l, r, instruction));
                    break;
                }
                case OP_AND:
                {
                    Bytecode offset = readByte_();
                    if (!Value::toBoolean(pop_()))
                    {
                        jump_(offset);
                        push_(false);
                    }
                    else
                    {
                        push_(true);
                    }
                    break;
                }
                case OP_OR:
                {
                    Bytecode offset = readByte_();
                    if (Value::toBoolean(pop_()))
                    {
                        jump_(offset);
                        push_(true);
                    }
                    else
                    {
                        push_(false);
                    }
                    break;
                }
                case OP_BEG:
                    env_r_.enter();
                    break;
                case OP_END:
                    env_r_.exit();
                    break;
                case OP_PRINT:
                    std::cout << Value::toString(pop_());
                    break;
                case OP_PRINTLN:
                    std::cout << Value::toString(pop_()) << std::endl;
                    break;
                case OP_JUMP:
                {
                    jump_(readByte_());
                    break;
                }
                case OP_JMPT:
                {
                    Bytecode offset = readByte_();
                    if (Value::toBoolean(pop_()))
                        jump_(offset);
                    break;
                }
                case OP_JMPF:
                {
                    Bytecode offset = readByte_();
                    if (!Value::toBoolean(pop_()))
                        jump_(offset);
                    break;
                }
                case OP_GET_HOST:
                {
                    push_(env_r_.getHost(readByte_()));
                    break;
                }
                case OP_SET_HOST:
                {
                    env_r_.setHost(readByte_(), peek_());
                    break;
                }
                case OP_DEF_GLOBAL:
                {
                    env_r_.defGlobal(peek_());
                    break;
                }
                case OP_GET_GLOBAL:
                {
                    push_(env_r_.getGlobal(readByte_()));
                    break;
                }
                case OP_SET_GLOBAL:
                {
                    env_r_.setGlobal(readByte_(), peek_());
                    break;
                }
                case OP_DEF_LOCAL:
                {
                    env_r_.defLocal(peek_());
                    break;;
                }
                case OP_GET_LOCAL:
                {
                    Bytecode t0 = readByte_(), t1 = readByte_();
                    push_(env_r_.getLocal(t0, t1));
                    break;
                }
                case OP_SET_LOCAL:
                {
                    Bytecode t0 = readByte_(), t1 = readByte_();
                    env_r_.setLocal(t0, t1, peek_());
                    break;
                }
                case OP_CALL:
                {
                    Bytecode argc = readByte_();
                    Value::Data callee = peek_(argc);
                    if (Value::is<Ref<Function>>(callee))
                    {
                        auto func_ref = Value::as<Ref<Function>>(callee);
                        auto& call_frame = frames_.emplace_back();
                        call_frame.closure.func = func_ref;
                        // call_frame.closure = ;
                        call_frame.save_ip = ip_;
                        ip_ = func_ref->chunk.codes.data();
                        env_r_.enter();
                        if (argc > func_ref->argc)
                        {
                            for (size_t upi = 1; upi <= argc; upi++)
                            {
                                if (upi <= func_ref->argc)
                                    env_r_.defLocal(peek_(argc - upi));
                                else
                                    break;
                            }
                        }
                        else
                        {
                            Bytecode loop_count = func_ref->argc;
                            for (size_t upi = 1; upi <= loop_count; upi++)
                            {
                                if (upi <= argc)
                                    env_r_.defLocal(peek_(argc - upi));
                                else
                                    env_r_.defLocal(Nil{});
                            }
                        }
                        for (size_t argi = 0; argi < argc+1; argi++)
                            pop_();
                    }
                    else if (Value::is<Native>(callee))
                    {
                        auto native = Value::as<Native>(callee);
                        Value::Data* argv = argc == 0 ? nullptr : &stack_[stack_.size() - argc];
                        Value::Data result = native(argc, argv);
                        for (size_t i = 0; i < argc + 1; i++)
                            pop_();
                        push_(result);
                    }
                    else
                    {
                        error_reporter_.report(std::format("尝试调用非函数对象 {}", Value::getDebugString(callee)), {0, 0});
                        for (size_t argi = 0; argi < argc; argi++)
                            pop_();
                    }
                    break;
                }
                case OP_RETURN:
                {
                    auto& call_frame = frames_.back();
                    ip_ = call_frame.save_ip;
                    frames_.pop_back();
                    env_r_.exit();
                    break;
                }
                case OP_HALT:
                    runing = false;
                    break;
                default: break;
            }
        }
    #ifdef REI_DEBUG_ENABLE
        std::cout << std::endl;
        std::cout << "\033[1m\033[38;2;255;105;180m内存检查：\033[0m" << std::endl;
        std::cout << "stack: size " << stack_.size() << std::endl;
        for (size_t i = 0; i < stack_.size(); i++)
            std::cout << Value::getDebugString(stack_[i]) << std::endl;
        std::cout << "env: depth " << env_r_.currLocalDepth() << std::endl;
    #endif
    }
}
#endif
