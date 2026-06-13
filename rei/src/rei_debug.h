#ifndef REI_DEBUG_H
#define REI_DEBUG_H

#include <stdio.h>

#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_TRACE
#define REI_DEBUG_LOG_TRACE(msg) printf("[REI::TRACE]\t%s\n", msg)
#else
#define REI_DEBUG_LOG_TRACE
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_INFO
#define REI_DEBUG_LOG_INFO(msg) printf("[REI::INFO]\t%s\n", msg)
#else
#define REI_DEBUG_LOG_INFO
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_WARNING
#define REI_DEBUG_LOG_WARNING(msg) printf("[REI::WARNING]\t%s\n", msg)
#else
#define REI_DEBUG_LOG_WARNING
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_ERROR
#define REI_DEBUG_LOG_ERROR(msg) printf("[REI::ERROR]\t%s\n", msg)
#else
#define REI_DEBUG_LOG_ERROR
#endif
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_FATAL
#define REI_DEBUG_LOG_FATAL(msg) printf("[REI::FATAL]\t%s\n", msg)
#else
#define REI_DEBUG_LOG_FATAL
#endif

#endif
