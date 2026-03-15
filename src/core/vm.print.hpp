#include "core/base/common.hpp"
#include <cmath>
#include <limits>

namespace luna
{
    static void _printChunk(Chunk* chunk)
    {
        for (size_t i = 0; i < chunk->codes.size(); i++)
        {
            auto instruction = static_cast<Opcode>(chunk->codes[i]);
            switch (instruction)
            {
                case OP_CONSTANT: i++; printf("CONSTANT %s\n", Value::getDebugString(chunk->constants[chunk->codes[i]]).c_str()); break;
                case OP_POP: printf("POP\n"); break;
                case OP_NIL: printf("NIL\n"); break;
                case OP_TRUE: printf("TRUE\n"); break;
                case OP_FALSE: printf("FALSE\n"); break;
                case OP_NEG: printf("NEG\n"); break;
                case OP_ADD: printf("ADD\n"); break;
                case OP_SUB: printf("SUB\n"); break;
                case OP_MUL: printf("MUL\n"); break;
                case OP_DIV: printf("DIV\n"); break;
                case OP_MOD: printf("MOD\n"); break;
                case OP_POW: printf("POW\n"); break;
                case OP_BIT_AND: printf("BIT AND\n"); break;
                case OP_BIT_OR: printf("BIT OR\n"); break;
                case OP_BIT_XOR: printf("BIT XOR\n"); break;
                case OP_BIT_XNOR: printf("BIT XNOR\n"); break;
                case OP_BIT_SHL: printf("BIT SHL\n"); break;
                case OP_BIT_SHR: printf("BIT SHR\n"); break;
                case OP_EQ: printf("EQ\n"); break;
                case OP_NE: printf("NE\n"); break;
                case OP_LT: printf("LT\n"); break;
                case OP_LE: printf("LE\n"); break;
                case OP_GT: printf("GT\n"); break;
                case OP_GE: printf("GE\n"); break;
                case OP_AND: printf("AND\n"); break;
                case OP_OR: printf("OR\n"); break;
                case OP_PRINT: printf("PRINT\n"); break;
                case OP_PRINTLN: printf("PRINTLN\n"); break;
                case OP_RETURN: printf("RETURN\n"); break;
                case OP_JUMP: printf("JUMP\n"); break;
                case OP_JUMP_IF_FALSE: printf("JUMP IF FALSE\n"); break;
                case OP_LOOP: printf("LOOP\n"); break;
                case OP_DEF_GLOBAL: i++; printf("DEF GLOBAL %s\n", Value::getDebugString(chunk->constants[chunk->codes[i]]).c_str()); break;
                case OP_GET_GLOBAL: i++; printf("GET GLOBAL %s\n", Value::getDebugString(chunk->constants[chunk->codes[i]]).c_str()); break;
                case OP_SET_GLOBAL: printf("SET GLOBAL\n"); break;
                case OP_DEF_LOCAL: printf("DEFINE LOCAL\n"); break;
                case OP_GET_LOCAL: printf("GET LOCAL\n"); break;
                case OP_SET_LOCAL: printf("SET LOCAL\n"); break;
                case OP_CALL: printf("CALL\n"); break;
                default: break;
            }
        }
    }
}
