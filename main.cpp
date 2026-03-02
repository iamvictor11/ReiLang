#include "luna/luna.hpp"
#include <windows.h>
#include <iostream>

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    if (argc == 1)
    {
    }
    else
    {
        for (int i = 0; i < argc; i++)
            std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }
    std::cout << "按任意键退出...";
    std::cin.get();
    return 0;
}
