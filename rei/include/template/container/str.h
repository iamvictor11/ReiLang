#ifndef C_TEMPLATE_CONTAINER_STR_H
#define C_TEMPLATE_CONTAINER_STR_H

#include "vector.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

#pragma region Dec
#define C_TEMPLATE_DECL_STRING(ATTR, SPREFIX, LPREFIX, NAME) \
    C_TEMPLATE_DECL_VECTOR(ATTR, SPREFIX, LPREFIX, NAME, char) \
    /* 创建 */ \
    ATTR LPREFIX##NAME SPREFIX##NAME##CreateFromCstr(const char* cstr); \
    ATTR LPREFIX##NAME SPREFIX##NAME##CreateFromFormat(const char* format, ...); \
    ATTR LPREFIX##NAME SPREFIX##NAME##CreateFromVFormat(const char* format, va_list args); \
    /* 属性 */ \
    ATTR size_t SPREFIX##NAME##Length(LPREFIX##NAME me); \
    /* 写入 */ \
    ATTR void SPREFIX##NAME##AssignFromCstr(LPREFIX##NAME me, const char* cstr); \
    ATTR void SPREFIX##NAME##AssignFromFormat(LPREFIX##NAME me, const char* format, ...); \
    ATTR void SPREFIX##NAME##AssignFromVFormat(LPREFIX##NAME me, const char* format, va_list args); \
    ATTR void SPREFIX##NAME##AppendFromCstr(LPREFIX##NAME me, const char* cstr); \
    ATTR void SPREFIX##NAME##AppendFromFormat(LPREFIX##NAME me, const char* format, ...); \
    ATTR void SPREFIX##NAME##AppendFromVFormat(LPREFIX##NAME me, const char* format, va_list args); \
    /* 转换 */ \
    ATTR const char* SPREFIX##NAME##ToCstr(const LPREFIX##NAME me); \
    ATTR int SPREFIX##NAME##ToInt(const LPREFIX##NAME me); \
    ATTR float SPREFIX##NAME##ToFloat(const LPREFIX##NAME me); \
    ATTR double SPREFIX##NAME##ToDouble(const LPREFIX##NAME me); \
    /* 哈希 */ \
    ATTR size_t SPREFIX##NAME##Hash(const LPREFIX##NAME me);
#pragma endregion
#pragma region Def
#define C_TEMPLATE_DEFN_STRING(ATTR, SPREFIX, LPREFIX, NAME) \
    C_TEMPLATE_DEFN_VECTOR(ATTR, SPREFIX, LPREFIX, NAME, char)
#pragma endregion
#pragma region Impl
#define C_TEMPLATE_IMPL_STRING(ATTR, SPREFIX, LPREFIX, NAME, ALLOCATOR) \
    C_TEMPLATE_IMPL_VECTOR(ATTR, SPREFIX, LPREFIX, NAME, char, ALLOCATOR) \
    /* 创建 */ \
    ATTR LPREFIX##NAME SPREFIX##NAME##CreateFromCstr(const char* cstr) \
    { \
        LPREFIX##NAME me = SPREFIX##NAME##Create(NULL); \
        if (me == NULL) return NULL; \
        SPREFIX##NAME##AssignFromCstr(me, cstr); \
        return me; \
    } \
    ATTR LPREFIX##NAME SPREFIX##NAME##CreateFromFormat(const char* format, ...) \
    { \
        va_list args; \
        va_start(args, format); \
        LPREFIX##NAME me = SPREFIX##NAME##CreateFromVFormat(format, args); \
        va_end(args); \
        return me; \
    } \
    ATTR LPREFIX##NAME SPREFIX##NAME##CreateFromVFormat(const char* format, va_list args) \
    { \
        LPREFIX##NAME me = SPREFIX##NAME##Create(NULL); \
        SPREFIX##NAME##AssignFromVFormat(me, format, args); \
        return me; \
    } \
    /* 属性 */ \
    ATTR size_t SPREFIX##NAME##Length(LPREFIX##NAME me) \
    { \
        return me->size; \
    } \
    /* 写入 */ \
    ATTR void SPREFIX##NAME##AssignFromCstr(LPREFIX##NAME me, const char* cstr) \
    { \
        if (cstr != NULL) \
            SPREFIX##NAME##Assign(me, strlen(cstr), cstr); \
    } \
    ATTR void SPREFIX##NAME##AppendFromCstr(LPREFIX##NAME me, const char* cstr) \
    { \
        if (cstr != NULL) \
            SPREFIX##NAME##Append(me, strlen(cstr), cstr); \
    } \
    ATTR void SPREFIX##NAME##AssignFromFormat(LPREFIX##NAME me, const char* format, ...) \
    { \
        va_list args; \
        va_start(args, format); \
        SPREFIX##NAME##AssignFromVFormat(me, format, args); \
        va_end(args); \
    } \
    ATTR void SPREFIX##NAME##AssignFromVFormat(LPREFIX##NAME me, const char* format, va_list args) \
    { \
        if (format == NULL) return; \
        va_list args_copy; \
        va_copy(args_copy, args); \
        int len = vsnprintf(NULL, 0, format, args_copy); \
        va_end(args_copy); \
        if (len < 0) return; \
        SPREFIX##NAME##Reserve(me, len + 1); \
        me->size = len; \
        vsnprintf(me->data, len + 1, format, args); \
    } \
    ATTR void SPREFIX##NAME##AppendFromFormat(LPREFIX##NAME me, const char* format, ...) \
    { \
        va_list args; \
        va_start(args, format); \
        SPREFIX##NAME##AppendFromVFormat(me, format, args); \
        va_end(args); \
    } \
    ATTR void SPREFIX##NAME##AppendFromVFormat(LPREFIX##NAME me, const char* format, va_list args) \
    { \
        if (format == NULL) return; \
        va_list args_copy; \
        va_copy(args_copy, args); \
        int len = vsnprintf(NULL, 0, format, args_copy); \
        va_end(args_copy); \
        if (len < 0) return; \
        size_t old_size = me->size; \
        SPREFIX##NAME##Reserve(me, old_size + len + 1); \
        vsnprintf(me->data + old_size, len + 1, format, args); \
        me->size = old_size + len; \
    } \
    /* 转换 */ \
    ATTR const char* SPREFIX##NAME##ToCstr(const LPREFIX##NAME me) \
    { \
        if (me->data == NULL) return ""; \
        if (me->size == me->capacity) \
            SPREFIX##NAME##Reserve(me, me->size + 1); \
        me->data[me->size] = '\0'; \
        return me->data; \
    } \
    ATTR int SPREFIX##NAME##ToInt(const LPREFIX##NAME me) \
    { \
        const char* str = SPREFIX##NAME##ToCstr(me); \
        while (isspace((unsigned char)*str)) str++; \
        int sign = 1; \
        if (*str == '+' || *str == '-') \
        { \
            if (*str == '-') sign = -1; \
            str++; \
        } \
        int base = 10; \
        if (*str == '0') \
        { \
            str++; \
            if (*str == 'b' || *str == 'B') \
            { \
                base = 2; \
                str++; \
            } \
            else if (*str == 'o' || *str == 'O') \
            { \
                base = 8; \
                str++; \
            } \
            else if (*str == 'x' || *str == 'X') \
            { \
                base = 16; \
                str++; \
            } \
        } \
        long result = 0; \
        const char* digits = "0123456789ABCDEF"; \
        while (*str) \
        { \
            if (*str == '\'') {str++; continue;} \
            int digit = -1; \
            for (int i = 0; i < base; i++) \
                if (toupper((unsigned char)*str) == digits[i]) \
                { \
                    digit = i; \
                    break; \
                } \
            if (digit == -1) break; \
            result = result * base + digit; \
            str++; \
        } \
        return (int)(result * sign); \
    } \
    ATTR float SPREFIX##NAME##ToFloat(const LPREFIX##NAME me) \
    { \
        return (float)SPREFIX##NAME##ToDouble(me);\
    } \
    ATTR double SPREFIX##NAME##ToDouble(const LPREFIX##NAME me) \
    { \
        const char* str = SPREFIX##NAME##ToCstr(me); \
        if (str == NULL) return 0.0f; \
        size_t len = strlen(str); \
        char* buf = (char*)(ALLOCATOR)->malloc((ALLOCATOR)->context, len + 1);\
        if (!buf) return 0.0f; \
        size_t j = 0; \
        for (size_t i = 0; i < len; i++) \
        { \
            if (str[i] == '\'') continue; \
            buf[j++] = str[i]; \
        } \
        buf[j] = '\0'; \
        double val = strtod(buf, NULL); \
        (ALLOCATOR)->free((ALLOCATOR)->context, buf); \
        return val; \
    } \
    /* 哈希 */ \
    ATTR size_t SPREFIX##NAME##Hash(const LPREFIX##NAME me) \
    { \
        const char* str = SPREFIX##NAME##ToCstr(me); \
        size_t hash = 5381; \
        int c; \
        while ((c = *str++) != '\0') \
            hash = ((hash << 5) + hash) + (unsigned char)c; \
        return hash; \
    }
#pragma endregion
#pragma region Sugar
#define C_TEMPLATE_STRING_FOREACH(CONTAINER, ELEM_PTR_NAME) C_TEMPLATE_VECTOR_FOREACH(char, CONTAINER, ELEM_PTR_NAME)
#define C_TEMPLATE_STRING_RFOREACH(CONTAINER, ELEM_PTR_NAME) C_TEMPLATE_VECTOR_RFOREACH(char, CONTAINER, ELEM_PTR_NAME)
#pragma endregion

#endif
