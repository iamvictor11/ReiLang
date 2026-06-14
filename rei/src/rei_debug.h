#ifndef REI_DEBUG_H
#define REI_DEBUG_H

#include <stdio.h>
#include <assert.h>

#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_TRACE
#define REI_DEBUG_LOG_TRACE(MSG) printf("[REI::TRACE]\t%s\n", MSG)
#else
#define REI_DEBUG_LOG_TRACE
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_INFO
#define REI_DEBUG_LOG_INFO(MSG) printf("[REI::INFO]\t%s\n", MSG)
#else
#define REI_DEBUG_LOG_INFO
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_WARNING
#define REI_DEBUG_LOG_WARNING(MSG) printf("[REI::WARNING]\t%s\n", MSG)
#else
#define REI_DEBUG_LOG_WARNING
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_ERROR
#define REI_DEBUG_LOG_ERROR(MSG) printf("[REI::ERROR]\t%s\n", MSG)
#else
#define REI_DEBUG_LOG_ERROR
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_FATAL
#define REI_DEBUG_LOG_FATAL(MSG) printf("[REI::FATAL]\t%s\n", MSG)
#else
#define REI_DEBUG_LOG_FATAL
#endif
#define REI_DEBUG_LOG_ASSERT(EXPR, MSG) \
    do \
    { \
        if (!(EXPR)) \
        { \
            printf("[REI::ASSERT]\t%s\n", MSG); \
            assert(EXPR); \
        } \
    } while (false)

#endif
