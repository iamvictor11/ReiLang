#pragma once
#include "vic/config.hpp"
#ifdef VIC_DEBUG_ENABLE
#include <string>
#include <format>
namespace vic
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
#define VIC_DEBUG_LOG_INFO(fmt, ...) \
    vic::Log::Info(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define VIC_DEBUG_LOG_WARNING(fmt, ...) \
    vic::Log::Warning(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define VIC_DEBUG_LOG_ERROR(fmt, ...) \
    vic::Log::Error(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define VIC_DEBUG_LOG_ASSERT(ok, fmt, ...) \
    vic::Log::Assert(ok, std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define VIC_DEBUG_LOG_SUCCESS(fmt, ...) \
    vic::Log::Success(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#else
#define VIC_DEBUG_LOG_INFO(fmt, ...)
#define VIC_DEBUG_LOG_WARNING(fmt, ...)
#define VIC_DEBUG_LOG_ERROR(fmt, ...)
#define VIC_DEBUG_LOG_ASSERT(ok, fmt, ...)
#define VIC_DEBUG_LOG_SUCCESS(fmt, ...)
#endif