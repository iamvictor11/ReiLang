#include "stl.hpp"
#include "core/vm.hpp"
#include <chrono>

namespace rei
{
    void VM::bindSTL_()
    {
        bind("dump", stl::dump);
        bind("clock", stl::clock);
    }
    namespace stl
    {
        Value::Data dump(REI_BYTECODE_TYPE argc, Value::Data argv[])
        {
            for (size_t i = 0; i < argc; i++)
                printf("\033[1m\033[32m%03zu \033[34m%s\033[0m\n", i, argv[i].dump().c_str());
            return Nil{};
        }
        Value::Data clock(REI_BYTECODE_TYPE argc, Value::Data argv[])
        {
            auto now = std::chrono::system_clock::now();
            if (argc >= 1 && argv[0].isString())
            {
                const std::string& precision = argv[0].asString();
                if (precision == "ns")
                {
                    uint64_t timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>
                    (
                        now.time_since_epoch()
                    ).count();
                    return static_cast<Integer>(timestamp);
                }
                else if (precision == "us")
                {
                    uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>
                    (
                        now.time_since_epoch()
                    ).count();
                    return static_cast<Integer>(timestamp);
                }
                else if (precision == "ms")
                {
                    uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>
                    (
                        now.time_since_epoch()
                    ).count();
                    return static_cast<Integer>(timestamp);
                }
                else if (precision == "s")
                {
                    uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>
                    (
                        now.time_since_epoch()
                    ).count();
                    return static_cast<Integer>(timestamp);
                }
                else
                {
                    uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>
                    (
                        now.time_since_epoch()
                    ).count();
                    return static_cast<Integer>(timestamp);
                }
            }
            uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()
            ).count();
            return static_cast<Integer>(timestamp);
        }
    }
}
