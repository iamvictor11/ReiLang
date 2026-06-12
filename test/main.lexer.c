#include "rei/rei.debug.result.h"
#include "rei/rei.debug.token.h"
#include "../rei/src/lexer/rei_lexer.h"
#include REI_C_TEMPLATE_LIB_PLATFORM_CONSOLE_H

#define TAB4 "    "

static const char* script =
"val := -12e+3;\n"
"'hajimi'\n"
"while (true)\n"
"{\n"
TAB4 "break;\n"
"}"
;

ReiAllocator reiAllocator_g;

int main()
{
    osSetConsoleOutputCP_UTF8();
    reiAllocator_g = reiDefaultAllocator();

    ReiLexer lexer;
    printf("script: %s\n", script);
    reiLexerInit(&lexer, script);
    ReiResult res = reiLexerStart(&lexer);
    if (res.status == REI_STATUS_SUCCESS)
    {
        printf("tokens: %zu\n", lexer.tokens->size);
        C_TEMPLATE_VECTOR_FOREACH(ReiToken, token, lexer.tokens)
        {
            reiPrintToken(token);
            printf("\n");
        }
    }
    reiPrintStatus(res.status);
    printf("\n");
    reiLexerFree(&lexer);

    osGetChar();
    return EXIT_SUCCESS;
}
