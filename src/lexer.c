#include "lexer.h"
#include "vm.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

static luna_TokenInfo lunaLexer_ScanNextCharToTokenInfo(luna_Lexer *lexer);
static bool lunaLexer_IsAtEnd(luna_Lexer *lexer);
static void lunaLexer_SkipWhitespace(luna_Lexer *lexer);
static void lunaLexer_SkipNote(luna_Lexer *lexer);
static luna_TokenInfo luna_MakeTokenInfo(luna_Lexer *lexer, luna_Token type, luna_Var value);
static void lunaLexer_ClearCurrentTokenText(luna_Lexer *lexer);
static void lunaLexer_AppendCharToCurrentTokenText(luna_Lexer *lexer, char c);
static luna_TokenInfo lunaLexer_ReportError(luna_Lexer *lexer, const char *format, ...);
static luna_TokenInfo lunaLexer_AnalyzeNumber(luna_Lexer *lexer);
static luna_TokenInfo lunaLexer_AnalyzeString(luna_Lexer *lexer, char quote);
static luna_TokenInfo lunaLexer_AnalyzeIdentifier(luna_Lexer *lexer);
static luna_TokenInfo lunaLexer_AnalyzeDef(luna_Lexer *lexer);
static luna_TokenInfo lunaLexer_AnalyzeOperator(luna_Lexer *lexer);
static void lunaTokenInfoList_Free(luna_TokenInfoList *list);
static void lunaTokenInfoList_Add(luna_TokenInfoList *list, luna_TokenInfo info);

#pragma region Public
void lunaLexer_Init(luna_Lexer *lexer, luna_VM *vm)
{
    lexer->vm = vm;
    lexer->source = NULL;
    lexer->current_char = NULL;
    lexer->line = 1;
    lexer->column = 1;
    lexer->current_token_text = LUNA_NULL_STRING;
    lexer->token_info_list.tokens = NULL;
    lexer->token_info_list.size = 0;
    lexer->token_info_list.capacity = 0;
    lexer->error = LUNA_NULL_ERROR;
    lexer->has_error = false;
}
void lunaLexer_Free(luna_Lexer *lexer)
{
    lexer->current_char = lexer->source = NULL;
    lexer->line = 1;
    lexer->column = 1;
    lunaString_Free(&(lexer->current_token_text));
    lunaTokenInfoList_Free(&(lexer->token_info_list));
    lunaError_Free(&lexer->error);
    lexer->has_error = false;
}
void lunaLexer_Load(luna_Lexer *lexer, const char *source)
{
    lexer->current_char = lexer->source = source;
    while (!lunaLexer_IsAtEnd(lexer))
    {
        luna_TokenInfo info = lunaLexer_ScanNextCharToTokenInfo(lexer);
        lunaTokenInfoList_Add(&(lexer->token_info_list), info);
        lunaString_Free(&(info.text));
        if (info.type == LUNA_TOKEN_EOF)
            return;
    }
}
bool lunaLexer_HasError(luna_Lexer *lexer)
{
    return lexer->has_error;
}
luna_Error lunaLexer_GetError(luna_Lexer *lexer)
{
    return lexer->error;
}
#pragma endregion
#pragma region Private
/// @brief 扫描
static luna_TokenInfo lunaLexer_ScanNextCharToTokenInfo(luna_Lexer *lexer)
{
    if (lexer->has_error)
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_EOF, LUNA_NIL_VAR);
    lunaLexer_SkipWhitespace(lexer);
    if (lunaLexer_IsAtEnd(lexer))
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_EOF, LUNA_NIL_VAR);
    char c = *lexer->current_char;
    // 注释
    if (c == '/' && *(lexer->current_char + 1) == '/')
    {
        lunaLexer_SkipNote(lexer);
        return lunaLexer_ScanNextCharToTokenInfo(lexer);
    }
    else if (isdigit(c) || (c == '.' && isdigit(*(lexer->current_char + 1))))// 数字
        return lunaLexer_AnalyzeNumber(lexer);
    else if (c == '"' || c == '\'')// 字符串
        return lunaLexer_AnalyzeString(lexer, c);
    else if (c == '#')// 定义
        return lunaLexer_AnalyzeDef(lexer);
    else if (isalpha(c) || c == '_')// 变量
        return lunaLexer_AnalyzeIdentifier(lexer);
    else// 操作符
        return lunaLexer_AnalyzeOperator(lexer);
}
static bool lunaLexer_IsAtEnd(luna_Lexer *lexer)
{
    return *lexer->current_char == '\0';
}
/// @brief 跳过空白字符
static void lunaLexer_SkipWhitespace(luna_Lexer *lexer)
{
    while (!lunaLexer_IsAtEnd(lexer))
    {
        char c = *lexer->current_char;
        switch (c)
        {
        case ' ':
        case '\t':
        case '\r':
            lexer->current_char++;
            lexer->column++;
            break;
        case '\n':
            lexer->current_char++;
            lexer->line++;
            lexer->column = 1;
            break;
        default:
            return;
        }
    }
}
/// @brief 跳过注释
static void lunaLexer_SkipNote(luna_Lexer *lexer)
{
    if (lexer->current_char[0] == '/' && lexer->current_char[1] == '/')
    {
        lexer->current_char += 2;
        lexer->column += 2;
        while (!lunaLexer_IsAtEnd(lexer) && *lexer->current_char != '\n')
        {
            lexer->current_char++;
            lexer->column++;
        }
    }
}
/// @brief 制作 Token 信息
static luna_TokenInfo luna_MakeTokenInfo(luna_Lexer *lexer, luna_Token type, luna_Var value)
{
    luna_TokenInfo info;
    info.type = type;
    info.text = LUNA_NULL_STRING;
    info.value = value;
    info.line = lexer->line;
    info.column = lexer->column;
    int token_len = lexer->current_token_text.len;
    if (token_len > 0)
    {
        lunaString_Malloc(&info.text, token_len + 1);
        lunaString_AppendObj(&info.text, &lexer->current_token_text);
    }
    return info;
}
/// @brief 清空当前 Token 文本
static void lunaLexer_ClearCurrentTokenText(luna_Lexer *lexer)
{
    lexer->current_token_text.len = 0;
    if (!lunaString_IsEmpty(&(lexer->current_token_text)))
        lexer->current_token_text.data[0] = '\0';
}
/// @brief 将 char 追加到当前 Token 文本
static void lunaLexer_AppendCharToCurrentTokenText(luna_Lexer *lexer, char c)
{
    if (!lexer->current_token_text.data)
        lunaString_Malloc(&lexer->current_token_text, 32);
    lunaString_AppendLen(&(lexer->current_token_text), (const char *)&c, 1);
}
/// @brief 报告错误
static luna_TokenInfo lunaLexer_ReportError(luna_Lexer *lexer, const char *format, ...)
{
    if (lexer->has_error)
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_EOF, LUNA_NIL_VAR);
    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    lunaError_Free(&lexer->error);
    lexer->error = luna_MakeError(lexer->line, lexer->column, "%s", buffer);
    lexer->has_error = true;
    return luna_MakeTokenInfo(lexer, LUNA_TOKEN_EOF, LUNA_NIL_VAR);
}
/// @brief 解析数字字面量
static luna_TokenInfo lunaLexer_AnalyzeNumber(luna_Lexer *lexer)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    bool is_float = false;
    if (*lexer->current_char == '0')
    {
        lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
        lexer->current_char++;
        lexer->column++;
        if (!lunaLexer_IsAtEnd(lexer))
        {
            char prefix = *lexer->current_char;
            // 十六进制
            if (prefix == 'x' || prefix == 'X')
            {
                lunaLexer_AppendCharToCurrentTokenText(lexer, prefix);
                lexer->current_char++;
                lexer->column++;
                int count = 0;
                while (!lunaLexer_IsAtEnd(lexer) && isxdigit(*lexer->current_char))
                {
                    lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
                    lexer->current_char++;
                    lexer->column++;
                    count++;
                }
                if (count == 0)
                    return lunaLexer_ReportError(lexer, "十六进制缺少有效数值");
                luna_Var v = {.vm = lexer->vm, .type = LUNA_TYPE_INT, .data.i = (luna_Int)strtoll(lexer->current_token_text.data, NULL, 16)};
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LIT_INT, v);
            }
            // 二进制
            else if (prefix == 'b' || prefix == 'B')
            {
                lunaLexer_AppendCharToCurrentTokenText(lexer, prefix);
                lexer->current_char++;
                lexer->column++;

                int count = 0;
                while (!lunaLexer_IsAtEnd(lexer) && (*lexer->current_char == '0' || *lexer->current_char == '1'))
                {
                    lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
                    lexer->current_char++;
                    lexer->column++;
                    count++;
                }
                if (count == 0)
                    return lunaLexer_ReportError(lexer, "二进制缺少有效数值");
                luna_Var v = {.vm = lexer->vm, .type = LUNA_TYPE_INT, .data.i = (luna_Int)strtoll(lexer->current_token_text.data + 2, NULL, 2)};
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LIT_INT, v);
            }
        }
    }
    // 整数
    while (!lunaLexer_IsAtEnd(lexer) && isdigit(*lexer->current_char))
    {
        lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
        lexer->current_char++;
        lexer->column++;
    }
    // 小数
    if (!lunaLexer_IsAtEnd(lexer) && *lexer->current_char == '.')
    {
        is_float = true;
        lunaLexer_AppendCharToCurrentTokenText(lexer, '.');
        lexer->current_char++;
        lexer->column++;
        while (!lunaLexer_IsAtEnd(lexer) && isdigit(*lexer->current_char))
        {
            lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
            lexer->current_char++;
            lexer->column++;
        }
        if (!lunaLexer_IsAtEnd(lexer) && *lexer->current_char == '.')
        {
            return lunaLexer_ReportError(lexer, "出现多余小数点");
        }
    }
    // 科学计数法
    if (!lunaLexer_IsAtEnd(lexer) && (*lexer->current_char == 'e' || *lexer->current_char == 'E'))
    {
        is_float = true;
        lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
        lexer->current_char++;
        lexer->column++;
        if (!lunaLexer_IsAtEnd(lexer) && (*lexer->current_char == '+' || *lexer->current_char == '-'))
        {
            lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
            lexer->current_char++;
            lexer->column++;
        }
        // 指数
        if (!lunaLexer_IsAtEnd(lexer) && isdigit(*lexer->current_char))
        {
            while (!lunaLexer_IsAtEnd(lexer) && isdigit(*lexer->current_char))
            {
                lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
                lexer->current_char++;
                lexer->column++;
            }
        }
        else
        {
            return lunaLexer_ReportError(lexer, "科学计数法缺少指数部分");
        }
    }
    luna_Var v = {.vm = lexer->vm};
    if (is_float)
    {
        v.type = LUNA_TYPE_FLOAT;
        v.data.f = (luna_Float)strtod(lexer->current_token_text.data, NULL);
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LIT_FLOAT, v);
    }
    else
    {
        v.type = LUNA_TYPE_INT;
        v.data.i = (luna_Int)strtoll(lexer->current_token_text.data, NULL, 10);
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LIT_INT, v);
    }
}
/// @brief 解析字符串字面量
static luna_TokenInfo lunaLexer_AnalyzeString(luna_Lexer *lexer, char quote)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    lexer->current_char++;
    lexer->column++;
    while (!lunaLexer_IsAtEnd(lexer) && *lexer->current_char != quote)
    {
        // 转义字符
        if (*lexer->current_char == '\\')
        {
            lexer->current_char++;
            lexer->column++;
            if (lunaLexer_IsAtEnd(lexer))
                return lunaLexer_ReportError(lexer, "字符串未闭合，缺少引号 %c", quote);
            switch (*lexer->current_char)
            {
            case 'n':
                lunaLexer_AppendCharToCurrentTokenText(lexer, '\n');
                break;
            case 't':
                lunaLexer_AppendCharToCurrentTokenText(lexer, '\t');
                break;
            case 'r':
                lunaLexer_AppendCharToCurrentTokenText(lexer, '\r');
                break;
            case '\\':
                lunaLexer_AppendCharToCurrentTokenText(lexer, '\\');
                break;
            case '"':
                lunaLexer_AppendCharToCurrentTokenText(lexer, '"');
                break;
            case '\'':
                lunaLexer_AppendCharToCurrentTokenText(lexer, '\'');
                break;
            default:
                return lunaLexer_ReportError(lexer, "未知的转义序列 '\\%c'", *lexer->current_char);
            }
        }
        else
        {
            lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
        }
        lexer->current_char++;
        lexer->column++;
    }
    if (lunaLexer_IsAtEnd(lexer) || *lexer->current_char != quote)
    {
        return lunaLexer_ReportError(lexer, "字符串未闭合，缺少引号 %c", quote);
    }
    lexer->current_char++;
    lexer->column++;
    luna_Var sv = {.vm = lexer->vm, .type = LUNA_TYPE_STRING};
    sv.data.r = LUNA_POOL_ALLOC(lexer->vm->strings);
    lunaString_AppendObj(LUNA_POOL_AT(lexer->vm->strings, sv.data.r), &(lexer->current_token_text));
    return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LIT_STRING, sv);
}
/// @brief 解析标识符
static luna_TokenInfo lunaLexer_AnalyzeIdentifier(luna_Lexer *lexer)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    while (!lunaLexer_IsAtEnd(lexer) && (isalnum(*lexer->current_char) || *lexer->current_char == '_'))
    {
        lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
        lexer->current_char++;
        lexer->column++;
    }
    if (!lunaString_IsEmpty(&(lexer->current_token_text)) && strcmp(lexer->current_token_text.data, "nil") == 0)
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LIT_NIL, LUNA_NIL_VAR);
    return luna_MakeTokenInfo(lexer, LUNA_TOKEN_VAR, LUNA_NIL_VAR);
}
/// @brief 解析定义符号
static luna_TokenInfo lunaLexer_AnalyzeDef(luna_Lexer *lexer)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    lexer->current_char++;
    lexer->column++;
    // 标识符
    if (lunaLexer_IsAtEnd(lexer) || (!isalpha(*lexer->current_char) && *lexer->current_char != '_'))
        return lunaLexer_ReportError(lexer, "定义名称必须是标识符");
    // 名称
    while (!lunaLexer_IsAtEnd(lexer) && (isalnum(*lexer->current_char) || *lexer->current_char == '_'))
    {
        lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current_char);
        lexer->current_char++;
        lexer->column++;
    }
    return luna_MakeTokenInfo(lexer, LUNA_TOKEN_DEF, LUNA_NIL_VAR);
}
/// @brief 解析符号
static luna_TokenInfo lunaLexer_AnalyzeOperator(luna_Lexer *lexer)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    char c = *lexer->current_char;
    char next = *(lexer->current_char + 1);
    switch (c)
    {
    case '+':
        if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_ADD, LUNA_NIL_VAR);
        }
        else if (next == '>')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_RETURN, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_ADD, LUNA_NIL_VAR);
    case '-':
        if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_SUB, LUNA_NIL_VAR);
        }
        else if (next == '>')
        {
            char next1 = *(lexer->current_char + 2);
            if (next1 == '>')
            {
                lexer->current_char += 3;
                lexer->column += 3;
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_RDARROW, LUNA_NIL_VAR);
            }
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_RARROW, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SUB, LUNA_NIL_VAR);
    case '*':
        if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_MUL, LUNA_NIL_VAR);
        }
        else if (next == '*')
        {
            char next1 = *(lexer->current_char + 2);
            if (next1 == '=')
            {
                lexer->current_char += 3;
                lexer->column += 3;
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_POW, LUNA_NIL_VAR);
            }
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_POW, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_MUL, LUNA_NIL_VAR);
    case '/':
        if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_DIV, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_DIV, LUNA_NIL_VAR);
    case '%':
        if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_MOD, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_MOD, LUNA_NIL_VAR);
    case '&':
        if (next == '&')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_AND, LUNA_NIL_VAR);
        }
        else if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_BIT_AND, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_AND, LUNA_NIL_VAR);
    case '|':
        if (next == '|')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_OR, LUNA_NIL_VAR);
        }
        else if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_BIT_OR, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_OR, LUNA_NIL_VAR);
    case '^':
        if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_BIT_XOR, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_XOR, LUNA_NIL_VAR);
    case '`':
        if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_BIT_XNOR, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_XNOR, LUNA_NIL_VAR);
    case '~':
        if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_BIT_NOT, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_NOT, LUNA_NIL_VAR);
    case '<':
        if (next == '-')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LARROW, LUNA_NIL_VAR);
        }
        else if (next == '<')
        {
            char next1 = *(lexer->current_char + 2);
            if (next1 == '-')
            {
                lexer->current_char += 3;
                lexer->column += 3;
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LDARROW, LUNA_NIL_VAR);
            }
            else if (next1 == '=')
            {
                lexer->current_char += 3;
                lexer->column += 3;
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_BIT_SHL, LUNA_NIL_VAR);
            }
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_SHL, LUNA_NIL_VAR);
        }
        else if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LE, LUNA_NIL_VAR);
        }
        else if (next == '>')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LOOP, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LT, LUNA_NIL_VAR);
    case '>':
        if (next == '_')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_TERMINAL, LUNA_NIL_VAR);
        }
        else if (next == '>')
        {
            char next1 = *(lexer->current_char + 2);
            if (next1 == '=')
            {
                lexer->current_char += 3;
                lexer->column += 3;
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SELF_BIT_SHR, LUNA_NIL_VAR);
            }
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_SHR, LUNA_NIL_VAR);
        }
        else if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_GE, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_GT, LUNA_NIL_VAR);
    case '=':
        if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_EQ, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_ASSIGN, LUNA_NIL_VAR);
    case '!':
        if (next == '=')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_NE, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_NOT, LUNA_NIL_VAR);
    case '?':
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_IF, LUNA_NIL_VAR);
    case ':':
        if (next == '?')
        {
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_ELIF, LUNA_NIL_VAR);
        }
        else if (next == '>')
        {
            char next1 = *(lexer->current_char + 2);
            if (next1 == '>')
            {
                lexer->current_char += 3;
                lexer->column += 3;
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BREAK, LUNA_NIL_VAR);
            }
            lexer->current_char += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_CONTINUE, LUNA_NIL_VAR);
        }
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_ELSE, LUNA_NIL_VAR);
    case '{':
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LBRACE, LUNA_NIL_VAR);
    case '}':
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_RBRACE, LUNA_NIL_VAR);
    case '[':
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LBRACKET, LUNA_NIL_VAR);
    case ']':
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_RBRACKET, LUNA_NIL_VAR);
    case '.':
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_DOT, LUNA_NIL_VAR);
    case ',':
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_COMMA, LUNA_NIL_VAR);
    case '(':
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LPAREN, LUNA_NIL_VAR);
    case ')':
        lexer->current_char++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_RPAREN, LUNA_NIL_VAR);
    default:
        return lunaLexer_ReportError(lexer, "无法识别的字符 '%c' (ASCII: %d)", c, c);
    }
}
#pragma endregion
#pragma region Token Info List
static void lunaTokenInfoList_Free(luna_TokenInfoList *list)
{
    for (int i = 0; i < list->size; i++)
    {
        if (!lunaString_IsEmpty(&(list->tokens[i].text)))
            lunaString_Free(&(list->tokens[i].text));
    }
    free(list->tokens);
    list->tokens = NULL;
    list->size = 0;
    list->capacity = 0;
}
static void lunaTokenInfoList_Add(luna_TokenInfoList *list, luna_TokenInfo info)
{
    if (list->size >= list->capacity)
    {
        int new_capacity = list->capacity == 0 ? 64 : list->capacity * 2;
        luna_TokenInfo *new_tokens = realloc(list->tokens, new_capacity * sizeof(luna_TokenInfo));
        if (!new_tokens)
        {
            // TODO: 内存分配失败
            return;
        }
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }
    list->tokens[list->size] = info;
    if (!lunaString_IsEmpty(&(info.text)))
    {
        luna_String new_str = LUNA_NULL_STRING;
        lunaString_Malloc(&new_str, info.text.len + 1);
        lunaString_AppendObj(&new_str, &(info.text));
        list->tokens[list->size].text = new_str;
    }
    list->size++;
}
#pragma endregion
