#include "rei_vm.h"
#include "../lexer/rei_lexer.h"

ReiStatus reiVMCompileModule(ReiVM me, const char* source)
{
    ReiLexer lexer;
    reiLexerInit(&lexer, source);
    
    reiLexerFree(&lexer);
}
ReiStatus reiVMLoadModule(ReiVM me, const ReiBytecode* code)
{
}
ReiStatus reiVMRunModule(ReiVM me)
{
}
const ReiBytecode* reiVMCacheModule(ReiVM me)
{
}
