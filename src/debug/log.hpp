#pragma once
#include "luna/config.hpp"
#ifdef LUNA_DEBUG_ENABLE
#include <string>
#include <format>
namespace luna
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
#define LUNA_DEBUG_LOG_INFO(fmt, ...) \
    luna::Log::Info(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define LUNA_DEBUG_LOG_WARNING(fmt, ...) \
    luna::Log::Warning(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define LUNA_DEBUG_LOG_ERROR(fmt, ...) \
    luna::Log::Error(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define LUNA_DEBUG_LOG_ASSERT(ok, fmt, ...) \
    luna::Log::Assert(ok, std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#define LUNA_DEBUG_LOG_SUCCESS(fmt, ...) \
    luna::Log::Success(std::format(fmt __VA_OPT__(, ) __VA_ARGS__), __FILE__, __LINE__)
#else
#define LUNA_DEBUG_LOG_INFO(fmt, ...)
#define LUNA_DEBUG_LOG_WARNING(fmt, ...)
#define LUNA_DEBUG_LOG_ERROR(fmt, ...)
#define LUNA_DEBUG_LOG_ASSERT(ok, fmt, ...)
#define LUNA_DEBUG_LOG_SUCCESS(fmt, ...)
#endif