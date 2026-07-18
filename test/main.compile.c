#include "rei/rei.h"
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
;

int main()
{
    printf("\x1b%%G");

    reiInitialize(NULL, NULL);

    ReiVM vm = reiVMCreate();
    printf("source:\n%s\n", script);
    ReiResult res = reiVMCompileModule(vm, script);
    switch (res)
    {
    case REI_SUCCESS:
        printf("REI_SUCCESS\n");
        break;
    case REI_ERROR_LEXER:
        printf("REI_ERROR_LEXER\n");
        break;
    case REI_ERROR_PARSER:
        printf("REI_ERROR_PARSER\n");
        break;
    case REI_ERROR_SEMANTIC:
        printf("REI_ERROR_SEMANTIC\n");
        break;
    case REI_ERROR_CODEGEN:
        printf("REI_ERROR_CODEGEN\n");
        break;
    case REI_ERROR_RUNTIME:
        printf("REI_ERROR_RUNTIME\n");
        break;
    }
    reiVMDestroy(vm);

    scanf("%c");
    return 0;
}
