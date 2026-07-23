#pragma once
#include "rei/internal/rei_result.hxx"
#include "rei/internal/rei_allocation_callbacks.hxx"
#include "rei/internal/asm/rei_image.hxx"
#include "rei/internal/compiler/rei_lexer.hxx"

namespace rei
{
class VirtualMachine final
{
private:
    ReiAllocationCallbacks callbacks_ {};
private:
    Lexer lexer_ {};
private:
    uint64_t regs_[Reg::REG_COUNT] {};
public:
    void allocationCallbacks(const ReiAllocationCallbacks& callbacks);
public:
    auto compile(const std::string& source) -> Result<Image>;
    void run(const Image& image);
};
}
