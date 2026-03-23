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
    Bytecode VM::bind(const std::string& name, const Value::Data& val)
    {
        env_r_.defHost(val);
        return env_c_.defHost(name);
    }
    void VM::loadSimple(const std::string& source)
    {
        (void)source;
    }
    void VM::loadFile(const std::string& path)
    {
    #if REI_DEBUG_ENABLE == 1
        std::cout << "\033[1m\033[38;2;255;105;180m词法分析：\033[0m" << std::endl;
    #endif
        std::string source {};
        if (!util::fileToString(path, &source))
        {
            error_reporter_.report(std::format("文件 {} 打不开 或 不存在", path.c_str()), {0, 0});
            return;
        }
        Lexer lexer {std::move(source), &error_reporter_};
        auto& tokens = lexer.start();
        if (!error_reporter_.empty()) return;
    #if REI_DEBUG_ENABLE == 1
        for (const auto& token : tokens)
            std::cout << token.toString() << std::endl;
    #endif
    #if REI_DEBUG_ENABLE == 1
        std::cout << "\033[1m\033[38;2;255;105;180m语法分析：\033[0m" << std::endl;
    #endif
        Parser parser {std::move(tokens), &chunk_, &env_c_, &error_reporter_};
        parser.start();
        if (!error_reporter_.empty())
        {
            chunk_.clear();
            env_c_.clearCache();
            return;
        }
    #if REI_DEBUG_ENABLE == 1
        chunk_debugPrint_(chunk_);
    #endif
    #if REI_DEBUG_ENABLE == 1
        std::cout << "\033[1m\033[38;2;255;105;180m内存检查：\033[0m" << std::endl;
        std::cout << "stack: size " << stack_.size() << std::endl;
        for (size_t i = 0; i < stack_.size(); i++)
            std::cout << Value::dump(stack_[i]) << std::endl;
        std::cout << "env: depth " << env_c_.currLocalDepth() << std::endl;
    #endif
        env_c_.clearCache();
    }
#pragma region Chunk
    Bytecode VM::readByte_()
    {
        // Bytecode* base = chunk_.codes.data();
        // if (frames_.empty())
        //     base = chunk_.codes.data();
        // else
        // {
        //     auto& cf = frames_.back();
        //     switch (cf.tag)
        //     {
        //     case CallFrame::CFT_FUNC:
        //         base = cf.func->chunk.codes.data();
        //         break;
        //     case CallFrame::CFT_CLOS:
        //         base = cf.clos->func.chunk.codes.data();
        //         break;
        //     }
        // }
        // size_t index = ip_ - base;
        // printf("debug i %zu\n", index);
        return *ip_++;
    }
    Value::Data VM::readConstant_()
    {
        size_t index = readByte_();
        if (frames_.empty())
            return chunk_.constants[index];
        else
            return frames_.back().func_ref->chunk.constants[index];
    }
    void VM::jump_(Bytecode offset)
    {
        ip_ += static_cast<REI_BYTECODE_INT>(offset);
    }
#pragma endregion
#pragma region Stack
    void VM::push_(Value::Data value)
    {
        stack_.push_back(value);
    }
    Value::Data VM::pop_()
    {
        Value::Data value = stack_.back();
        stack_.pop_back();
        return value;
    }
    Value::Data VM::peek_()
    {
        return stack_.back();
    }
    Value::Data VM::peek_(int distance)
    {
        return stack_[stack_.size() -1 - distance];
    }
#pragma endregion
#pragma region Error
    bool VM::hasError()
    {
        return error_reporter_.empty();
    }
    Error::Msg VM::popError()
    {
        return error_reporter_.pop();
    }
#pragma endregion
}
