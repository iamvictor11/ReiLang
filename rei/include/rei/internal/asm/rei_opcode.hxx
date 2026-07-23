#pragma once
#include <cstdint>

namespace rei
{
namespace Opcode
{
enum E : uint8_t
{
    OP_NOP,
    OP_MOV, OP_MOVK,
    OP_LOAD, OP_LOADK,
    OP_STOR, OP_STORK,
    OP_UADD, OP_IADD, OP_FADD,
    OP_USUB, OP_ISUB, OP_FSUB,
    OP_UMUL, OP_IMUL, OP_FMUL,
    OP_UDIV, OP_IDIV, OP_FDIV,
    OP_UMOD, OP_IMOD, OP_FMOD,
    OP_UCMP, OP_ICMP, OP_FCMP,
    OP_UADDK, OP_IADDK, OP_FADDK,
    OP_USUBK, OP_ISUBK, OP_FSUBK,
    OP_UMULK, OP_IMULK, OP_FMULK,
    OP_UDIVK, OP_IDIVK, OP_FDIVK,
    OP_UMODK, OP_IMODK, OP_FMODK,
    OP_UCMPK, OP_ICMPK, OP_FCMPK,
    OP_SHL, OP_SHR, OP_SAR,
    OP_SHLK, OP_SHRK, OP_SARK,
    OP_BAND, OP_BANDK,
    OP_BOR, OP_BORK,
    OP_BXOR, OP_BXORK,
    OP_BNOT, OP_BNOTK,
    OP_U2I, OP_I2U, OP_I2F, OP_F2I,
    OP_UEX, OP_IEX,
    OP_JMP,
    OP_JMPE, OP_JMPNE,
    OP_JMPA, OP_JMPB,
    OP_JMPL, OP_JMPG,
    OP_CALL,
    OP_RET,
    OP_HALT,
    OPCODE_COUNT
};
}
namespace Reg
{
enum E : uint8_t
{
    RG_R0, RG_R1, RG_R2, RG_R3, RG_R4, RG_R5, RG_R6, RG_R7, RG_R8, RG_R9, RG_R10, RG_R11, RG_R12, RG_R13, RG_R14, RG_R15,
    RG_T0, RG_T1, RG_T2, RG_T3, RG_T4, RG_T5, RG_T6, RG_T7, RG_T8, RG_T9, RG_T10, RG_T11, RG_T12, RG_T13, RG_T14, RG_T15,
    RG_IP, RG_SP, RG_FP, RG_LR,
    REG_COUNT
};
template<int N> constexpr E ARG = static_cast<E>(R0 + N);
}
namespace ConditionCode
{
enum F : uint8_t
{
    CC_00 = 0,
    CC_ZF = 1 << 0,
    CC_SF = 1 << 1,
    CC_OF = 1 << 2,
    CC_CF = 1 << 3,
    CC_AF = 1 << 4,
    CC_PF = 1 << 5,
    CC_ALL = UINT8_MAX
};
}
}
