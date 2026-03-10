#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include "luna/luna.h"

static void repl(luna_VM *vm);

int main(int argc, const char* argv[])
{
    setlocale(LC_ALL, "en_US.UTF-8");

    luna_VM vm;
    lunaVM_init(&vm);

    if (argc == 1)
    {
        repl(&vm);
    }
    else if (argc == 2)
    {
        lunaVM_loadFile(&vm, argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: luna [path]\n");
        exit(64);
    }

    lunaVM_free(&vm);

    printf("按任意键退出...");
    getchar();
    return EXIT_SUCCESS;
}

static void repl(luna_VM *vm)
{
  char line[1024];
  for (;;)
  {
    printf("> ");
    if (!fgets(line, sizeof(line), stdin))
    {
      printf("\n");
      break;
    }
    lunaVM_loadFile(vm, line);
  }
}
