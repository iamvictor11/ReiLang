#ifndef REI_LEXER_TOKEN_DEBUG_I
#define REI_LEXER_TOKEN_DEBUG_I

#include "rei_token.h"

const char* reiTokenTypeToCstr(ReiTokenType type);
char* reiTokenToCstr(const ReiToken* me, char* buff, size_t len);

#endif
