#include "rei/rei.hpp"
#include <locale>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>

namespace fs = std::filesystem;

void menu_()
{
    std::cout << "load <PATH>   - 加载指定的 Lua 文件" << std::endl;
    std::cout << "run           - 运行当前加载的脚本" << std::endl;
    std::cout << "list          - 列出当前目录下的 .rei 文件" << std::endl;
    std::cout << "clear         - 清屏" << std::endl;
    std::cout << "help          - 显示此帮助信息" << std::endl;
    std::cout << "quit          - 退出解释器" << std::endl;
}
void list_()
{
    std::error_code ec;
    int count = 0;
    fs::recursive_directory_iterator it(".", ec), end;
    if (ec)
    {
        std::cout << "无法访问当前目录: " << ec.message() << std::endl;
        return;
    }
    for (; it != end; it.increment(ec))
    {
        if (ec)
        {
            std::cout << "遍历目录出错: " << ec.message() << std::endl;
            break;
        }
        const auto& entry = *it;
        auto path = entry.path();
        if (!entry.is_regular_file())
            continue;
        if (path.extension() == ".rei")
        {
            std::error_code ec0;
            auto clean = path.lexically_normal().generic_string();
            auto size = fs::file_size(path, ec0);
            std::cout << std::left << std::setw(50) << clean;
            if (ec0)
                std::cout << std::right << std::setw(10) << "(未知)" << std::endl;
            else
                std::cout << std::right << std::setw(10) << size << " byte" << std::endl;
            count++;
        }
    }
    if (count == 0)
        std::cout << "未找到任何 .rei 文件" << std::endl;
    else
        std::cout << "共 " << count << " 个文件\n" << std::endl;
}
void repl_(rei::VM& vm)
{
    std::cout << std::endl;
    std::cout << "╔════════════════════════════════╗" << std::endl;
    std::cout << "║     REI 交互式虚拟机 v1.0      ║" << std::endl;
    std::cout << "╚════════════════════════════════╝" << std::endl;
    menu_();
    std::string line;
    std::string currentFile = "";
    while (true)
    {
        std::cout << "\n[" << (currentFile.empty() ? "未加载" : currentFile) << "] > ";
        
        if (!std::getline(std::cin, line))
            break;
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        if (line.empty())
            continue;
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        for (auto& c : cmd) c = tolower(c);
        if (cmd == "quit")
        {
            std::cout << ":) 拜拜！" << std::endl;
            break;
        }
        else if (cmd == "help")
        {
            menu_();
        }
        else if (cmd == "load")
        {
            std::string filename;
            iss >> filename;
            if (filename.empty())
            {
                std::cout << "请指定文件名" << std::endl;
                continue;
            }
            std::error_code ec;
            if (!fs::exists(filename, ec))
            {
                if (ec)
                    std::cout << "文件检查失败: " << ec.message() << std::endl;
                else
                    std::cout << "文件不存在: " << filename << std::endl;
                continue;
            }
            if (!fs::is_regular_file(filename, ec))
            {
                std::cout << "不是有效文件: " << filename << std::endl;
                continue;
            }
            if (!vm.loadFile(filename))
            {
                std::cout << ":( 加载失败: " << filename << std::endl;
                continue;
            }
            currentFile = filename;
            std::cout << ":) 加载成功: " << filename << std::endl;
        }
        else if (cmd == "run")
        {
            if (currentFile.empty())
            {
                std::cout << "没有加载任何文件，请先使用 load 命令" << std::endl;
                continue;
            }
            vm.run();
        }
        else if (cmd == "list")
        {
            list_();
        }
        else if (cmd == "clear")
        {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            menu_();
        }
        else
        {
            std::cout << "未知命令: " << cmd << "，输入 'help' 查看可用命令" << std::endl;
        }
    }
}
int main(int argc, char *argv[])
{
    std::setlocale(LC_ALL, "en_US.UTF-8");
    rei::VM vm {};
    if (argc > 1)
    {
        std::string filename = argv[1];
        if (!vm.loadFile(filename))
        {
            std::cout << ":( 加载失败: " << filename << std::endl;
            return 1;
        }
        vm.run();
    }
    else
    {
        repl_(vm);
    }
    std::cout << "\n按 Enter 键退出...";
    std::cin.get();
    return 0;
}
