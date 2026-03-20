#include "vm.hpp"
#include "vm.expr.hpp"
#include "vm.print.hpp"
#include "rei/config.hpp"
#include "util/file.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <iostream>
#include <format>
namespace rei
{
    VM::VM()
    {
        bindSTL_();
    }
    Value::Coord VM::bind(const std::string& name, const Value::Data& val)
    {
        return _env.bind(name, val);
    }
    void VM::loadSimple(const std::string& source)
    {
        (void)source;
    }
    void VM::loadFile(const std::string& path)
    {
    #ifdef REI_DEBUG_ENABLE
        std::cout << "\033[1m\033[38;2;255;105;180m词法分析：\033[0m" << std::endl;
    #endif
        std::string source {};
        if (!util::fileToString(path, &source))
        {
            _error_reporter.report(std::format("文件 {} 打不开 或 不存在", path.c_str()), {0, 0});
            return;
        }
        Lexer lexer {std::move(source), &_error_reporter};
        auto& tokens = lexer.start();
        if (!_error_reporter.empty()) return;
    #ifdef REI_DEBUG_ENABLE
        for (const auto& token : tokens)
            std::cout << token.toString() << std::endl;
    #endif
    #ifdef REI_DEBUG_ENABLE
        std::cout << "\033[1m\033[38;2;255;105;180m语法分析：\033[0m" << std::endl;
    #endif
        Parser parser {std::move(tokens), &_chunk, &_env, &_error_reporter};
        parser.start();
        if (!_error_reporter.empty())
        {
            _chunk.clear();
            return;
        }
        _env.clearCache();
    #ifdef REI_DEBUG_ENABLE
        chunk_debugPrint_(_chunk);
    #endif
    #ifdef REI_DEBUG_ENABLE
        std::cout << "\033[1m\033[38;2;255;105;180m内存检查：\033[0m" << std::endl;
        std::cout << "stack: size " << _stack.size() << std::endl;
        for (size_t i = 0; i < _stack.size(); i++)
            std::cout << Value::getDebugString(_stack[i]) << std::endl;
        std::cout << "env: depth " << _env.currLocalDepth() << std::endl;
    #endif
    }
#pragma region Run
    void VM::run()
    {
    #ifdef REI_DEBUG_ENABLE
        std::cout << "\033[1m\033[38;2;255;105;180m运行结果：\033[0m" << std::endl; 
    #endif
        if (!_error_reporter.empty()) return;
        _ip = _chunk.codes.data();
        _end = &(_chunk.codes.back());
        while (_ip <= _end)
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
                    break;
                }
                case OP_BEG:
                    _env.enter();
                    break;
                case OP_END:
                    _env.exit();
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
                case OP_LOOP: break;
                case OP_DEF_VAR:
                {
                    _env.def(Value::Coord{static_cast<Value::Lifecycle>(readByte_()), readByte_(), readByte_()}, peek_());
                    break;
                }
                case OP_GET_VAR:
                {
                    push_(_env.get(Value::Coord{static_cast<Value::Lifecycle>(readByte_()), readByte_(), readByte_()}));
                    break;
                }
                case OP_SET_VAR:
                {
                    _env.set(Value::Coord{static_cast<Value::Lifecycle>(readByte_()), readByte_(), readByte_()}, peek_());
                    break;
                }
                case OP_CALL:
                {
                    Bytecode argc = readByte_();
                    Value::Data callee = peek_(argc);
                    if (Value::is<Ref<Function>>(callee))
                    {
                        auto func_ref = Value::as<Ref<Function>>(callee);
                        auto& call_frame = _frames.emplace_back();
                        call_frame.closure.func = func_ref;
                        // call_frame.closure = ;
                        call_frame.save_ip = _ip;
                        call_frame.save_end = _end;
                        _ip = func_ref->chunk.codes.data();
                        _end = &(func_ref->chunk.codes.back());
                        _env.enter();
                        if (argc > func_ref->argc)
                        {
                            for (size_t upi = 1; upi <= argc; upi++)
                            {
                                if (upi <= func_ref->argc)
                                    _env.def(Value::Coord{Value::VLC_LOCAL, 0, upi - 1}, peek_(argc - upi));
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
                                    _env.def(Value::Coord{Value::VLC_LOCAL, 0, upi - 1}, peek_(argc - upi));
                                else
                                    _env.def(Value::Coord{Value::VLC_LOCAL, 0, upi - 1}, Nil{});
                            }
                        }
                        for (size_t argi = 0; argi < argc+1; argi++)
                            pop_();
                    }
                    else if (Value::is<Native>(callee))
                    {
                        auto native = Value::as<Native>(callee);
                        Value::Data* argv = argc == 0 ? nullptr : &_stack[_stack.size() - argc];
                        Value::Data result = native(argc, argv);
                        for (size_t i = 0; i < argc + 1; i++)
                            pop_();
                        push_(result);
                    }
                    else
                    {
                        _error_reporter.report("尝试调用非函数对象", {0, 0});
                        for (size_t argi = 0; argi < argc; argi++)
                            pop_();
                    }
                    break;
                }
                case OP_RETURN:
                {
                    auto& call_frame = _frames.back();
                    _ip = call_frame.save_ip;
                    _end = call_frame.save_end;
                    _frames.pop_back();
                    _env.exit();
                    break;
                }
                default: break;
            }
        }
    #ifdef REI_DEBUG_ENABLE
        std::cout << std::endl;
        std::cout << "\033[1m\033[38;2;255;105;180m内存检查：\033[0m" << std::endl;
        std::cout << "stack: size " << _stack.size() << std::endl;
        for (size_t i = 0; i < _stack.size(); i++)
            std::cout << Value::getDebugString(_stack[i]) << std::endl;
        std::cout << "env: depth " << _env.currLocalDepth() << std::endl;
    #endif
    }
#pragma endregion
#pragma region Chunk
    Bytecode VM::readByte_()
    {
        return *_ip++;
    }
    Value::Data VM::readConstant_()
    {
        size_t index = readByte_();
        if (_frames.empty())
            return _chunk.constants[index];
        else
            return _frames.back().closure.func->chunk.constants[index];
    }
    void VM::jump_(Bytecode offset)
    {
        _ip += static_cast<REI_BYTECODE_INT>(offset);
    }
#pragma endregion
#pragma region Stack
    void VM::push_(Value::Data value)
    {
        _stack.push_back(value);
    }
    Value::Data VM::pop_()
    {
        Value::Data value = _stack.back();
        _stack.pop_back();
        return value;
    }
    Value::Data VM::peek_()
    {
        return _stack.back();
    }
    Value::Data VM::peek_(int distance)
    {
        return _stack[_stack.size() -1 - distance];
    }
#pragma endregion
#pragma region Error
    bool VM::hasError()
    {
        return _error_reporter.empty();
    }
    Error::Msg VM::popError()
    {
        return _error_reporter.pop();
    }
#pragma endregion
}
