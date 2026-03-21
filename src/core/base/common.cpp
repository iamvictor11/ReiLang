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
            case VT_UPVAL:      return upval->val != nullptr || upval->data.toBoolean();
            case VT_CLOSURE:    return closure != nullptr;
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
            case VT_UPVAL:
                if (upval)
                {
                    if (upval->val)
                        return upval->val->toInteger();
                    else
                        return upval->data.toInteger();
                }
                else
                    return 0;
            case VT_CLOSURE:    return static_cast<Integer>(std::bit_cast<uintptr_t>(closure.get()));
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
            case VT_UPVAL:
                if (upval)
                {
                    if (upval->val)
                        return upval->val->toFloat();
                    else
                        return upval->data.toFloat();
                }
                else
                    return 0;
            case VT_CLOSURE:    return static_cast<Float>(std::bit_cast<uintptr_t>(closure.get()));
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
                    "function: argc {}",
                    func ? func->argc : 0
                );
            case VT_UPVAL:
                if (upval)
                {
                    if (upval->val)
                        return upval->val->toString();
                    else
                        return upval->data.toString();
                }
                else
                    return "nil";
            case VT_CLOSURE:
                return std::format(
                    "closure: argc {}, upvalc {}",
                    closure ? closure->func.argc : 0,
                    closure ? closure->upvals.size() : 0
                );
            case VT_NATIVE:
                return std::format(
                    "native: {:x}",
                    std::bit_cast<uintptr_t>(native)
                );
            }
            return "unknown";
        }
        std::string dump(const Data& data)
        {
            switch (data.tag)
            {
            case VT_NIL:        return "nil";
            case VT_BOOLEAN:    return data.b ? "true" : "false";
            case VT_INTEGER:    return std::to_string(data.i);
            case VT_FLOAT:      return std::to_string(data.f);
            case VT_STRING:     return data.str ? "\"" + escape(*data.str) + "\"" : "\"\"";
            case VT_FUNCTION:
                return std::format(
                    "function: {:x} argc {}",
                    std::bit_cast<uintptr_t>(data.func.get()),
                    data.func ? data.func->argc : 0
                );
            case VT_UPVAL:
                if (data.upval)
                {
                    if (data.upval->val)
                        return data.upval->val->toString();
                    else
                        return data.upval->data.toString();
                }
                else
                    return "nil";
            case VT_CLOSURE:
                return std::format(
                    "closure: {:x} argc {}, upvalc {}",
                    std::bit_cast<uintptr_t>(data.closure.get()),
                    data.closure ? data.closure->func.argc : 0,
                    data.closure ? data.closure->upvals.size() : 0
                );
            case VT_NATIVE:
                return std::format(
                    "native: {:x}",
                    std::bit_cast<uintptr_t>(data.native)
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
#pragma endregion
}
