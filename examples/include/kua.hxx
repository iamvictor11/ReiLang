#pragma once
#include <iostream>
#include "rei/rei.hxx"

static inline void fs_useUTF8()
{
    printf("\x1b%%G");
}
static inline void fs_waitEnter()
{
    std::cin.get();
}
