#pragma once
#include <cstdint>
#include <variant>
#include <memory>
#include <unordered_map>
#include <vector>

namespace luna
{
    struct Position final
    {
        uint64_t line = 1, column = 1;
    };

    struct Table;
    struct Function;

    using Nil = std::monostate;
    using Bool = bool;
    using Int = int64_t;
    using Float = double;
    using String = std::string;
    template<typename T>
    using Ref = std::shared_ptr<T>;

    using Value = std::variant<Nil, Bool, Int, Float, String, Ref<Table>, Ref<Function>>;
    
    struct Variable final
    {
    public:
        Value value;
    public:
        Variable() = default;
        Variable(Value v) : value(std::move(v)) {}
    public:
        template<typename T>
        bool is() const
        {
            return std::holds_alternative<T>(value);
        }
        template<typename T>
        T& as()
        {
            return std::get<T>(value);
        }
        template<typename T>
        const T& as() const
        {
            return std::get<T>(value);
        }
    };
    struct Table final : public std::enable_shared_from_this<Table>
    {
    public:
        static constexpr size_t npos = static_cast<size_t>(-1);
    public:
        std::vector<Value> data;
        std::unordered_map<String, size_t> map;
    public:
        size_t size() const
        {
            return data.size();
        }
        size_t capacity() const
        {
            return data.capacity();
        }
        Value get(size_t index) const
        {
            if (index < data.size())
                return data[index];
            return Nil{};
        }
        void set(size_t index, Value value)
        {
            if (index >= data.size())
                data.resize(index + 1);
            data[index] = std::move(value);
        }
        void remove(size_t index)
        {
            if (index >= data.size())
                return;
            data[index] = Nil{};
        }
        void erase(size_t index)
        {
            if (index >= data.size())
                return;
            data.erase(data.begin() + index);
            for (auto it = map.begin(); it != map.end();)
            {
                if (it->second == index)
                {
                    it = map.erase(it);
                }
                else
                {
                    if (it->second > index)
                    {
                        it->second--;
                    }
                    ++it;
                }
            }
        }
        void insert(size_t index, Value value)
        {
            if (index > data.size())
            {
                data.push_back(std::move(value));
            }
            else
            {
                data.insert(data.begin() + index, std::move(value));
                for (auto& [key, i] : map)
                    if (i >= index)
                        i++;
            }
        }
        Value get(const String& key) const
        {
            if (auto it = map.find(key); it != map.end())
                return get(it->second);
            return Nil{};
        }
        void set(const String& key, Value value)
        {
            if (auto it = map.find(key); it != map.end())
            {
                set(it->second, std::move(value));
            }
            else
            {
                size_t newIndex = data.size();
                data.push_back(std::move(value));
                map[key] = newIndex;
            }
        }
        void remove(const String& key)
        {
            if (auto it = map.find(key); it != map.end())
                remove(it->second);
        }
        void erase(const String& key)
        {
            if (auto it = map.find(key); it != map.end())
            {
                size_t index = it->second;
                map.erase(it);
                erase(index);
            }
        }
        void affix(size_t index, const String& key)
        {
            map[key] = index;
        }
        void detach(const String& key)
        {
            map.erase(key);
        }
        void clear()
        {
            data.clear();
            map.clear();
        }
        size_t find(const Value& value) const
        {
            for (size_t i = 0; i < data.size(); ++i)
            {
                if (data[i] == value)
                    return i;
            }
            return npos;
        }
    };
    struct Function
    {
    public:
        enum class Kind : uint8_t
        {
            NATIVE,
            SCRIPT
        };
    public:
        
    };
}
