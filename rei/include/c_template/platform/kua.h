#ifndef C_TEMPLATE_PLATFORM_KUA_H
#define C_TEMPLATE_PLATFORM_KUA_H

#if defined(_WIN32) || defined(_WIN64)
    #define C_TEMPLATE_PLATFORM_WINDOWS 1
    #define C_TEMPLATE_PLATFORM_NAME "Windows"
    #include <windows.h>
    #include <conio.h>
#elif defined(__linux__) || defined(__linux)
    #define C_TEMPLATE_PLATFORM_LINUX 1
    #define C_TEMPLATE_PLATFORM_NAME "Linux"
    #include <unistd.h>
#elif defined(__APPLE__) && defined(__MACH__)
    #define C_TEMPLATE_PLATFORM_MACOS 1
    #define C_TEMPLATE_PLATFORM_NAME "macOS"
    #include <mach/mach.h>
#else
    #define C_TEMPLATE_PLATFORM_UPNNOWN 1
    #define C_TEMPLATE_PLATFORM_NAME "Unknown"
    #warning "Unknown platform, using POSIX compatibility mode"
#endif

#endif
