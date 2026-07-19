#pragma once
#include <cstdint>

namespace rei
{
namespace Opcode
{
enum E
{
    NOP,
    MOV, MOVK,
    LOAD, LOADK,
    STOR, STORK,
    UADD, IADD, FADD,
    USUB, ISUB, FSUB,
    UMUL, IMUL, FMUL,
    UDIV, IDIV, FDIV,
    UMOD, IMOD, FMOD,
    UCMP, ICMP, FCMP,
    UADDK, IADDK, FADDK,
    USUBK, ISUBK, FSUBK,
    UMULK, IMULK, FMULK,
    UDIVK, IDIVK, FDIVK,
    UMODK, IMODK, FMODK,
    UCMPK, ICMPK, FCMPK,
    SHL, SHR, SAR,
    SHLK, SHRK, SARK,
    BAND, BANDK,
    BOR, BORK,
    BXOR, BXORK,
    BNOT, BNOTK,
    U2I, I2U, I2F, F2I,
    UEX, IEX,
    JMP,
    JMPE, JMPNE,
    JMPA, JMPB,
    JMPL, JMPG,
    CALL,
    RET,
    HALT
};
}
namespace Reg
{
enum E
{
    R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
    T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    IP, SP, FP, LR,
    REG_COUNT
};
template<int N> constexpr E ARG = static_cast<E>(R0 + N);
}
}
