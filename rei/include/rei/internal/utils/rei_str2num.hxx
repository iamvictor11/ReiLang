#pragma once
#include <cstdint>
#include <cctype>
#include <string>
#include <cstdlib>

namespace rei
{
static inline auto stringToInt(const std::string& str) -> int64_t
{
    size_t pos = 0;
    while (pos < str.length() && isspace((unsigned char)str[pos])) pos++;
    int sign = 1;
    if (pos < str.length() && (str[pos] == '+' || str[pos] == '-'))
    {
        if (str[pos] == '-') sign = -1;
        pos++;
    }
    int base = 10;
    if (pos < str.length() && str[pos] == '0')
    {
        pos++;
        if (pos < str.length() && (str[pos] == 'b' || str[pos] == 'B'))
        {
            base = 2;
            pos++;
        }
        else if (pos < str.length() && (str[pos] == 'o' || str[pos] == 'O'))
        {
            base = 8;
            pos++;
        }
        else if (pos < str.length() && (str[pos] == 'x' || str[pos] == 'X'))
        {
            base = 16;
            pos++;
        }
        else
        {
            pos--;
        }
    }
    long result = 0;
    const std::string digits = "0123456789ABCDEF";
    while (pos < str.length())
    {
        if (str[pos] == '\'')
        {
            pos++;
            continue;
        }
        int digit = -1;
        for (int i = 0; i < base; i++)
        {
            if (toupper((unsigned char)str[pos]) == digits[i])
            {
                digit = i;
                break;
            }
        }
        if (digit == -1) break;
        result = result * base + digit;
        pos++;
    }
    return (int64_t)(result * sign);
}
static inline auto stringToFloat(const std::string& str) -> double
{
    if (str.empty()) return 0.0;
    std::string cleaned;
    cleaned.reserve(str.length());
    for (char c : str)
        if (c != '\'') cleaned.push_back(c);
    return std::strtod(cleaned.c_str(), nullptr);
}
}
