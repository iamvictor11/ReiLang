#ifndef REI_INTERNAL_LEXER_TOKEN_DEBUG_I
#define REI_INTERNAL_LEXER_TOKEN_DEBUG_I

#include "rei/internal/lexer/rei_token.h"

const char* reiTokenKindToCstr(ReiTokenKind type);
char* reiTokenToCstr(const ReiToken* me, char* buff, size_t len);

#endif
