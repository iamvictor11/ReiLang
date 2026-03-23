#include "rei/config.hpp"
#if REI_COMPUTED_GOTO_OPT == 1 && defined(__GNUC__) && !defined(__clang__)
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
        #define REI_LABEL(op) REI_LABEL_##op
        static const void* dispatch_table[] =
        {
            &&REI_LABEL(OP_CONSTANT),
            &&REI_LABEL(OP_CLONE),
            &&REI_LABEL(OP_POP),
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
            &&REI_LABEL(OP_ENTER),
            &&REI_LABEL(OP_EXIT),
            &&REI_LABEL(OP_PRINT),
            &&REI_LABEL(OP_PRINTLN),
            &&REI_LABEL(OP_JUMP),
            &&REI_LABEL(OP_JMPT),
            &&REI_LABEL(OP_JMPF),
            &&REI_LABEL(OP_GET_HOST),
            &&REI_LABEL(OP_SET_HOST),
            &&REI_LABEL(OP_DEF_GLOBAL),
            &&REI_LABEL(OP_GET_GLOBAL),
            &&REI_LABEL(OP_SET_GLOBAL),
            &&REI_LABEL(OP_DEF_LOCAL),
            &&REI_LABEL(OP_GET_LOCAL),
            &&REI_LABEL(OP_SET_LOCAL),
            &&REI_LABEL(OP_GET_ONCE),
            &&REI_LABEL(OP_SET_ONCE),
            &&REI_LABEL(OP_CALL),
            &&REI_LABEL(OP_RETURN),
            &&REI_LABEL(OP_HALT)
        };
        #define REI_DISPATCH goto *dispatch_table[readByte_()]
        #define REI_IP ip_[-1]
        Value::Data tempVal, tempL, tempR;
        Bytecode tempB0, tempB1, tempB3;
        Value::Data callee;
        Bytecode offset, argc;
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
            tempVal = pop_();
            push_(_dispatchUnary(tempVal, static_cast<Opcode>(REI_IP)));
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
            tempR = pop_();
            tempL = pop_();
            push_(_dispatchBinary(tempL, tempR, static_cast<Opcode>(REI_IP)));
            REI_DISPATCH;
        }
        REI_LABEL(OP_AND):
        {
            offset = readByte_();
            if (!(pop_().toBoolean()))
            {
                jump_(offset);
                push_(false);
            }
            else
            {
                push_(true);
            }
            REI_DISPATCH;
        }
        REI_LABEL(OP_OR):
        {
            offset = readByte_();
            if (pop_().toBoolean())
            {
                jump_(offset);
                push_(true);
            }
            else
            {
                push_(false);
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
            offset = readByte_();
            if (pop_().toBoolean())
                jump_(offset);
            REI_DISPATCH;
        }
        REI_LABEL(OP_JMPF):
        {
            offset = readByte_();
            if (!(pop_().toBoolean()))
                jump_(offset);
            REI_DISPATCH;
        }
        REI_LABEL(OP_GET_HOST):
        {
            push_(env_r_.getHost(readByte_()));
            REI_DISPATCH;
        }
        REI_LABEL(OP_SET_HOST):
        {
            env_r_.setHost(readByte_(), peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_DEF_GLOBAL):
        {
            env_r_.defGlobal(peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_GET_GLOBAL):
        {
            push_(env_r_.getGlobal(readByte_()));
            REI_DISPATCH;
        }
        REI_LABEL(OP_SET_GLOBAL):
        {
            env_r_.setGlobal(readByte_(), peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_DEF_LOCAL):
        {
            env_r_.defLocal(peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_GET_LOCAL):
        {
            tempB0 = readByte_();
            tempB1 = readByte_();
            push_(env_r_.getLocal(tempB0, tempB1));
            REI_DISPATCH;
        }
        REI_LABEL(OP_SET_LOCAL):
        {
            tempB0 = readByte_();
            tempB1 = readByte_();
            env_r_.setLocal(tempB0, tempB1, peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_GET_ONCE):
        {
            push_(frames_.back().func_ref->onces[readByte_()]);
            REI_DISPATCH;
        }
        REI_LABEL(OP_SET_ONCE):
        {
            frames_.back().func_ref->onces[readByte_()] = peek_();
            REI_DISPATCH;
        }
        REI_LABEL(OP_CALL):
        {
            argc = readByte_();
            callee = peek_(argc);
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
            REI_DISPATCH;
        }
        REI_LABEL(OP_RETURN):
        {
            auto& call_frame = frames_.back();
            ip_ = call_frame.save_ip;
            frames_.pop_back();
            env_r_.exit();
            REI_DISPATCH;
        }
        REI_LABEL(OP_HALT):
        #undef REI_LABEL
        #undef REI_DISPATCH
        #undef REI_IP
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
