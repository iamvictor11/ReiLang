#include "core/base/common.hpp"
#include <cmath>
#include <limits>

namespace rei
{
    static void chunk_debugPrint_(const Chunk& chunk, size_t level = 0)
    {
        for (size_t i = 0;i < chunk.codes.size();i++)
        {
            auto instruction = static_cast<Opcode>(chunk.codes[i]);
            for (size_t j = 0; j < level; j++)
                printf("    ");
            printf("\033[1m\033[90m%04zu\033[0m", i);
            switch (instruction)
            {
                case OP_CONSTANT:
                {
                    i++;
                    Bytecode ci = chunk.codes[i];
                    printf("%-12s\033[1m\033[32m%04zu \033[34m%s\033[0m\n", "CONSTANT", ci, Value::getDebugString(chunk.constants[ci]).c_str());
                    if (Value::is<Ref<Function>>(chunk.constants[ci]))
                    {
                        auto func_ref = Value::as<Ref<Function>>(chunk.constants[ci]);
                        chunk_debugPrint_(func_ref->chunk, level + 1);
                    }
                    break;
                }
                case OP_POP:
                    printf("%-12s\n", "POP");
                    break;
                case OP_NIL:
                    printf("%-12s\n", "NIL");
                    break;
                case OP_TRUE:
                    printf("%-12s\n", "TRUE");
                    break;
                case OP_FALSE:
                    printf("%-12s\n", "FALSE");
                    break;
                case OP_NEG:
                    printf("%-12s\n", "NEG");
                    break;
                case OP_ADD:
                    printf("%-12s\n", "ADD");
                    break;
                case OP_SUB:
                    printf("%-12s\n", "SUB");
                    break;
                case OP_MUL:
                    printf("%-12s\n", "MUL");
                    break;
                case OP_DIV:
                    printf("%-12s\n", "DIV");
                    break;
                case OP_MOD:
                    printf("%-12s\n", "MOD");
                    break;
                case OP_POW:
                    printf("%-12s\n", "POW");
                    break;
                case OP_BIT_AND:
                    printf("%-12s\n", "BIT AND");
                    break;
                case OP_BIT_OR:
                    printf("%-12s\n", "BIT OR");
                    break;
                case OP_BIT_XOR:
                    printf("%-12s\n", "BIT XOR");
                    break;
                case OP_BIT_XNOR:
                    printf("%-12s\n", "BIT XNOR");
                    break;
                case OP_BIT_SHL:
                    printf("%-12s\n", "BIT SHL");
                    break;
                case OP_BIT_SHR:
                    printf("%-12s\n", "BIT SHR");
                    break;
                case OP_EQ:
                    printf("%-12s\n", "EQ");
                    break;
                case OP_NE:
                    printf("%-12s\n", "NE");
                    break;
                case OP_LT:
                    printf("%-12s\n", "LT");
                    break;
                case OP_LE:
                    printf("%-12s\n", "LE");
                    break;
                case OP_GT:
                    printf("%-12s\n", "GT");
                    break;
                case OP_GE:
                    printf("%-12s\n", "GE");
                    break;
                case OP_AND:
                    i++;
                    printf("%-12s\033[1m\033[32m%04zu\033[0m\n", "AND", chunk.codes[i]);
                    break;
                case OP_OR:
                    i++;
                    printf("%-12s\033[1m\033[32m%04zu\033[0m\n", "OR", chunk.codes[i]);
                    break;
                case OP_BEG:
                    printf("%-12s\n", "BEG");
                    break;
                case OP_END:
                    printf("%-12s\n", "END");
                    break;
                case OP_PRINT:
                    printf("%-12s\n", "PRINT");
                    break;
                case OP_PRINTLN:
                    printf("%-12s\n", "PRINTLN");
                    break;
                case OP_JUMP:
                    i++;
                    printf("%-12s\033[1m\033[32m%04d\033[0m\n", "JUMP", chunk.codes[i]);
                    break;
                case OP_JMPT:
                    i++;
                    printf("%-12s\033[1m\033[32m%04d\033[0m\n", "JMPT", chunk.codes[i]);
                    break;
                case OP_JMPF:
                    i++;
                    printf("%-12s\033[1m\033[32m%04d\033[0m\n", "JMPF", chunk.codes[i]);
                    break;
                case OP_DEF_VAR:
                {
                    i++; size_t ca = chunk.codes[i];
                    i++; size_t cb = chunk.codes[i];
                    i++; size_t cc = chunk.codes[i];
                    printf("%-12s\033[1m\033[32m%04zu,%04zu,%04zu\033[0m\n", "DEF VAR", ca, cb, cc);
                    break;
                }
                case OP_GET_VAR:
                {
                    i++; size_t ca = chunk.codes[i];
                    i++; size_t cb = chunk.codes[i];
                    i++; size_t cc = chunk.codes[i];
                    printf("%-12s\033[1m\033[32m%04zu,%04zu,%04zu\033[0m\n", "GET VAR", ca, cb, cc);
                    break;
                }
                case OP_SET_VAR:
                {
                    i++; size_t ca = chunk.codes[i];
                    i++; size_t cb = chunk.codes[i];
                    i++; size_t cc = chunk.codes[i];
                    printf("%-12s\033[1m\033[32m%04zu,%04zu,%04zu\033[0m\n", "SET VAR", ca, cb, cc);
                    break;
                }
                case OP_CALL:
                    i++;
                    printf("%-12s\033[1m\033[32m%04d\033[0m\n", "CALL", chunk.codes[i]);
                    break;
                case OP_RETURN:
                    printf("%-12s\n", "RETURN");
                    break;
                default:
                    break;
            }
        }
    }
}
