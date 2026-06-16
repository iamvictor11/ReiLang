#ifndef REI_DEBUG_H
#define REI_DEBUG_H

#include <stdbool.h>

typedef struct ReiDebugState
{
    bool hasError;
} ReiDebugState;

extern ReiDebugState reiDebugState_g;

#define REI_HAS_ERROR (reiDebugState_g.hasError)

void reiDebugLogTrace(const char* message);
void reiDebugLogInfo(const char* message);
void reiDebugLogWarning(const char* message);
void reiDebugLogError(const char* message);
void reiDebugLogFatal(const char* message);
void reiDebugLogAssert(bool expr, const char* message);

#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_TRACE
#define REI_DEBUG_LOG_TRACE(MSG) reiDebugLogTrace(MSG)
#else
#define REI_DEBUG_LOG_TRACE
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_INFO
#define REI_DEBUG_LOG_INFO(MSG) reiDebugLogInfo(MSG)
#else
#define REI_DEBUG_LOG_INFO
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_WARNING
#define REI_DEBUG_LOG_WARNING(MSG) reiDebugLogWarning(MSG)
#else
#define REI_DEBUG_LOG_WARNING
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_ERROR
#define REI_DEBUG_LOG_ERROR(MSG) reiDebugLogError(MSG)
#else
#define REI_DEBUG_LOG_ERROR
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_FATAL
#define REI_DEBUG_LOG_FATAL(MSG) reiDebugLogFatal(MSG)
#else
#define REI_DEBUG_LOG_FATAL
#endif
#define REI_DEBUG_LOG_ASSERT(EXPR, MSG) reiDebugLogAssert(EXPR, MSG)

#endif
