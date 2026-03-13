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
        std::cout << "词法分析：" << std::endl;
    #endif
        Lexer lexer {util::fileToString(path), &_error_reporter};
        auto& tokens = lexer.start();
        if (!_error_reporter.empty()) return;
    #ifdef LUNA_DEBUG_ENABLE
        for (const auto& token : tokens)
            std::cout << token.toString() << std::endl;
    #endif
    #ifdef LUNA_DEBUG_ENABLE
        std::cout << "语法分析：" << std::endl; 
    #endif
        Parser parser {std::move(tokens), &_chunk, &_error_reporter};
        parser.start();
        if (!_error_reporter.empty()) return;
    #ifdef LUNA_DEBUG_ENABLE
        _printChunk(&_chunk);
    #endif
    }
#pragma region Run
    void VM::run()
    {
        if (!_error_reporter.empty()) return;
        _ip = _chunk.codes.data();
        Bytecode* endt = &_chunk.codes.back();
        while (_ip < endt)
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
                case OP_PRINT:
                    break;
                case OP_PRINTLN:
                    break;
                case OP_RETURN: break;
                case OP_JUMP: break;
                case OP_JUMP_IF_FALSE: break;
                case OP_LOOP: break;
                case OP_DEFINE_GLOBAL: break;
                case OP_GET_GLOBAL: break;
                case OP_SET_GLOBAL: break;
                case OP_GET_LOCAL: break;
                case OP_SET_LOCAL: break;
                case OP_CALL: break;
                default: break;
            }
        }
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
