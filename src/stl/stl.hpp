#pragma once
#include "core/base/common.hpp"

namespace rei::stl
{
    /* util */
    Value::Data dump(REI_BYTECODE_TYPE argc, Value::Data argv[]);
    Value::Data clock(REI_BYTECODE_TYPE argc, Value::Data argv[]);
    Value::Data len(REI_BYTECODE_TYPE argc, Value::Data argv[]);
    /* midi */
    Value::Data scale(REI_BYTECODE_TYPE argc, Value::Data argv[]);
}
