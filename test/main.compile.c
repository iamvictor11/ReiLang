#include "rei/rei.h"
#include REI_C_TEMPLATE_LIB_PLATFORM_CONSOLE_H
#include <stdio.h>

#define TAB4 "    "

static const char* script =
"kon val = -12e+3;\n"
"mut var = 11;\n"
"hajimi := '哈基米'\n"
"while (true)\n"
"{\n"
TAB4 "break;\n"
"}\n"
"rename val vaal;"
;

int main()
{
    osSetConsoleOutputCP_UTF8();
    reiInitialize(NULL, NULL);

    ReiVM vm = reiVMCreate();
    printf("source:\n%s\n", script);
    ReiResult res = reiVMCompileModule(vm, script);
    switch (res)
    {
    case REI_RESULT_SUCCESS:
        printf("REI_RESULT_SUCCESS\n");
        break;
    case REI_RESULT_LEXER_ERROR:
        printf("REI_RESULT_LEXER_ERROR\n");
        break;
    case REI_RESULT_PARSER_ERROR:
        printf("REI_RESULT_PARSER_ERROR\n");
        break;
    case REI_RESULT_SEMANTIC_ERROR:
        printf("REI_RESULT_SEMANTIC_ERROR\n");
        break;
    case REI_RESULT_CODEGEN_ERROR:
        printf("REI_RESULT_CODEGEN_ERROR\n");
        break;
    case REI_RESULT_RUNTIME_ERROR:
        printf("REI_RESULT_RUNTIME_ERROR\n");
        break;
    }
    reiVMDestroy(vm);

    osGetChar();
    return EXIT_SUCCESS;
}
