#include "kua.hpp"
#include <windows.h>

namespace luna::kua
{
    void setConsoleOutputCPToUTF8()
    {
        SetConsoleOutputCP(CP_UTF8);
    }
}
