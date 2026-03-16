#pragma once
#include "rei/config.hpp"
#ifdef REI_DEBUG_ENABLE
#include <string>
#include <format>
namespace rei
{
    namespace Log
    {
        void Info(const std::string &, const char *file, int line);
        void Warning(const std::string &, const char *file, int line);
        void Error(const std::string &, const char *file, int line);
        void Assert(bool ok, const std::string &, const char *file, int line);
        void Success(const std::string &, const char *file, int line);
    };
}
#define REI_DEBUG_LOG_INFO(fmt, ...) \
    rei::Log::Info(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define REI_DEBUG_LOG_WARNING(fmt, ...) \
    rei::Log::Warning(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define REI_DEBUG_LOG_ERROR(fmt, ...) \
    rei::Log::Error(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define REI_DEBUG_LOG_ASSERT(ok, fmt, ...) \
    rei::Log::Assert(ok, std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define REI_DEBUG_LOG_SUCCESS(fmt, ...) \
    rei::Log::Success(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#else
#define REI_DEBUG_LOG_INFO(fmt, ...)
#define REI_DEBUG_LOG_WARNING(fmt, ...)
#define REI_DEBUG_LOG_ERROR(fmt, ...)
#define REI_DEBUG_LOG_ASSERT(ok, fmt, ...)
#define REI_DEBUG_LOG_SUCCESS(fmt, ...)
#endif