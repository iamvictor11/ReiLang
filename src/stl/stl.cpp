#include "stl.hpp"

namespace rei::stl
{
    Value::Data dump(REI_BYTECODE_TYPE argc, Value::Data argv[])
    {
        printf("rei::stl::dump()\nargc %03zu argv %p\n",
            argc, argv);
        for (size_t i = 0; i < argc; i++)
            printf("\033[1m\033[32m%03zu \033[34m%s\033[0m\n", i, Value::getDebugString(argv[i]).c_str());
        return Nil{};
    }
    Value::Data now(REI_BYTECODE_TYPE argc, Value::Data argv[])
    {
        return Nil();
    }
}
