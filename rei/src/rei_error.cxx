#include "rei/internal/rei_error.hxx"
#include "rei/internal/rei_log.hxx"
#include <stdexcept>

namespace rei
{
    auto Error::format() const -> std::string
    {
        std::string res = title + ": " +
            message +
            "[" + string_ReiErrorCode(code) + "]"
        ;
        return res;
    }
    void Error::report()
    {
        #if REI_WITH_EXCEPTIONS
            throw std::runtime_error(format());
        #else
            REI_LOG_ERROR("{}", format());
        #endif
    }
}
