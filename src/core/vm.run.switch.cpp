#include "rei/config.hpp"
#if !(REI_COMPUTED_GOTO_OPT == 1 && defined(__GNUC__) && !defined(__clang__))
#include "vm.hpp"
#include "vm.expr.hpp"
#include <iostream>
#include <format>

namespace rei
{
    void VM::run()
    {
    #if REI_DEBUG_ENABLE == 1
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
                case OP_CLONE:      push_(pop_().clone()); break;
                case OP_POP:        pop_(); break;
                case OP_KEEP:       push_(static_cast<Integer>(readByte_())); break;
                case OP_NIL:    push_(Nil{}); break;
                case OP_TRUE:   push_(Boolean(true)); break;
                case OP_FALSE:  push_(Boolean(false)); break;
                case OP_NEG:
                case OP_BIT_NOT:
                case OP_NOT:
                {
                    Value::Data v = pop_();
                    push_(dispatchUnary_(v, instruction));
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
                    push_(dispatchBinary_(l, r, instruction));
                    break;
                }
                case OP_AND:
                {
                    Bytecode offset = readByte_();
                    if (!pop_().toBoolean())
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
                    if (pop_().toBoolean())
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
                case OP_INIT_ARRAY:
                {
                    Bytecode elemc = readByte_();
                    auto array_ref = std::make_shared<Array>(elemc);
                    for (size_t i = 0; i < elemc; i++)
                        array_ref->data[i] = (peek_(elemc - 1 - i));
                    for (size_t i = 0; i < elemc; i++)
                        pop_();
                    push_(array_ref);
                    break;
                }
                case OP_ENTER:
                    env_r_.enter();
                    break;
                case OP_EXIT:
                    env_r_.exit();
                    break;
                case OP_PRINT:
                    std::cout << pop_().toString();
                    break;
                case OP_PRINTLN:
                    std::cout << pop_().toString() << std::endl;
                    break;
                case OP_JUMP:
                {
                    jump_(readByte_());
                    break;
                }
                case OP_JMPT:
                {
                    Bytecode offset = readByte_();
                    if (pop_().toBoolean())
                        jump_(offset);
                    break;
                }
                case OP_JMPF:
                {
                    Bytecode offset = readByte_();
                    if (!pop_().toBoolean())
                        jump_(offset);
                    break;
                }
                case OP_HOST_GET:
                {
                    push_(env_r_.getHost(readByte_()));
                    break;
                }
                case OP_HOST_SET:
                {
                    env_r_.setHost(readByte_(), peek_());
                    break;
                }
                case OP_HOST_SSET:
                {
                    Bytecode slot = readByte_();
                    auto op = static_cast<Opcode>(pop_().asInteger());
                    auto value = dispatchBinary_(env_r_.getHost(slot), pop_(), op);
                    env_r_.setHost(slot, value);
                    push_(value);
                    break;
                }
                case OP_GLOBAL_DEF:
                {
                    env_r_.defGlobal(peek_());
                    break;
                }
                case OP_GLOBAL_GET:
                {
                    push_(env_r_.getGlobal(readByte_()));
                    break;
                }
                case OP_GLOBAL_SET:
                {
                    env_r_.setGlobal(readByte_(), peek_());
                    break;
                }
                case OP_GLOBAL_SSET:
                {
                    Bytecode slot = readByte_();
                    auto op = static_cast<Opcode>(pop_().asInteger());
                    auto value = dispatchBinary_(env_r_.getGlobal(slot), pop_(), op);
                    env_r_.setGlobal(slot, value);
                    push_(value);
                    break;
                }
                case OP_LOCAL_DEF:
                {
                    env_r_.defLocal(peek_());
                    break;;
                }
                case OP_LOCAL_GET:
                {
                    Bytecode uplevel = readByte_(), slot = readByte_();
                    push_(env_r_.getLocal(uplevel, slot));
                    break;
                }
                case OP_LOCAL_SET:
                {
                    Bytecode uplevel = readByte_(), slot = readByte_();
                    env_r_.setLocal(uplevel, slot, peek_());
                    break;
                }
                case OP_LOCAL_SSET:
                {
                    Bytecode uplevel = readByte_();
                    Bytecode slot = readByte_();
                    auto op = static_cast<Opcode>(pop_().asInteger());
                    auto value = dispatchBinary_(env_r_.getLocal(uplevel, slot), pop_(), op);
                    env_r_.setLocal(uplevel, slot, value);
                    push_(value);
                    break;
                }
                case OP_ONCE_GET:
                {
                    push_(frames_.back().func_ref->onces[readByte_()]);
                    break;
                }
                case OP_ONCE_SET:
                {
                    frames_.back().func_ref->onces[readByte_()] = peek_();
                    break;
                }
                case OP_ONCE_SSET:
                {
                    Value::Data& target = frames_.back().func_ref->onces[readByte_()];
                    auto op = static_cast<Opcode>(pop_().asInteger());
                    target = dispatchBinary_(target, pop_(), op);
                    push_(target);
                    break;
                }
                case OP_CALL:
                {
                    Bytecode argc = readByte_();
                    Value::Data callee = peek_(argc);
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
                        error_reporter_.report(std::format("尝试调用非函数对象 {}", callee.dump()), {0, 0});
                        for (size_t argi = 0; argi < argc; argi++)
                            pop_();
                        break;
                    }
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
                case OP_INDEX_GET:
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
                    break;;
                }
                case OP_INDEX_SET:
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
                    break;;
                }
                case OP_INDEX_SSET:
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
                    break;
                }
                case OP_HALT:
                    runing = false;
                    break;
                default: break;
            }
        }
    #if REI_DEBUG_ENABLE == 1
        std::cout << std::endl;
        std::cout << "\033[1m\033[38;2;255;105;180m内存检查：\033[0m" << std::endl;
        std::cout << "stack: size " << stack_.size() << std::endl;
        for (size_t i = 0; i < stack_.size(); i++)
            std::cout << stack_[i].dump() << std::endl;
        std::cout << "env: depth " << env_r_.currLocalDepth() << std::endl;
    #endif
    }
}
#endif
