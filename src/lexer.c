#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

/// @brief 跳过空白字符
static void lunaLexer_SkipWhitespace(luna_Lexer *lexer)
{
    while (!lunaLexer_IsAtEnd(lexer))
    {
        char c = *lexer->current;
        switch (c)
        {
        case ' ':
        case '\t':
        case '\r':
            lexer->current++;
            lexer->column++;
            break;
        case '\n':
            lexer->current++;
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
    if (lexer->current[0] == '/' && lexer->current[1] == '/')
    {
        lexer->current += 2;
        lexer->column += 2;
        while (!lunaLexer_IsAtEnd(lexer) && *lexer->current != '\n')
        {
            lexer->current++;
            lexer->column++;
        }
    }
}
/// @brief 制作 Token 信息
static luna_TokenInfo luna_MakeTokenInfo(luna_Lexer *lexer, luna_Token type)
{
    luna_TokenInfo info;
    info.type = type;
    info.text = lexer->current_token_text;
    info.line = lexer->line;
    info.column = lexer->column - (lexer->current_token_text ? (int)lexer->current_token_text->len : 0);
    return info;
}
/// @brief 清空当前 Token 文本
static void lunaLexer_ClearCurrentTokenText(luna_Lexer *lexer)
{
    if (lexer->current_token_text)
    {
        lexer->current_token_text->len = 0;
        if (lexer->current_token_text->data)
            lexer->current_token_text->data[0] = '\0';
    }
}
/// @brief 将 char 追加到当前 Token 文本
static void lunaLexer_AppendCharToCurrentTokenText(luna_Lexer *lexer, char c)
{
    if (!lexer->current_token_text)
    {
        lexer->current_token_text = malloc(sizeof(luna_String));
        *lexer->current_token_text = LUNA_NULL_STRING;
        lunaString_Malloc(lexer->current_token_text, 32);
    }
    char str[2] = {c, '\0'};
    lunaString_Append(lexer->current_token_text, str);
}
/// @brief 报告错误
static luna_TokenInfo lunaLexer_ReportError(luna_Lexer *lexer, const char *format, ...)
{
    if (lexer->has_error)
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_EOF);
    va_list args;
    va_start(args, format);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    int col = lexer->column - (lexer->current_token_text ? (int)lexer->current_token_text->len : 0);
    lunaError_Free(&lexer->error);
    lexer->error = luna_MakeError(lexer->line, col, "%s", buffer);
    lexer->has_error = true;
    return luna_MakeTokenInfo(lexer, LUNA_TOKEN_EOF);
}
/// @brief 解析数字字面量
static luna_TokenInfo lunaLexer_AnalyzeNumber(luna_Lexer *lexer)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    bool has_int = false;
    bool is_float = false;
    // 整数
    while (!lunaLexer_IsAtEnd(lexer) && isdigit(*lexer->current))
    {
        lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current);
        lexer->current++;
        lexer->column++;
    }
    // 小数
    if (!lunaLexer_IsAtEnd(lexer) && *lexer->current == '.')
    {
        char next = *(lexer->current + 1);
        if (isdigit(next))
        {
            is_float = true;
            lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current);
            lexer->current++;
            lexer->column++;
            while (!lunaLexer_IsAtEnd(lexer) && isdigit(*lexer->current))
            {
                lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current);
                lexer->current++;
                lexer->column++;
            }
        }
        else if (next == '.')
        {
            return lunaLexer_ReportError(lexer, "无效的数字格式：多个小数点");
        }
    }
    // 科学计数法
    if (!lunaLexer_IsAtEnd(lexer) && (*lexer->current == 'e' || *lexer->current == 'E'))
    {
        is_float = true;
        lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current);
        lexer->current++;
        lexer->column++;
        if (!lunaLexer_IsAtEnd(lexer) && (*lexer->current == '+' || *lexer->current == '-'))
        {
            lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current);
            lexer->current++;
            lexer->column++;
        }
        // 指数
        if (!lunaLexer_IsAtEnd(lexer) && isdigit(*lexer->current))
        {
            while (!lunaLexer_IsAtEnd(lexer) && isdigit(*lexer->current))
            {
                lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current);
                lexer->current++;
                lexer->column++;
            }
        }
        else
        {
            return lunaLexer_ReportError(lexer, "科学计数法缺少指数部分");
        }
    }
    return luna_MakeTokenInfo(lexer, is_float ? LUNA_TOKEN_TYPE_FLOAT : LUNA_TOKEN_TYPE_INT);
}

/// @brief 解析字符串字面量
static luna_TokenInfo lunaLexer_AnalyzeString(luna_Lexer *lexer, char quote)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    lexer->current++;
    lexer->column++;
    while (!lunaLexer_IsAtEnd(lexer) && *lexer->current != quote)
    {
        // 转义字符
        if (*lexer->current == '\\')
        {
            lexer->current++;
            lexer->column++;
            if (lunaLexer_IsAtEnd(lexer))
                return lunaLexer_ReportError(lexer, "字符串未闭合，缺少引号 %c", quote);
            switch (*lexer->current)
            {
            case 'n': lunaLexer_AppendCharToCurrentTokenText(lexer, '\n'); break;
            case 't': lunaLexer_AppendCharToCurrentTokenText(lexer, '\t'); break;
            case 'r': lunaLexer_AppendCharToCurrentTokenText(lexer, '\r'); break;
            case '\\': lunaLexer_AppendCharToCurrentTokenText(lexer, '\\'); break;
            case '"': lunaLexer_AppendCharToCurrentTokenText(lexer, '"'); break;
            case '\'': lunaLexer_AppendCharToCurrentTokenText(lexer, '\''); break;
            default:
                return lunaLexer_ReportError(lexer, "未知的转义序列 '\\%c'", *lexer->current);
            }
        }
        else
        {
            lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current);
        }
        lexer->current++;
        lexer->column++;
    }
    if (lunaLexer_IsAtEnd(lexer) || *lexer->current != quote)
    {
        return lunaLexer_ReportError(lexer, "字符串未闭合，缺少引号 %c", quote);
    }
    lexer->current++;
    lexer->column++;
    return luna_MakeTokenInfo(lexer, LUNA_TOKEN_TYPE_STRING);
}
/// @brief 解析标识符
static luna_TokenInfo lunaLexer_AnalyzeIdentifier(luna_Lexer *lexer)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    while (!lunaLexer_IsAtEnd(lexer) && (isalnum(*lexer->current) || *lexer->current == '_'))
    {
        lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current);
        lexer->current++;
        lexer->column++;
    }
    if (lexer->current_token_text && strcmp(lexer->current_token_text->data, "nil") == 0)
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_TYPE_NIL);
    return luna_MakeTokenInfo(lexer, LUNA_TOKEN_VAR);
}
/// @brief 解析定义语句
static luna_TokenInfo lunaLexer_AnalyzeDef(luna_Lexer *lexer)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    lexer->current++;
    lexer->column++;
    // 标识符
    if (lunaLexer_IsAtEnd(lexer) || (!isalpha(*lexer->current) && *lexer->current != '_'))
        return lunaLexer_ReportError(lexer, "定义名称必须是标识符");
    // 名称
    while (!lunaLexer_IsAtEnd(lexer) && (isalnum(*lexer->current) || *lexer->current == '_'))
    {
        lunaLexer_AppendCharToCurrentTokenText(lexer, *lexer->current);
        lexer->current++;
        lexer->column++;
    }
    return luna_MakeTokenInfo(lexer, LUNA_TOKEN_DEF);
}
/// @brief 解析终端操作
static luna_TokenInfo lunaLexer_AnalyzeTerminal(luna_Lexer *lexer)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    if (!lunaLexer_IsAtEnd(lexer))
    {
        const char *saved = lexer->current;
        int saved_line = lexer->line;
        int saved_col = lexer->column;
        lunaLexer_SkipWhitespace(lexer);
        // 操作
        if (!lunaLexer_IsAtEnd(lexer))
        {
            if (*lexer->current == '<' && *(lexer->current + 1) == '-')
            {
                // 打印
                lexer->current += 2;
                lexer->column += 2;
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_PRINT);
            }
            else if (*lexer->current == '-' && *(lexer->current + 1) == '>')
            {
                // 扫描
                lexer->current += 2;
                lexer->column += 2;
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SCAN);
            }
        }
        lexer->current = saved;
        lexer->line = saved_line;
        lexer->column = saved_col;
    }
    return luna_MakeTokenInfo(lexer, LUNA_TOKEN_TERMINAL);
}
/// @brief 解析操作符
static luna_TokenInfo lunaLexer_AnalyzeOperator(luna_Lexer *lexer)
{
    lunaLexer_ClearCurrentTokenText(lexer);
    char c = *lexer->current;
    char next = *(lexer->current + 1);
    char next1 = *(lexer->current + 2);
    // 终端
    if (c == '>' && next == '_')
    {
        lexer->current += 2;
        lexer->column += 2;
        return lunaLexer_AnalyzeTerminal(lexer);
    }
    // 常规
    switch (c)
    {
    case '+':
        if (next == '>')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_RETURN);
        }
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_ADD);
    case '-':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_SUB);
    case '*':
        if (next == '*')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_POW);
        }
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_MUL);
    case '/':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_DIV);
    case '%':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_MOD);
    case '&':
        if (next == '&')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_AND);
        }
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_AND);
    case '|':
        if (next == '|')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_OR);
        }
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_OR);
    case '^':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_XOR);
    case '`':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_XNOR);
    case '~':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_NOT);
    case '<':
        if (next == '<')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_SHL);
        }
        else if (next == '=')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LE);
        }
        else if (next == '>')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LOOP);
        }
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LT);
    case '>':
        if (next == '>')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BIT_SHR);
        }
        else if (next == '=')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_GE);
        }
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_GT);
        
    case '=':
        if (next == '=')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_EQ);
        }
        else if (next == '>')
        {
            if (next1 == '>')
            {
                lexer->current += 3;
                lexer->column += 3;
                return luna_MakeTokenInfo(lexer, LUNA_TOKEN_BREAK);
            }
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_CONTINUE);
        }
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_ASSIGN);
    case '!':
        if (next == '=')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_NE);
        }
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_NOT);
    case '?':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_IF);
    case ':':
        if (next == '?')
        {
            lexer->current += 2;
            lexer->column += 2;
            return luna_MakeTokenInfo(lexer, LUNA_TOKEN_ELIF);
        }
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_ELSE);
    case '{':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LBRACE);
    case '}':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_RBRACE);
    case '[':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LBRACKET);
    case ']':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_RBRACKET);
    case '.':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_DOT);
    case ',':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_COMMA);
    case '(':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_LPAREN);
    case ')':
        lexer->current++;
        lexer->column++;
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_RPAREN);
    default:
        return lunaLexer_ReportError(lexer, "无法识别的字符 '%c' (ASCII: %d)", c, c);
    }
}


void lunaLexer_Init(luna_Lexer *lexer, luna_VM *vm, const char *source)
{
    lexer->vm = vm;
    lexer->source = source;
    lexer->current = source;
    lexer->line = 1;
    lexer->column = 1;
    lexer->current_token_text = NULL;
    lexer->has_error = false;
    lexer->error.message = NULL;
}
void lunaLexer_Free(luna_Lexer *lexer)
{
    if (lexer->current_token_text)
    {
        lunaString_Free(lexer->current_token_text);
        free(lexer->current_token_text);
        lexer->current_token_text = NULL;
    }
    lunaError_Free(&lexer->error);
}
bool lunaLexer_IsAtEnd(luna_Lexer *lexer)
{
    return *lexer->current == '\0';
}
bool lunaLexer_HasError(luna_Lexer *lexer)
{
    return lexer->has_error;
}
luna_Error lunaLexer_GetError(luna_Lexer *lexer)
{
    return lexer->error;
}
void lunaLexer_ClearError(luna_Lexer *lexer)
{
    lunaError_Free(&lexer->error);
    lexer->has_error = false;
}
luna_TokenInfo lunaLexer_NextToken(luna_Lexer *lexer)
{
    if (lexer->has_error)
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_EOF);
    lunaLexer_SkipWhitespace(lexer);
    if (lunaLexer_IsAtEnd(lexer))
        return luna_MakeTokenInfo(lexer, LUNA_TOKEN_EOF);
    char c = *lexer->current;
    // 注释
    if (c == '/' && *(lexer->current + 1) == '/')
    {
        lunaLexer_SkipNote(lexer);
        return lunaLexer_NextToken(lexer);
    }
    // 数字
    if (isdigit(c) || (c == '.' && isdigit(*(lexer->current + 1))))
        return lunaLexer_AnalyzeNumber(lexer);
    // 字符串
    if (c == '"' || c == '\'')
        return lunaLexer_AnalyzeString(lexer, c);
    // 定义
    if (c == '#')
        return lunaLexer_AnalyzeDef(lexer);
    // 变量
    if (isalpha(c) || c == '_')
        return lunaLexer_AnalyzeIdentifier(lexer);
    // 操作符
    return lunaLexer_AnalyzeOperator(lexer);
}