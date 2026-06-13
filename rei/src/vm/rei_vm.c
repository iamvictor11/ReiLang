#include "rei_vm.h"
#include "../rei_allocator.h"
#include "../lexer/rei_lexer.h"

ReiVM reiVMCreate(void)
{
    ReiVM me = (ReiVM)reiAllocator_g.malloc(reiAllocator_g.context, sizeof(ReiVM_T));
    if (me == NULL) return NULL;
    me->cache = NULL;
    return me;
}
void reiVMDestroy(ReiVM me)
{
    if (me == NULL) return;
    if (me->cache)
        reiAllocator_g.free(reiAllocator_g.context, me->cache);
    reiAllocator_g.free(reiAllocator_g.context, me);
}

ReiResult reiVMCompileModule(ReiVM me, const char* source)
{
    ReiLexer lexer;
    printf("source: %s\n", source);
    reiLexerInit(&lexer, source);
    reiLexerStart(&lexer);



    reiLexerFree(&lexer);
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
