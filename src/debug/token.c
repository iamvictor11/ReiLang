#include "../lexer.h"
#include <stdlib.h>
#include <string.h>

const char *lunaTokenInfo_ToString(luna_TokenInfo info)
{
    static char buf[256];
    const char *type_name = "UNKNOWN";
    switch (info.type)
    {
    /*注释*/
    case LUNA_TOKEN_NOTE:
        type_name = "NOTE";
        break;
    /*定义/赋值*/
    case LUNA_TOKEN_DEF:
        type_name = "DEF";
        break;
    case LUNA_TOKEN_ASSIGN:
        type_name = "ASSIGN";
        break;
    /*变量名/字面量*/
    case LUNA_TOKEN_VAR:
        type_name = "VAR";
        break;
    case LUNA_TOKEN_LIT_NIL:
        type_name = "LIT_NIL";
        break;
    case LUNA_TOKEN_LIT_INT:
        type_name = "LIT_INT";
        break;
    case LUNA_TOKEN_LIT_FLOAT:
        type_name = "LIT_FLOAT";
        break;
    case LUNA_TOKEN_LIT_STRING:
        type_name = "LIT_STRING";
        break;
    /*数学运算*/
    case LUNA_TOKEN_ADD:
        type_name = "ADD";
        break;
    case LUNA_TOKEN_SUB:
        type_name = "SUB";
        break;
    case LUNA_TOKEN_MUL:
        type_name = "MUL";
        break;
    case LUNA_TOKEN_DIV:
        type_name = "DIV";
        break;
    case LUNA_TOKEN_MOD:
        type_name = "MOD";
        break;
    case LUNA_TOKEN_POW:
        type_name = "POW";
        break;
    case LUNA_TOKEN_SELF_ADD:
        type_name = "SELF_ADD";
        break;
    case LUNA_TOKEN_SELF_SUB:
        type_name = "SELF_SUB";
        break;
    case LUNA_TOKEN_SELF_MUL:
        type_name = "SELF_MUL";
        break;
    case LUNA_TOKEN_SELF_DIV:
        type_name = "SELF_DIV";
        break;
    case LUNA_TOKEN_SELF_MOD:
        type_name = "SELF_MOD";
        break;
    case LUNA_TOKEN_SELF_POW:
        type_name = "SELF_POW";
        break;
    /*位运算*/
    case LUNA_TOKEN_BIT_AND:
        type_name = "BIT_AND";
        break;
    case LUNA_TOKEN_BIT_OR:
        type_name = "BIT_OR";
        break;
    case LUNA_TOKEN_BIT_XOR:
        type_name = "BIT_XOR";
        break;
    case LUNA_TOKEN_BIT_XNOR:
        type_name = "BIT_XNOR";
        break;
    case LUNA_TOKEN_BIT_NOT:
        type_name = "BIT_NOT";
        break;
    case LUNA_TOKEN_BIT_SHL:
        type_name = "BIT_SHL";
        break;
    case LUNA_TOKEN_BIT_SHR:
        type_name = "BIT_SHR";
        break;
    case LUNA_TOKEN_SELF_BIT_AND:
        type_name = "SELF_BIT_AND";
        break;
    case LUNA_TOKEN_SELF_BIT_OR:
        type_name = "SELF_BIT_OR";
        break;
    case LUNA_TOKEN_SELF_BIT_XOR:
        type_name = "SELF_BIT_XOR";
        break;
    case LUNA_TOKEN_SELF_BIT_XNOR:
        type_name = "SELF_BIT_XNOR";
        break;
    case LUNA_TOKEN_SELF_BIT_NOT:
        type_name = "SELF_BIT_NOT";
        break;
    case LUNA_TOKEN_SELF_BIT_SHL:
        type_name = "SELF_BIT_SHL";
        break;
    case LUNA_TOKEN_SELF_BIT_SHR:
        type_name = "SELF_BIT_SHR";
        break;
    /*比较运算*/
    case LUNA_TOKEN_EQ:
        type_name = "EQ";
        break;
    case LUNA_TOKEN_NE:
        type_name = "NE";
        break;
    case LUNA_TOKEN_LT:
        type_name = "LT";
        break;
    case LUNA_TOKEN_LE:
        type_name = "LE";
        break;
    case LUNA_TOKEN_GT:
        type_name = "GT";
        break;
    case LUNA_TOKEN_GE:
        type_name = "GE";
        break;
    /*逻辑*/
    case LUNA_TOKEN_NOT:
        type_name = "NOT";
        break;
    case LUNA_TOKEN_AND:
        type_name = "AND";
        break;
    case LUNA_TOKEN_OR:
        type_name = "OR";
        break;
    /*关键字*/
    case LUNA_TOKEN_IF:
        type_name = "IF";
        break;
    case LUNA_TOKEN_ELIF:
        type_name = "ELIF";
        break;
    case LUNA_TOKEN_ELSE:
        type_name = "ELSE";
        break;
    case LUNA_TOKEN_LOOP:
        type_name = "LOOP";
        break;
    case LUNA_TOKEN_RETURN:
        type_name = "RETURN";
        break;
    case LUNA_TOKEN_CONTINUE:
        type_name = "CONTINUE";
        break;
    case LUNA_TOKEN_BREAK:
        type_name = "BREAK";
        break;
    /*区域*/
    case LUNA_TOKEN_LPAREN:
        type_name = "LPAREN";
        break;
    case LUNA_TOKEN_RPAREN:
        type_name = "RPAREN";
        break;
    case LUNA_TOKEN_LBRACKET:
        type_name = "LBRACKET";
        break;
    case LUNA_TOKEN_RBRACKET:
        type_name = "RBRACKET";
        break;
    case LUNA_TOKEN_LBRACE:
        type_name = "LBRACE";
        break;
    case LUNA_TOKEN_RBRACE:
        type_name = "RBRACE";
        break;
    /*功能*/
    case LUNA_TOKEN_RARROW:
        type_name = "RARROW";
        break;
    case LUNA_TOKEN_LARROW:
        type_name = "LARROW";
        break;
    case LUNA_TOKEN_RDARROW:
        type_name = "RDARROW";
        break;
    case LUNA_TOKEN_LDARROW:
        type_name = "LDARROW";
        break;
    case LUNA_TOKEN_DOT:
        type_name = "DOT";
        break;
    case LUNA_TOKEN_COMMA:
        type_name = "COMMA";
        break;
    /*终端*/
    case LUNA_TOKEN_TERMINAL:
        type_name = "TERMINAL";
        break;
    /*其他*/
    case LUNA_TOKEN_EOF:
        type_name = "EOF";
        break;
    case LUNA_TOKEN_ERROR:
        type_name = "ERROR";
        break;
    default:
        type_name = "UNKNOWN";
        break;
    }
    if (info.type == LUNA_TOKEN_LIT_INT)
        sprintf(buf, "[%d:%d] %s: %lld", info.line, info.column, type_name, (long long)info.value.data.i);
    else if (info.type == LUNA_TOKEN_LIT_FLOAT)
        sprintf(buf, "[%d:%d] %s: %g", info.line, info.column, type_name, info.value.data.f);
    else if (info.type == LUNA_TOKEN_LIT_STRING || info.type == LUNA_TOKEN_DEF || info.type == LUNA_TOKEN_VAR)
        sprintf(buf, "[%d:%d] %s: \"%s\"", info.line, info.column, type_name, info.text.data);
    else
        sprintf(buf, "[%d:%d] %s", info.line, info.column, type_name);
    return buf;
}
const char *lunaTokenInfoList_ToString(luna_TokenInfoList *list)
{
    if (list == NULL || list->size == 0)
        return "Empty Token List";
    size_t capacity = list->size * 64;
    char *result = (char *)malloc(capacity);
    if (!result)
        return "Memory Allocation Failed";
    result[0] = '\0';
    size_t current_len = 0;
    for (int i = 0; i < list->size; i++)
    {
        const char *token_str = lunaTokenInfo_ToString(list->tokens[i]);
        size_t token_len = strlen(token_str);
        if (current_len + token_len + 2 >= capacity)
        {
            capacity *= 2;
            char *new_ptr = (char *)realloc(result, capacity);
            if (!new_ptr)
                return result;
            result = new_ptr;
        }
        strcat(result, token_str);
        strcat(result, "\n");
        current_len += (token_len + 1);
    }
    return result;
}
