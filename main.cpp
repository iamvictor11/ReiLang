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

    // std::cout << "打印抽象语法树：" << std::endl;
    // luna::ast::NRef ln = luna::ast::make_ref(luna::Expr::Literal(1));
    // luna::ast::NRef rn = luna::ast::make_ref(luna::Expr::Literal(2));
    // luna::ast::NRef bn = luna::ast::make_ref(luna::Expr::Binary(std::move(ln), luna::Token::TK_ADD, std::move(rn)));
    // luna::ast::Printer printer{};
    // printer(bn->tempRef());

    luna::VM vm {};
    vm.runFile("test.luna");

    std::cout << "按 Enter 键退出...";
    std::cin.get();
    return 0;
}
