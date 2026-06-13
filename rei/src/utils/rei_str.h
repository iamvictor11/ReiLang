#ifndef REI_UTILS_STR_H
#define REI_UTILS_STR_H

#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static inline int64_t reiCstrToInt(const char* str)
{
    while (isspace((unsigned char)*str)) str++;
    int sign = 1;
    if (*str == '+' || *str == '-')
    {
        if (*str == '-') sign = -1;
        str++;
    }
    int base = 10;
    if (*str == '0')
    {
        str++;
        if (*str == 'b' || *str == 'B')
        {
            base = 2;
            str++;
        }
        else if (*str == 'o' || *str == 'O')
        {
            base = 8;
            str++;
        }
        else if (*str == 'x' || *str == 'X')
        {
            base = 16;
            str++;
        }
    }
    long result = 0;
    const char* digits = "0123456789ABCDEF";
    while (*str)
    {
        if (*str == '\'') {str++; continue;}
        int digit = -1;
        for (int i = 0; i < base; i++)
            if (toupper((unsigned char)*str) == digits[i])
            {
                digit = i;
                break;
            }
        if (digit == -1) break;
        result = result * base + digit;
        str++;
    }
    return (int)(result * sign);
}
static inline double reiCharsToFloat(const char* chars)
{
    if (chars == NULL) return 0.0f;
    const size_t len = REI_MAX_BUFFER_SIZE;
    char buf[len];
    memset(buf, 0, len);
    if (!buf) return 0.0f;
    size_t j = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (chars[i] == '\'') continue;
        buf[j++] = chars[i];
    }
    buf[j] = '\0';
    double val = strtod(buf, NULL);
    return val;
}
static inline double reiCstrToFloat(const char* str)
{
    if (str == NULL) return 0.0f;
    size_t len = strlen(str);
    char buf[len];
    memset(buf, 0, len);
    if (!buf) return 0.0f;
    size_t j = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (str[i] == '\'') continue;
        buf[j++] = str[i];
    }
    buf[j] = '\0';
    double val = strtod(buf, NULL);
    return val;
}

#endif
