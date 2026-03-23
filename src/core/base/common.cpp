#include "common.hpp"
#include "string.hpp"
#include <charconv>
#include <format>

namespace rei
{
    namespace Value
    {
        Boolean Data::toBoolean() const
        {
            switch (tag)
            {
            case VT_NIL:        return false;
            case VT_BOOLEAN:    return b;
            case VT_INTEGER:    return i != 0;
            case VT_FLOAT:      return f != 0.0;
            case VT_STRING:     return str && !str->empty();
            case VT_FUNCTION:   return func != nullptr;
            case VT_NATIVE:     return native != nullptr;
            }
            return false;
        }
        Integer Data::toInteger() const
        {
            switch (tag)
            {
            case VT_NIL:        return 0;
            case VT_BOOLEAN:    return static_cast<Integer>(b);
            case VT_INTEGER:    return i;
            case VT_FLOAT:      return static_cast<Integer>(f);
            case VT_STRING:
            {
                Integer val = 0;
                if (str)
                {
                    auto& s = *str;
                    auto [ptr, ec] = std::from_chars(
                        s.data(),
                        s.data() + s.size(),
                        val
                    );
                    if (ec == std::errc()) return val;
                }
                return 0;
            }
            case VT_FUNCTION:   return static_cast<Integer>(std::bit_cast<uintptr_t>(func.get()));
            case VT_NATIVE:     return static_cast<Integer>(std::bit_cast<uintptr_t>(native));
            }
            return 0;
        }
        Float Data::toFloat() const
        {
            switch (tag)
            {
            case VT_NIL:        return 0.0;
            case VT_BOOLEAN:    return static_cast<Float>(b);
            case VT_INTEGER:    return static_cast<Float>(i);
            case VT_FLOAT:      return f;
            case VT_STRING:
            {
                Float val = 0.0;
                if (str)
                {
                    auto& s = *str;
                    auto [ptr, ec] = std::from_chars(
                        s.data(),
                        s.data() + s.size(),
                        val
                    );
                    if (ec == std::errc()) return val;
                }
                return 0.0;
            }
            case VT_FUNCTION:   return static_cast<Float>(std::bit_cast<uintptr_t>(func.get()));
            case VT_NATIVE:     return static_cast<Float>(std::bit_cast<uintptr_t>(native));
            }
            return 0.0;
        }
        String Data::toString() const
        {
            switch (tag)
            {
            case VT_NIL:        return "nil";
            case VT_BOOLEAN:    return b ? "true" : "false";
            case VT_INTEGER:    return std::to_string(i);
            case VT_FLOAT:      return std::to_string(f);
            case VT_STRING:     return str ? *str : "";
            case VT_FUNCTION:
                return std::format(
                    "function: argc {}, once {}",
                    func ? func->argc : 0,
                    func ? func->onces.size() : 0
                );
            case VT_NATIVE:
                return std::format(
                    "native: {:x}",
                    std::bit_cast<uintptr_t>(native)
                );
            }
            return "unknown";
        }
        Data Data::clone() const
        {
            switch (tag)
            {
            case VT_STRING:
                if (str)
                    return Data(std::make_shared<String>(*str));
                return Data();
            case VT_FUNCTION:
                if (func)
                    return Data(func->clone());
                return Data();
            default:
                return Data(*this);
            }
        }
        std::string Data::dump() const
        {
            switch (tag)
            {
            case VT_NIL:        return "nil";
            case VT_BOOLEAN:    return b ? "true" : "false";
            case VT_INTEGER:    return std::to_string(i);
            case VT_FLOAT:      return std::to_string(f);
            case VT_STRING:     return str ? "\"" + escape(*str) + "\"" : "\"\"";
            case VT_FUNCTION:
                return std::format(
                    "function: {:x} argc {}, once {}",
                    std::bit_cast<uintptr_t>(func.get()),
                    func ? func->argc : 0,
                    func ? func->onces.size() : 0
                );
            case VT_NATIVE:
                return std::format(
                    "native: {:x}",
                    std::bit_cast<uintptr_t>(native)
                );
            }
            return "unknown";
        }
    }
#pragma region Chunk
void Chunk::clear()
{
    constants.clear();
    codes.clear();
}
Chunk Chunk::clone() const
{
    Chunk cloned;
    cloned.constants.reserve(constants.size());
    for (const auto& constant : constants)
        cloned.constants.push_back(constant.clone());
    cloned.codes = codes;
    return cloned;
}
#pragma endregion
#pragma region Function
Ref<Function> Function::clone() const
{
    auto cloned = std::make_shared<Function>();
    cloned->chunk = chunk.clone();
    cloned->argc = argc;
    cloned->onces.reserve(onces.size());
    for (const auto& value : onces)
        cloned->onces.push_back(value.clone());
    return cloned;
}
#pragma endregion
}
