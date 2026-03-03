#include "luna/luna.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    luna::kua::setConsoleOutputCPToUTF8();
    if (argc == 1)
    {
    }
    else
    {
        for (int i = 0; i < argc; i++)
            std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }

    luna::VM vm {};
    vm.runFile("test.luna");

    std::cout << "按 Enter 键退出...";
    std::cin.get();
    return 0;
}
