#include "luna/luna.hpp"
#include <iostream>

void repl(luna::VM& vm)
{
    std::cout << "Luna 解释器" << std::endl;
    std::cout << "帮助 help" << std::endl;
    std::cout << "加载 load" << std::endl;
    std::cout << "运行 run" << std::endl;
    std::cout << "退出 quit" << std::endl;
    std::string line;
    while (true)
    {
        std::cout << "> ";
        if (!std::getline(std::cin, line) || line == "exit") break;
        if (line.empty()) continue;
        vm.loadFile(line);
    }
}

int main(int argc, char *argv[])
{
    luna::kua::setConsoleOutputCPToUTF8();
    luna::VM vm {};
    if (argc > 1)
    {
        vm.loadFile(argv[1]);
        vm.run();
    }
    else
    {
        vm.loadFile("test.luna");
        vm.run();
    }
    std::cout << "按 Enter 键退出...";
    std::cin.get();
    return 0;
}
