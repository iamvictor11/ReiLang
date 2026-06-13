#include "rei/rei.h"
#include REI_C_TEMPLATE_LIB_PLATFORM_CONSOLE_H
#include <stdio.h>

#define TAB4 "    "

static const char* script =
"val := -12e+3;\n"
"'hajimi'\n"
"while (true)\n"
"{\n"
TAB4 "break;\n"
"}"
;

int main()
{
    osSetConsoleOutputCP_UTF8();
    reiInitialize(NULL);

    ReiVM vm = reiVMCreate();
    ReiResult res = reiVMCompileModule(vm, script);
    switch (res.status)
    {
    case REI_STATUS_SUCCESS:
        printf("REI_STATUS_SUCCESS\n");
        break;
    case REI_STATUS_LEXER_ERROR:
        printf("REI_STATUS_LEXER_ERROR\n");
        break;
    case REI_STATUS_PARSER_ERROR:
        printf("REI_STATUS_PARSER_ERROR\n");
        break;
    case REI_STATUS_SEMANTIC_ERROR:
        printf("REI_STATUS_SEMANTIC_ERROR\n");
        break;
    case REI_STATUS_CODEGEN_ERROR:
        printf("REI_STATUS_CODEGEN_ERROR\n");
        break;
    case REI_STATUS_RUNTIME_ERROR:
        printf("REI_STATUS_RUNTIME_ERROR\n");
        break;
    }
    reiVMDestroy(vm);

    osGetChar();
    return EXIT_SUCCESS;
}
