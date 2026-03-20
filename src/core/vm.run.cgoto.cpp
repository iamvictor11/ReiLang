#include "rei/config.hpp"
#if defined(REI_COMPUTED_GOTO_OPT) && defined(__GNUC__) && !defined(__clang__)
#include "vm.hpp"
#include "vm.expr.hpp"
#include <iostream>

namespace rei
{
    void VM::run()
    {
    #ifdef REI_DEBUG_ENABLE
        std::cout << "\033[1m\033[38;2;255;105;180m运行结果：\033[0m" << std::endl; 
    #endif
        if (!error_reporter_.empty()) return;
        ip_ = chunk_.codes.data();
        end_ = &(chunk_.codes.back());
        #define REI_LABEL(op) REI_LABEL_##op
        static const void* dispatch_table[] =
        {
            &&REI_LABEL(OP_CONSTANT),
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
            &&REI_LABEL(OP_BEG),
            &&REI_LABEL(OP_END),
            &&REI_LABEL(OP_PRINT),
            &&REI_LABEL(OP_PRINTLN),
            &&REI_LABEL(OP_JUMP),
            &&REI_LABEL(OP_JMPT),
            &&REI_LABEL(OP_JMPF),
            &&REI_LABEL(OP_DEF_VAR),
            &&REI_LABEL(OP_GET_VAR),
            &&REI_LABEL(OP_SET_VAR),
            &&REI_LABEL(OP_CALL),
            &&REI_LABEL(OP_RETURN),
            &&REI_LABEL(OP_HALT)
        };
        #define REI_DISPATCH goto *dispatch_table[readByte_()]
        #define REI_IP ip_[-1]
        Value::Data tempVal, tempL, tempR;
        Value::Data callee;
        Ref<Function> func_ref;
        Native native;
        Bytecode offset, argc;
        REI_DISPATCH;
        REI_LABEL(OP_CONSTANT):
        {
            push_(readConstant_());
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
            if (!Value::toBoolean(pop_()))
            {
                jump_(offset);
                push_(false);
            }
            REI_DISPATCH;
        }
        REI_LABEL(OP_OR):
        {
            offset = readByte_();
            if (Value::toBoolean(pop_()))
            {
                jump_(offset);
                push_(true);
            }
            REI_DISPATCH;
        }
        REI_LABEL(OP_BEG):
        {
            env_.enter();
            REI_DISPATCH;
        }
        REI_LABEL(OP_END):
        {
            env_.exit();
            REI_DISPATCH;
        }
        REI_LABEL(OP_PRINT):
        {
            std::cout << Value::toString(pop_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_PRINTLN):
        {
            std::cout << Value::toString(pop_()) << std::endl;
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
            if (Value::toBoolean(pop_()))
                jump_(offset);
            REI_DISPATCH;
        }
        REI_LABEL(OP_JMPF):
        {
            offset = readByte_();
            if (!Value::toBoolean(pop_()))
                jump_(offset);
            REI_DISPATCH;
        }
        REI_LABEL(OP_DEF_VAR):
        {
            env_.def(Value::Coord{static_cast<Value::Lifecycle>(readByte_()), readByte_(), readByte_()}, peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_GET_VAR):
        {
            push_(env_.get(Value::Coord{static_cast<Value::Lifecycle>(readByte_()), readByte_(), readByte_()}));
            REI_DISPATCH;
        }
        REI_LABEL(OP_SET_VAR):
        {
            env_.set(Value::Coord{static_cast<Value::Lifecycle>(readByte_()), readByte_(), readByte_()}, peek_());
            REI_DISPATCH;
        }
        REI_LABEL(OP_CALL):
        {
            argc = readByte_();
            callee = peek_(argc);
            if (Value::is<Ref<Function>>(callee))
            {
                func_ref = Value::as<Ref<Function>>(callee);
                auto& call_frame = frames_.emplace_back();
                call_frame.closure.func = func_ref;
                call_frame.save_ip = ip_;
                call_frame.save_end = end_;
                ip_ = func_ref->chunk.codes.data();
                end_ = &(func_ref->chunk.codes.back());
                env_.enter();
                if (argc > func_ref->argc)
                {
                    for (size_t upi = 1; upi <= argc; upi++)
                    {
                        if (upi <= func_ref->argc)
                            env_.def(Value::Coord{Value::VLC_LOCAL, 0, upi - 1}, peek_(argc - upi));
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
                            env_.def(Value::Coord{Value::VLC_LOCAL, 0, upi - 1}, peek_(argc - upi));
                        else
                            env_.def(Value::Coord{Value::VLC_LOCAL, 0, upi - 1}, Nil{});
                    }
                }
                for (size_t argi = 0; argi < argc + 1; argi++)
                    pop_();
                goto *dispatch_table[func_ref->chunk.codes[0]];
            }
            else if (Value::is<Native>(callee))
            {
                native = Value::as<Native>(callee);
                Value::Data* argv = argc == 0 ? nullptr : &stack_[stack_.size() - argc];
                Value::Data result = native(argc, argv);
                for (size_t i = 0; i < argc + 1; i++)
                    pop_();
                push_(result);
            }
            else
            {
                error_reporter_.report("尝试调用非函数对象", {0, 0});
                for (size_t argi = 0; argi < argc; argi++)
                    pop_();
            }
            REI_DISPATCH;
        }
        REI_LABEL(OP_RETURN):
        {
            auto& call_frame = frames_.back();
            ip_ = call_frame.save_ip;
            end_ = call_frame.save_end;
            frames_.pop_back();
            env_.exit();
            REI_DISPATCH;
        }
        REI_LABEL(OP_HALT):
        #undef REI_LABEL
        #undef REI_DISPATCH
        #undef REI_IP
    #ifdef REI_DEBUG_ENABLE
        std::cout << std::endl;
        std::cout << "\033[1m\033[38;2;255;105;180m内存检查：\033[0m" << std::endl;
        std::cout << "stack: size " << stack_.size() << std::endl;
        for (size_t i = 0; i < stack_.size(); i++)
            std::cout << Value::getDebugString(stack_[i]) << std::endl;
        std::cout << "env: depth " << env_.currLocalDepth() << std::endl;
    #endif
    }
}
#endif
