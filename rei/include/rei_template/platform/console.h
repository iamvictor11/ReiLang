#ifndef C_TEMPLATE_PLATFORM_CONSOLE_H
#define C_TEMPLATE_PLATFORM_CONSOLE_H

void osSetConsoleOutputCP_UTF8();
void osGetChar();

#ifdef C_TEMPLATE_IMPL_FILE

#include "kua.h"

void osSetConsoleOutputCP_UTF8()
{
#if C_TEMPLATE_PLATFORM_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
#elif C_TEMPLATE_PLATFORM_LINUX
#elif C_TEMPLATE_PLATFORM_MACOS
#else
#endif
}
void osGetChar()
{
#if C_TEMPLATE_PLATFORM_WINDOWS
    _getch();
#elif C_TEMPLATE_PLATFORM_LINUX
#elif C_TEMPLATE_PLATFORM_MACOS
#else
#endif
}

#endif

#endif
