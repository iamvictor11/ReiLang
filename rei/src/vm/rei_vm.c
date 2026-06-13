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

ReiResult reiVMCompileModule(ReiVM me, const char* source)
{
    ReiResult res;
    ReiLexer lexer;
    printf("source: %s\n", source);
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
