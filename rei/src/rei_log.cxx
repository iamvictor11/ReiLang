#include "rei/internal/rei_log.hxx"
#include <iostream>
#include <cassert>

#define REI_ANSI_BOLD_        "\033[1m"
#define REI_ANSI_FG_YELLOW_   "\033[33m"
#define REI_ANSI_FG_RED_      "\033[31m"
#define REI_ANSI_FG_MAGENTA_  "\033[35m"
#define REI_ANSI_RESET_       "\033[0m"

namespace rei
{
    void warning(const std::string& msg, const char* file, int line)
    {
        std::cout << REI_ANSI_BOLD_ REI_ANSI_FG_YELLOW_ << "[rei::warning]" << REI_ANSI_RESET_ << msg
            << std::format("<{}:{}>", file, line) << std::endl;
    }
    void error(const std::string& msg, const char* file, int line)
    {
        std::cout << REI_ANSI_BOLD_ REI_ANSI_FG_RED_ << "[rei::error]" << REI_ANSI_RESET_ << msg
            << std::format("<{}:{}>", file, line) << std::endl;
    }
    void zassert(bool expr, const std::string& msg, const char* file, int line)
    {
        std::string amsg {};
        amsg = amsg + (REI_ANSI_BOLD_ REI_ANSI_FG_MAGENTA_) + "[rei::assert]" + REI_ANSI_RESET_ + msg
            + std::format("<{}:{}>", file, line) + "\n";
        assert(expr && amsg.c_str());
    }
}

#undef REI_ANSI_BOLD_
#undef REI_ANSI_FG_YELLOW_
#undef REI_ANSI_FG_RED_
#undef REI_ANSI_FG_MAGENTA_
#undef REI_ANSI_RESET_
