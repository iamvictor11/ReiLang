#include "stl.hpp"
#include "core/vm.hpp"

namespace rei
{
    void VM::bindSTL_()
    {
        /* util */
        bind("dump", stl::dump);
        bind("clock", stl::clock);
        bind("len", stl::len);
    }
}
