#ifndef REI_DEBUG_RESULT_H
#define REI_DEBUG_RESULT_H

#include "rei.h"
#include <stdio.h>

static const char* reiStatusToCstr(ReiStatus me);
static void reiPrintStatus(const ReiStatus status);

static const char* reiStatusNames_g[REI_STATUS_UNDEFINED + 1] =
{
    "REI_STATUS_SUCCESS",
    "REI_STATUS_LEXER_ERROR",
    "REI_STATUS_PARSER_ERROR",
    "REI_STATUS_SEMANTIC_ERROR",
    "REI_STATUS_CODEGEN_ERROR",
    "REI_STATUS_UNDEFINED"
};

static const char* reiStatusToCstr(ReiStatus me)
{
    return reiStatusNames_g[me];
}
static void reiPrintStatus(ReiStatus status)
{
    printf("%s", reiStatusNames_g[status]);
}

#endif
