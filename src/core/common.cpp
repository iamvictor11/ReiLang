#include "common.hpp"

namespace luna
{
#pragma region Table
    size_t Table::size() const
    {
        return data.size();
    }
    size_t Table::capacity() const
    {
        return data.capacity();
    }
    Value Table::get(size_t index) const
    {
        if (index < data.size())
            return data[index];
        return Nil{};
    }
    void Table::set(size_t index, Value value)
    {
        if (index >= data.size())
            data.resize(index + 1);
        data[index] = std::move(value);
    }
    void Table::remove(size_t index)
    {
        if (index >= data.size())
            return;
        data[index] = Nil{};
    }
    void Table::erase(size_t index)
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
    void Table::insert(size_t index, Value value)
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
    Value Table::get(const String& key) const
    {
        if (auto it = map.find(key); it != map.end())
            return get(it->second);
        return Nil{};
    }
    void Table::set(const String& key, Value value)
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
    void Table::remove(const String& key)
    {
        if (auto it = map.find(key); it != map.end())
            remove(it->second);
    }
    void Table::erase(const String& key)
    {
        if (auto it = map.find(key); it != map.end())
        {
            size_t index = it->second;
            map.erase(it);
            erase(index);
        }
    }
    void Table::affix(size_t index, const String& key)
    {
        map[key] = index;
    }
    void Table::detach(const String& key)
    {
        map.erase(key);
    }
    void Table::clear()
    {
        data.clear();
        map.clear();
    }
    size_t Table::find(const Value& value) const
    {
        for (size_t i = 0; i < data.size(); ++i)
        {
            if (data[i] == value)
                return i;
        }
        return npos;
    }
#pragma endregion
#pragma region Function
#pragma endregion
}
