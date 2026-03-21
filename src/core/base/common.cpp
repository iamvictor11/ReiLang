#include "common.hpp"
#include "string.hpp"
#include <charconv>
#include <format>

namespace rei
{
    namespace Value
    {
        Integer toInteger(const Value::Data& data)
        {
            return std::visit([](auto&& arg) -> Float
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (is_type<T, Nil>)
                    return Integer(0);
                else if constexpr (is_type<T, Boolean>)
                    return static_cast<Integer>(arg);
                else if constexpr (is_type<T, Integer>)
                    return arg;
                else if constexpr (is_type<T, Float>)
                    return static_cast<Integer>(arg);
                else if constexpr (is_type<T, String>)
                {
                    Integer val = 0.0;
                    auto [ptr, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), val);
                    if (ec == std::errc()) return val;
                    return Integer(0);
                }
                else if constexpr (is_type<T, Ref<Function>>)
                    return static_cast<Integer>(std::bit_cast<uintptr_t>(&(*arg)));
                else if constexpr (is_type<T, Native>)
                    return static_cast<Integer>(std::bit_cast<uintptr_t>(arg));
                else
                    return false;
            }, data);
        }
        Float toFloat(const Value::Data& data)
        {
            return std::visit([](auto&& arg) -> Float
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (is_type<T, Nil>)
                    return Float(0.0);
                else if constexpr (is_type<T, Boolean>)
                    return static_cast<Float>(arg);
                else if constexpr (is_type<T, Integer>)
                    return static_cast<Float>(arg);
                else if constexpr (is_type<T, Float>)
                    return arg;
                else if constexpr (is_type<T, String>)
                {
                    Float val = 0.0;
                    auto [ptr, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), val);
                    if (ec == std::errc()) return val;
                    return Float(0.0);
                }
                else if constexpr (is_type<T, Ref<Function>>)
                    return static_cast<Float>(std::bit_cast<uintptr_t>(&(*arg)));
                else if constexpr (is_type<T, Native>)
                    return static_cast<Float>(std::bit_cast<uintptr_t>(arg));
                else
                    return false;
            }, data);
        }
        bool toBoolean(const Value::Data& data)
        {
            return std::visit([](auto&& arg) -> Boolean
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (is_type<T, Nil>)
                    return false;
                else if constexpr (is_type<T, Boolean>)
                    return arg;
                else if constexpr (is_type<T, Integer>)
                    return static_cast<Boolean>(arg);
                else if constexpr (is_type<T, Float>)
                    return static_cast<Boolean>(arg);
                else if constexpr (is_type<T, String>)
                    return !arg.empty();
                else if constexpr (is_type<T, Ref<Function>>)
                    return arg != nullptr;
                else if constexpr (is_type<T, Native>)
                    return arg != nullptr;
                else
                    return false;
            }, data);
        }
        std::string toString(const Value::Data& data)
        {
            return std::visit([](auto&& arg) -> std::string
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (is_type<T, Nil>)
                    return "nil";
                else if constexpr (is_type<T, Boolean>)
                    return arg ? "true" : "false";
                else if constexpr (is_type<T, Integer>)
                    return std::to_string(arg);
                else if constexpr (is_type<T, Float>)
                    return std::to_string(arg);
                else if constexpr (is_type<T, String>)
                    return arg;
                else if constexpr (is_type<T, Ref<Function>>)
                    return std::format("function: argc {}",
                        arg->argc
                    );
                else if constexpr (is_type<T, Native>)
                    return std::format("native: {:x}",
                        std::bit_cast<uintptr_t>(arg)
                    );
                else
                    return "unknown";
            }, data);
        }
        std::string getDebugString(const Value::Data& data)
        {
            return std::visit([](auto&& arg) -> std::string
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (is_type<T, Nil>)
                    return "nil";
                else if constexpr (is_type<T, Boolean>)
                    return arg ? "true" : "false";
                else if constexpr (is_type<T, Integer>)
                    return std::to_string(arg);
                else if constexpr (is_type<T, Float>)
                    return std::to_string(arg);
                else if constexpr (is_type<T, String>)
                    return escape(arg);
                else if constexpr (is_type<T, Ref<Function>>)
                    return std::format("function: argc {}",
                        arg->argc
                    );
                else if constexpr (is_type<T, Native>)
                    return std::format("native: {}",
                        std::bit_cast<uintptr_t>(arg)
                    );
                else
                    return "unknown";
            }, data);
        }
    }
#pragma region Chunk
void Chunk::clear()
{
    constants.clear();
    codes.clear();
}
#pragma endregion
#pragma region Function
#pragma endregion
}
