#include "vm.hpp"
#include "vm.expr.hpp"
#include "vm.print.hpp"
#include "luna/config.hpp"
#include "util/file.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include <iostream>
namespace luna
{
    void VM::loadSimple(const std::string& source)
    {
        auto s = util::fileToString(source);
    }
    void VM::loadFile(const std::string& path)
    {
    #ifdef LUNA_DEBUG_ENABLE
        std::cout << "\033[1m\033[38;2;255;105;180m词法分析：\033[0m" << std::endl;
    #endif
        Lexer lexer {util::fileToString(path), &_error_reporter};
        auto& tokens = lexer.start();
        if (!_error_reporter.empty()) return;
    #ifdef LUNA_DEBUG_ENABLE
        for (const auto& token : tokens)
            std::cout << token.toString() << std::endl;
    #endif
    #ifdef LUNA_DEBUG_ENABLE
        std::cout << "\033[1m\033[38;2;255;105;180m语法分析：\033[0m" << std::endl;
    #endif
        Parser parser {std::move(tokens), &_chunk, &_env, &_error_reporter};
        parser.start();
        if (!_error_reporter.empty())
        {
            _chunk.clear();
            _env.clear();
            return;
        }
    #ifdef LUNA_DEBUG_ENABLE
        _Chunk_debugPrint(&_chunk);
    #endif
    }
#pragma region Run
    void VM::run()
    {
    #ifdef LUNA_DEBUG_ENABLE
        std::cout << "\033[1m\033[38;2;255;105;180m运行结果：\033[0m" << std::endl; 
    #endif
        if (!_error_reporter.empty()) return;
        _ip = _chunk.codes.data();
        Bytecode* endt = &_chunk.codes.back();
        while (_ip <= endt)
        {
            auto instruction = static_cast<Opcode>(_readByte());
            switch (instruction)
            {
                case OP_CONSTANT:   _push(_readConstant()); break;
                case OP_POP:        _pop(); break;
                case OP_NIL:    _push(Nil{}); break;
                case OP_TRUE:   _push(Boolean(true)); break;
                case OP_FALSE:  _push(Boolean(false)); break;
                case OP_NEG:
                case OP_BIT_NOT:
                case OP_NOT:
                {
                    Value::Data v = _pop();
                    _push(_dispatchUnary(v, instruction));
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
                    Value::Data r = _pop();
                    Value::Data l = _pop();
                    _push(_dispatchBinary(l, r, instruction));
                    break;
                }
                case OP_AND:
                case OP_OR:
                    break;
                case OP_BEG:
                    _env.enter();
                    break;
                case OP_END:
                    _env.exit();
                    break;
                case OP_PRINT:
                    std::cout << Value::toString(_pop());
                    break;
                case OP_PRINTLN:
                    std::cout << Value::toString(_pop()) << std::endl;
                    break;
                case OP_RETURN: break;
                case OP_JUMP: break;
                case OP_JUMP_IF_FALSE: break;
                case OP_LOOP: break;
                case OP_DEF_VAR:
                {
                    _env.def(_readByte(), _peek());
                    break;
                }
                case OP_GET_VAR:
                {
                    _push(_env.get(_readByte()));
                    break;
                }
                case OP_SET_VAR:
                {
                    _env.set(_readByte(), _peek());
                    break;
                }
                case OP_CALL: break;
                default: break;
            }
        }
    #ifdef LUNA_DEBUG_ENABLE
        std::cout << std::endl;
        std::cout << "\033[1m\033[38;2;255;105;180m内存检查：\033[0m" << std::endl;
        std::cout << "stack: size " << _stack.size() << std::endl;
        for (size_t i = 0; i < _stack.size(); i++)
            std::cout << Value::getDebugString(_stack[i]) << std::endl;
    #endif
    }
    Bytecode VM::_readByte()
    {
        return *_ip++;
    }
    Value::Data VM::_readConstant()
    {
        size_t index = _readByte();
        return _chunk.constants[index];
    }
    void VM::_push(Value::Data value)
    {
        _stack.push_back(value);
    }
    Value::Data VM::_pop()
    {
        Value::Data value = _stack.back();
        _stack.pop_back();
        return value;
    }
    Value::Data VM::_peek()
    {
        return _stack.back();
    }
    Value::Data VM::_peek(int distance)
    {
        return _stack[-1 - distance];
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
