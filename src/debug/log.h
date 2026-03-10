#ifndef LUNA_DEBUG_LOG_H
#define LUNA_DEBUG_LOG_H
#include "vm/chunk.h"

void lunaChunk_debugInfo(luna_Chunk *self, const char *name);
void lunaValue_print(luna_Value *self);

#endif