#include "stl.hpp"
#include "vvmidi/vvmidi.hpp"

using namespace vvmidi;

namespace rei::stl
{
    Value::Data scale(REI_BYTECODE_TYPE argc, Value::Data argv[])
    {
        switch (argc)
        {
        case 0:
            return Integer(0);
            break;
        case 1:
            if (argv[0].toInteger() == 0)
                return Integer(-1);
            else
                return Integer(Scale<C_Major>::make({
                    static_cast<Degree>(argv[0].toInteger() - 1)
                }));
        case 2:
            if (argv[0].toInteger() == 0)
                return Integer(-1);
            else
                return Integer(Scale<C_Major>::make({
                    static_cast<Degree>(argv[0].toInteger() - 1),
                    static_cast<Accidental>(argv[1].toInteger())
                }));
        case 3:
            if (argv[0].toInteger() == 0)
                return Integer(-1);
            else
                return Integer(Scale<C_Major>::make({
                    static_cast<Degree>(argv[0].toInteger() - 1),
                    static_cast<Accidental>(argv[1].toInteger()),
                    static_cast<Octave>(argv[2].toInteger())
                }));
        default:
            break;
        }
        return Integer(-1);
    }
}
