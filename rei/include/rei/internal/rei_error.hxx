#pragma once
#include "rei/rei.config.hxx"
#include <cstdint>
#include <string>

extern "C"
{
typedef enum ReiErrorCode
{
    REI_SUCCESS,
    REI_ERROR_LEXER,
    REI_ERROR_PARSER,
    REI_ERROR_SEMANTIC,
    REI_ERROR_EVALUATOR,
    REI_ERROR_CODEGEN,
    REI_ERROR_RUNTIME,
    REI_ERROR_UNKNOWN
} ReiErrorCode;
static inline const char* string_ReiErrorCode(ReiErrorCode code)
{
    switch (code)
    {
    case REI_SUCCESS:           return "REI_SUCCESS";
    case REI_ERROR_LEXER:       return "REI_ERROR_LEXER";
    case REI_ERROR_PARSER:      return "REI_ERROR_PARSER";
    case REI_ERROR_SEMANTIC:    return "REI_ERROR_SEMANTIC";
    case REI_ERROR_EVALUATOR:   return "REI_ERROR_EVALUATOR";
    case REI_ERROR_CODEGEN:     return "REI_ERROR_CODEGEN";
    case REI_ERROR_RUNTIME:     return "REI_ERROR_RUNTIME";
    default: return "REI_ERROR_UNKNOWN";
    }
}
}

namespace rei
{
    struct Error final
    {
    public:
        std::string title;
        std::string message;
        ReiErrorCode code = REI_SUCCESS;
    public:
        auto format() const -> std::string;
    public:
        bool okey() const { return code == REI_SUCCESS; }
        bool has() const { return code != REI_SUCCESS; }
        bool empty() const { return code == REI_SUCCESS; }
    public:
        #if REI_WITH_EXCEPTIONS
        [[noreturn]]
        #endif
        void report();
    };
}
