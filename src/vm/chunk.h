#ifndef LUNA_COMMON_CHUNK_H
#define LUNA_COMMON_CHUNK_H
#include "common/value.h"

void lunaChunk_init(luna_Chunk *self);
void lunaChunk_free(luna_Chunk *self);
size_t lunaChunk_writeConstant(luna_Chunk *self, luna_Value element);
void lunaChunk_writeBytecode(luna_Chunk *self, luna_Byte element, luna_Position pos);

#endif