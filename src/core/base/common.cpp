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
                if constexpr (std::is_same_v<T, Nil>)
                    return Integer(0);
                else if constexpr (std::is_same_v<T, Boolean>)
                    return static_cast<Integer>(arg);
                else if constexpr (std::is_same_v<T, Integer>)
                    return arg;
                else if constexpr (std::is_same_v<T, Float>)
                    return static_cast<Integer>(arg);
                else if constexpr (std::is_same_v<T, String>)
                {
                    Integer val = 0.0;
                    auto [ptr, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), val);
                    if (ec == std::errc()) return val;
                    return Integer(0);
                }
                else if constexpr (std::is_same_v<T, Ref<Function>>)
                    return static_cast<Integer>(toAddress(arg));
                else
                    return false;
            }, data);
        }
        Float toFloat(const Value::Data& data)
        {
            return std::visit([](auto&& arg) -> Float
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, Nil>)
                    return Float(0.0);
                else if constexpr (std::is_same_v<T, Boolean>)
                    return static_cast<Float>(arg);
                else if constexpr (std::is_same_v<T, Integer>)
                    return static_cast<Float>(arg);
                else if constexpr (std::is_same_v<T, Float>)
                    return arg;
                else if constexpr (std::is_same_v<T, String>)
                {
                    Float val = 0.0;
                    auto [ptr, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), val);
                    if (ec == std::errc()) return val;
                    return Float(0.0);
                }
                else if constexpr (std::is_same_v<T, Ref<Function>>)
                    return static_cast<Float>(toAddress(arg));
                else
                    return false;
            }, data);
        }
        bool toBoolean(const Value::Data& data)
        {
            return std::visit([](auto&& arg) -> Boolean
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, Nil>)
                    return false;
                else if constexpr (std::is_same_v<T, Boolean>)
                    return arg;
                else if constexpr (std::is_same_v<T, Integer>)
                    return static_cast<Boolean>(arg);
                else if constexpr (std::is_same_v<T, Float>)
                    return static_cast<Boolean>(arg);
                else if constexpr (std::is_same_v<T, String>)
                    return !arg.empty();
                else if constexpr (std::is_same_v<T, Ref<Function>>)
                    return arg != nullptr;
                else
                    return false;
            }, data);
        }
        std::string toString(Value::Data data)
        {
            return std::visit([](auto&& arg) -> std::string
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, Nil>)
                    return "nil";
                else if constexpr (std::is_same_v<T, Boolean>)
                    return arg ? "true" : "false";
                else if constexpr (std::is_same_v<T, Integer>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, Float>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, String>)
                    return arg;
                else if constexpr (std::is_same_v<T, Ref<Function>>)
                    return std::format("function: kind {}, argc {}",
                        arg->kind == Function::Kind::NATIVE ? "Native" : "Script",
                        arg->argc
                    );
                else
                    return "unknown";
            }, data);
        }
        std::string getDebugString(Value::Data data)
        {
            return std::visit([](auto&& arg) -> std::string
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, Nil>)
                    return "nil";
                else if constexpr (std::is_same_v<T, Boolean>)
                    return arg ? "true" : "false";
                else if constexpr (std::is_same_v<T, Integer>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, Float>)
                    return std::to_string(arg);
                else if constexpr (std::is_same_v<T, String>)
                    return escape(arg);
                else if constexpr (std::is_same_v<T, Ref<Function>>)
                    return std::format("function: kind {}, argc {}",
                        arg->kind == Function::Kind::NATIVE ? "Native" : "Script",
                        arg->argc
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
