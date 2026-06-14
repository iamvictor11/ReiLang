#include "rei_vm.h"
#include "../rei_allocator.h"
#include "../lexer/rei_lexer.h"

ReiVM reiVMCreate(void)
{
    ReiVM me = reiMalloc(ReiVM_T, 1);
    if (me == NULL) return NULL;
    me->cache = NULL;
    return me;
}
void reiVMDestroy(ReiVM me)
{
    if (me == NULL) return;
    if (me->cache)
        reiFree(me->cache);
    reiFree(me);
}
#pragma region Module
ReiResult reiVMCompileModule(ReiVM me, const char* source)
{
    ReiResult res;
    ReiLexer lexer;
    reiLexerInit(&lexer, source);
    res = reiLexerStart(&lexer);
    if (res != REI_RESULT_SUCCESS) return res;
    reiLexerFree(&lexer);
    return res;
}
ReiResult reiVMLoadModule(ReiVM me, const ReiBytecode* code)
{
}
ReiResult reiVMRunModule(ReiVM me)
{
}
const ReiBytecode* reiVMCacheModule(ReiVM me)
{
}
#pragma endregion
#pragma region Native
bool reiIsBoolean(ReiVM vm, uint32_t i);
bool reiIsInteger(ReiVM vm, uint32_t i);
bool reiIsFloating(ReiVM vm, uint32_t i);
bool reiIsString(ReiVM vm, uint32_t i);

bool        reiAtBoolean(ReiVM vm, uint32_t i);
int64_t     reiAtInteger(ReiVM vm, uint32_t i);
double      reiAtFloating(ReiVM vm, uint32_t i);
const char* reiAtString(ReiVM vm, uint32_t i);

bool        reiToBoolean(ReiVM vm, uint32_t i);
int64_t     reiToInteger(ReiVM vm, uint32_t i);
double      reiToFloating(ReiVM vm, uint32_t i);
const char* reiToString(ReiVM vm, uint32_t i);

bool        reiRvBoolean(ReiVM vm, bool v);
int64_t     reiRvInteger(ReiVM vm, bool v);
double      reiRvFloating(ReiVM vm, bool v);
const char* reiRvString(ReiVM vm, bool v);
#pragma endregion
