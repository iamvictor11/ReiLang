#ifndef C_TEMPLATE_TYPE_UUID_H
#define C_TEMPLATE_TYPE_UUID_H

#include "../allocator.h"
#include <stdint.h>

#pragma region Dec
#define C_TEMPLATE_DECL_UUID(ATTR, SPREFIX, LPREFIX, NAME) \
    typedef struct LPREFIX##NAME LPREFIX##NAME; \
    /* 随机数引擎函数指针 */ \
    ATTR LPREFIX##NAME SPREFIX##NAME##Generator(LPREFIX##NAME state); \
    ATTR void SPREFIX##NAME##Clampor(LPREFIX##NAME* value, LPREFIX##NAME min, LPREFIX##NAME max); \
    /* 转换 */ \
    ATTR LPREFIX##NAME SPREFIX##NAME##FromCstr(const char* str); \
    ATTR const char* SPREFIX##NAME##ToCstr(LPREFIX##NAME me);
#pragma endregion

#pragma region Def
#define C_TEMPLATE_DEFN_UUID(ATTR, SPREFIX, LPREFIX, NAME) \
    typedef struct LPREFIX##NAME \
    { \
        uint64_t high, low; \
    } LPREFIX##NAME;
#pragma endregion

#pragma region Impl
#define C_TEMPLATE_IMPL_UUID(ATTR, SPREFIX, LPREFIX, NAME, ALLOCATOR) \
    /* 私有 */ \
    static inline uint64_t SPREFIX##NAME##_HexToInt(char c) \
    { \
        if (c >= '0' && c <= '9') return c - '0'; \
        if (c >= 'a' && c <= 'f') return c - 'a' + 10; \
        if (c >= 'A' && c <= 'F') return c - 'A' + 10; \
        return 0; \
    } \
    static inline uint64_t SPREFIX##NAME##_GetByteInCstr(const char* str, size_t index) \
    { \
        return (SPREFIX##NAME##_HexToInt(str[index]) << 4) | SPREFIX##NAME##_HexToInt(str[index + 1]); \
    } \
    static inline void SPREFIX##NAME##_WriteByteInBuffer(char* bufffer, size_t offset, uint8_t byte) \
    { \
        static const char* digits = "0123456789abcdef"; \
        bufffer[offset] = digits[(byte >> 4) & 0x0F]; \
        bufffer[offset + 1] = digits[byte & 0x0F]; \
    } \
    /* 随机数引擎函数指针 */ \
    ATTR LPREFIX##NAME SPREFIX##NAME##Generator(LPREFIX##NAME state) \
    { \
        LPREFIX##NAME result; \
        uint64_t x = state.high ^ state.low; \
        if (x == 0) x = 1; \
        x ^= x >> 12; \
        x ^= x << 25; \
        x ^= x >> 27; \
        result.high = x * 0x2545F4914F6CDD1DULL; \
        result.low = result.high ^ (result.high << 31) ^ (result.high >> 11); \
        return result; \
    } \
    ATTR void SPREFIX##NAME##Clampor(LPREFIX##NAME* value, LPREFIX##NAME min, LPREFIX##NAME max) \
    { \
        uint64_t hRange = max.high - min.high + 1; \
        (*value).high = min.high + ((*value).high % hRange); \
        uint64_t lRange = max.low - min.low + 1; \
        (*value).low = min.low + ((*value).low % lRange); \
    } \
    /* 转换 */ \
    ATTR LPREFIX##NAME SPREFIX##NAME##FromCstr(const char* str) \
    { \
        LPREFIX##NAME result = {0, 0}; \
        if (!str) return result; \
        size_t len = strlen(str); \
        if (len < 36) return result; \
        result.high |= SPREFIX##NAME##_GetByteInCstr(str, 0)    << 56; \
        result.high |= SPREFIX##NAME##_GetByteInCstr(str, 2)    << 48; \
        result.high |= SPREFIX##NAME##_GetByteInCstr(str, 4)    << 40; \
        result.high |= SPREFIX##NAME##_GetByteInCstr(str, 6)    << 32; \
        result.high |= SPREFIX##NAME##_GetByteInCstr(str, 9)    << 24; \
        result.high |= SPREFIX##NAME##_GetByteInCstr(str, 11)   << 16; \
        result.high |= SPREFIX##NAME##_GetByteInCstr(str, 14)   << 8; \
        result.high |= SPREFIX##NAME##_GetByteInCstr(str, 16); \
        result.low  |= SPREFIX##NAME##_GetByteInCstr(str, 19)   << 56; \
        result.low  |= SPREFIX##NAME##_GetByteInCstr(str, 21)   << 48; \
        result.low  |= SPREFIX##NAME##_GetByteInCstr(str, 24)   << 40; \
        result.low  |= SPREFIX##NAME##_GetByteInCstr(str, 26)   << 32; \
        result.low  |= SPREFIX##NAME##_GetByteInCstr(str, 28)   << 24; \
        result.low  |= SPREFIX##NAME##_GetByteInCstr(str, 30)   << 16; \
        result.low  |= SPREFIX##NAME##_GetByteInCstr(str, 32)   << 8; \
        result.low  |=  SPREFIX##NAME##_GetByteInCstr(str, 34); \
        return result; \
    } \
    ATTR const char* SPREFIX##NAME##ToCstr(LPREFIX##NAME me) \
    { \
        static char buf[37]; \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 0,   (me.high >> 56) & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 2,   (me.high >> 48) & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 4,   (me.high >> 40) & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 6,   (me.high >> 32) & 0xFF); \
        buf[8] = '-'; \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 9,   (me.high >> 24) & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 11,  (me.high >> 16) & 0xFF); \
        buf[13] = '-'; \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 14,  (me.high >> 8)  & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 16,  me.high         & 0xFF); \
        buf[18] = '-'; \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 19,  (me.low  >> 56) & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 21,  (me.low  >> 48) & 0xFF); \
        buf[23] = '-'; \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 24,  (me.low  >> 40) & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 26,  (me.low  >> 32) & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 28,  (me.low  >> 24) & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 30,  (me.low  >> 16) & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 32,  (me.low  >> 8)  & 0xFF); \
        SPREFIX##NAME##_WriteByteInBuffer(buf, 34,  me.low          & 0xFF); \
        buf[36] = '\0'; \
        return buf; \
    }
#pragma endregion

#endif