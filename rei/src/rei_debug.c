#include "rei_debug.h"
#include "rei_callbacks.h"
#include <stdio.h>
#include <assert.h>

ReiDebugState reiDebugState_g = {0};

void reiDebugLogTrace(const char* message)
{
    if (reiCallbacks_g.debug) reiDebug(REI_DEBUG_LEVEL_TRACE, message);
    else printf("[REI::TRACE]\t%s\n", message);
}
void reiDebugLogInfo(const char* message)
{
    if (reiCallbacks_g.debug) reiDebug(REI_DEBUG_LEVEL_INFO, message);
    else printf("[REI::INFO]\t%s\n", message);
}
void reiDebugLogWarning(const char* message)
{
    if (reiCallbacks_g.debug) reiDebug(REI_DEBUG_LEVEL_WARNING, message);
    else printf("[REI::WARNING]\t%s\n", message);
}
void reiDebugLogError(const char* message)
{
    reiDebugState_g.hasError = true;
    if (reiCallbacks_g.debug) reiDebug(REI_DEBUG_LEVEL_ERROR, message);
    else printf("[REI::ERROR]\t%s\n", message);
}
void reiDebugLogFatal(const char* message)
{
    reiDebugState_g.hasError = false;
    if (reiCallbacks_g.debug) reiDebug(REI_DEBUG_LEVEL_FATAL, message);
    else printf("[REI::FATAL]\t%s\n", message);
}
void reiDebugLogAssert(bool expr, const char* message)
{
    if (expr) return;
    printf("[REI::ASSERT]\t%s\n", message);
    assert(false);
}
