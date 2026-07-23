#include "kua.hxx"

const char* rei_code =
"int abc;"
;

int main(int argc, char *argv[])
{
    fs_useUTF8();

    rei::VirtualMachine vm {};
    vm.compile(rei_code);

    fs_waitEnter();
    return 0;
}
