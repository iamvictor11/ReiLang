#include "core/base/common.hpp"
#include <cmath>
#include <limits>

namespace rei
{
    static void _Chunk_debugPrint(const Chunk& chunk, size_t level = 0)
    {
        for (size_t i = 0;i < chunk.codes.size();i++)
        {
            auto instruction = static_cast<Opcode>(chunk.codes[i]);
            for (size_t i = 0; i < level; i++)
                printf("\t");
            switch (instruction)
            {
                case OP_CONSTANT:
                {
                    i++;
                    Bytecode ci = chunk.codes[i];
                    printf("%-16s\033[1m\033[32m%04zu \033[34m%s\033[0m\n", "CONSTANT", ci, Value::getDebugString(chunk.constants[ci]).c_str());
                    if (std::holds_alternative<Ref<Function>>(chunk.constants[ci]))
                    {
                        auto func_ref = std::get<Ref<Function>>(chunk.constants[ci]);
                        _Chunk_debugPrint(func_ref->chunk, level + 1);
                    }
                    break;
                }
                case OP_POP:
                    printf("%-16s\n", "POP");
                    break;
                case OP_NIL:
                    printf("%-16s\n", "NIL");
                    break;
                case OP_TRUE:
                    printf("%-16s\n", "TRUE");
                    break;
                case OP_FALSE:
                    printf("%-16s\n", "FALSE");
                    break;
                case OP_NEG:
                    printf("%-16s\n", "NEG");
                    break;
                case OP_ADD:
                    printf("%-16s\n", "ADD");
                    break;
                case OP_SUB:
                    printf("%-16s\n", "SUB");
                    break;
                case OP_MUL:
                    printf("%-16s\n", "MUL");
                    break;
                case OP_DIV:
                    printf("%-16s\n", "DIV");
                    break;
                case OP_MOD:
                    printf("%-16s\n", "MOD");
                    break;
                case OP_POW:
                    printf("%-16s\n", "POW");
                    break;
                case OP_BIT_AND:
                    printf("%-16s\n", "BIT AND");
                    break;
                case OP_BIT_OR:
                    printf("%-16s\n", "BIT OR");
                    break;
                case OP_BIT_XOR:
                    printf("%-16s\n", "BIT XOR");
                    break;
                case OP_BIT_XNOR:
                    printf("%-16s\n", "BIT XNOR");
                    break;
                case OP_BIT_SHL:
                    printf("%-16s\n", "BIT SHL");
                    break;
                case OP_BIT_SHR:
                    printf("%-16s\n", "BIT SHR");
                    break;
                case OP_EQ:
                    printf("%-16s\n", "EQ");
                    break;
                case OP_NE:
                    printf("%-16s\n", "NE");
                    break;
                case OP_LT:
                    printf("%-16s\n", "LT");
                    break;
                case OP_LE:
                    printf("%-16s\n", "LE");
                    break;
                case OP_GT:
                    printf("%-16s\n", "GT");
                    break;
                case OP_GE:
                    printf("%-16s\n", "GE");
                    break;
                case OP_AND:
                    i++;
                    printf("%-16s\033[1m\033[32m%04zu\033[0m\n", "AND", chunk.codes[i]);
                    break;
                case OP_OR:
                    i++;
                    printf("%-16s\033[1m\033[32m%04zu\033[0m\n", "OR", chunk.codes[i]);
                    break;
                case OP_BEG:
                    printf("%-16s\n", "BEG");
                    break;
                case OP_END:
                    printf("%-16s\n", "END");
                    break;
                case OP_PRINT:
                    printf("%-16s\n", "PRINT");
                    break;
                case OP_PRINTLN:
                    printf("%-16s\n", "PRINTLN");
                    break;
                case OP_JUMP:
                    i++;
                    printf("%-16s\033[1m\033[32m%04d\033[0m\n", "JUMP", chunk.codes[i]);
                    break;
                case OP_JMPT:
                    i++;
                    printf("%-16s\033[1m\033[32m%04d\033[0m\n", "JMPT", chunk.codes[i]);
                    break;
                case OP_JMPF:
                    i++;
                    printf("%-16s\033[1m\033[32m%04d\033[0m\n", "JMPF", chunk.codes[i]);
                    break;
                case OP_LOOP:
                    printf("%-16s\n", "LOOP");
                    break;
                case OP_DEF_VAR:
                    i++;
                    printf("%-16s\033[1m\033[32m%04zu \033[34m%s\033[0m\n", "DEF VAR", chunk.codes[i], Value::getDebugString(chunk.constants[chunk.codes[i]]).c_str());
                    break;
                case OP_GET_VAR:
                {
                    i++; size_t cd = chunk.codes[i];
                    i++; size_t ci = chunk.codes[i];
                    i++; size_t cl = chunk.codes[i];
                    printf("%-16s\033[1m\033[32m%04zu,%04zu,%04zu\033[0m\n", "GET VAR", cd, ci, cl);
                    break;
                }
                case OP_SET_VAR:
                {
                    i++; size_t cd = chunk.codes[i];
                    i++; size_t ci = chunk.codes[i];
                    i++; size_t cl = chunk.codes[i];
                    printf("%-16s\033[1m\033[32m%04zu,%04zu,%04zu\033[0m\n", "SET VAR", cd, ci, cl);
                    break;
                }
                case OP_CALL:
                    printf("%-16s\n", "CALL");
                    break;
                case OP_RETURN:
                    printf("%-16s\n", "RETURN");
                    break;
                default:
                    break;
            }
        }
    }
}
