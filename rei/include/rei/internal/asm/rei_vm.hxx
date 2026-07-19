#include "rei/internal/asm/rei_opcode.hxx"
#include <string>

namespace rei
{
class VirtualMachine final
{
private:
    uint64_t regs_[Reg::REG_COUNT] {};
};
}
