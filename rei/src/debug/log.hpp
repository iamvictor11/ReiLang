#pragma once
#include "rei/config.hpp"
#include <string>
#include <format>

namespace rei
{
    namespace Log
    {
        void echoInfo(const std::string &, const char *file, int line);
        void echoWarning(const std::string &, const char *file, int line);
        void echoError(const std::string &, const char *file, int line);
        void echoAssert(bool ok, const std::string &, const char *file, int line);
        void echoSuccess(const std::string &, const char *file, int line);
    };
}
#if REI_DEBUG_ENABLE >= 1
#define REI_DEBUG_LOG_INFO(fmt, ...) \
    rei::Log::echoInfo(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define REI_DEBUG_LOG_WARNING(fmt, ...) \
    rei::Log::echoWarning(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define REI_DEBUG_LOG_SUCCESS(fmt, ...) \
    rei::Log::echoSuccess(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#else
#define REI_DEBUG_LOG_INFO(fmt, ...)
#define REI_DEBUG_LOG_WARNING(fmt, ...)
#define REI_DEBUG_LOG_SUCCESS(fmt, ...)
#endif
#define REI_DEBUG_LOG_ERROR(fmt, ...) \
    rei::Log::echoError(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define REI_DEBUG_LOG_ASSERT(ok, fmt, ...) \
    rei::Log::echoAssert(ok, std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
