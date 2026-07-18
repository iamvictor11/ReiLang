#include "rei/internal/vm/rei_vm.h"
#include "rei/internal/rei_allocator.h"
#include "rei/internal/lexer/rei_lexer.h"

REI_API ReiVM reiVMCreate(void)
{
    ReiVM me = reiMalloc(ReiVM_T, 1);
    if (me == NULL) return NULL;
    me->cache = NULL;
    return me;
}
REI_API void reiVMDestroy(ReiVM me)
{
    if (me == NULL) return;
    if (me->cache)
        reiFree(me->cache);
    reiFree(me);
}
#pragma region Module
REI_API ReiResult reiVMCompile(ReiVM me, const char* source)
{
    ReiLexer lexer;
    // ReiParser parser;
    if (!reiLexerInit(&lexer, source))
    {
        reiLexerFree(&lexer);
        return REI_ERROR_LEXER;
    }
    ReiResult res;
    res = reiLexerStart(&lexer);
    if (res != REI_SUCCESS) goto REI_VM_COMPILE_MODULE_FUNC_FREE;
REI_VM_COMPILE_MODULE_FUNC_FREE:
    // reiParserFree(&parser);
    reiLexerFree(&lexer);
    return res;
}
REI_API ReiResult reiVMLoad(ReiVM me)
{
}
REI_API ReiResult reiVMRun(ReiVM me)
{
}
#pragma endregion
