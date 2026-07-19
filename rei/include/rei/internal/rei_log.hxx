#pragma once
#include <string>
#include <format>

namespace rei
{
    void warning(const std::string& msg, const char* file, int line);
    void error(const std::string& msg, const char* file, int line);
    void zassert(bool expr, const std::string& msg, const char* file, int line);
}
#define REI_LOG_WARNING(FMT, ...) \
    rei::warning(std::format(FMT __VA_OPT__(,) __VA_ARGS__), __FILE__, __LINE__)
#define REI_LOG_ERROR(FMT, ...) \
    rei::error(std::format(FMT __VA_OPT__(,) __VA_ARGS__), __FILE__, __LINE__)
#define REI_LOG_ASSERT(EXPR, FMT, ...) \
    rei::zassert(EXPR, std::format(FMT __VA_OPT__(,) __VA_ARGS__), __FILE__, __LINE__)
