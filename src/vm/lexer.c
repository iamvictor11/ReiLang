// lexer.c 补充部分
#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Aid
static bool _isBdigit(char c);
static bool _isOdigit(char c);
// Scan
static void _scan(luna_Lexer *self);
static void _lunaLexer_moveCursor(luna_Lexer *self);
static void _lunaLexer_moveCursorln(luna_Lexer *self);
static void _lunaLexer_smartMoveCursor(luna_Lexer *self, char c);
// Kan/Move
static bool _lunaLexer_isAtEnd(luna_Lexer *self);
static bool _lunaLexer_isAtEndOffset(luna_Lexer *self, size_t offset);
static char _lunaLexer_advance(luna_Lexer *self);
static void _lunaLexer_pass(luna_Lexer *self);
static void _lunaLexer_passStep(luna_Lexer *self, size_t step);
static bool _lunaLexer_matchChar(luna_Lexer *self, char c);
static bool _lunaLexer_matchString(luna_Lexer *self, const char *s, size_t omit);
static char _lunaLexer_peek(luna_Lexer *self);
static char _lunaLexer_peekOffset(luna_Lexer *self, size_t offset);
// Skip
static void _lunaLexer_skipNote(luna_Lexer *self);
static void _lunaLexer_skipWhite(luna_Lexer *self);
// Lex
static void _lunaLexer_lexOther(luna_Lexer *self, char c);
static void _lunaLexer_lexNumber(luna_Lexer *self);
static void _lunaLexer_lexString(luna_Lexer *self, char beg);
static void _lunaLexer_lexRawString(luna_Lexer *self, char beg);
static void _lunaLexer_lexIdentifier(luna_Lexer *self);
// Xie
static void _lunaLexer_addToken(luna_Lexer *self, luna_TokenType type);
static void _lunaLexer_addTokenLiteral(luna_Lexer *self, luna_TokenType type, luna_Value literal);
static void _lunaLexer_addTokenFull(luna_Lexer *self, luna_TokenType type, const char *lexeme, size_t lexeme_len, luna_Value literal);

void lunaLexer_start(luna_Lexer *self, const char *source)
{
    self->source = source;
    self->source_len = strlen(source);
    self->cursor.start = source;
    self->cursor.curr = source;
    self->cursor.pos = (luna_Position){1, 1};
    
    lunaTokenArray_init(&self->tokens);
    
    while (!_lunaLexer_isAtEnd(self))
        _scan(self);
    
    luna_Token eof_token;
    eof_token.type = LUNA_TK_EOF;
    eof_token.lexeme.start = "";
    eof_token.lexeme.len = 0;
    eof_token.literal = (luna_Value){0};
    eof_token.pos = self->cursor.pos;
    lunaTokenArray_write(&self->tokens, eof_token);
}
#pragma region Aid
static bool _isBdigit(char c)
{
    return c == '0' || c == '1';
}

static bool _isOdigit(char c)
{
    return c >= '0' && c <= '7';
}
#pragma endregion
#pragma region Scan
static void _scan(luna_Lexer *self)
{
    _lunaLexer_skipWhite(self);
    self->cursor.start = self->cursor.curr;
    char c = _lunaLexer_advance(self);
    switch (c)
    {
    case '\0': break;
    case ',': _lunaLexer_addToken(self, LUNA_TK_COMMA); break;
    case '.': _lunaLexer_addToken(self, LUNA_TK_DOT); break;
    case ':': 
        if (_lunaLexer_matchChar(self, ':')) _lunaLexer_addToken(self, LUNA_TK_DCOLON);
        else if (_lunaLexer_matchChar(self, '=')) _lunaLexer_addToken(self, LUNA_TK_WALRUS);
        else _lunaLexer_addToken(self, LUNA_TK_COLON);
        break;
    case ';': _lunaLexer_addToken(self, LUNA_TK_SEMICOLON); break;
    case '(': _lunaLexer_addToken(self, LUNA_TK_LPAREN); break;
    case ')': _lunaLexer_addToken(self, LUNA_TK_RPAREN); break;
    case '[': _lunaLexer_addToken(self, LUNA_TK_LBRACKET); break;
    case ']': _lunaLexer_addToken(self, LUNA_TK_RBRACKET); break;
    case '{': _lunaLexer_addToken(self, LUNA_TK_LBRACE); break;
    case '}': _lunaLexer_addToken(self, LUNA_TK_RBRACE); break;
    case '+': _lunaLexer_addToken(self, _lunaLexer_matchChar(self, '=') ? LUNA_TK_SELF_ADD : LUNA_TK_ADD); break;
    case '-': 
        if (_lunaLexer_matchChar(self, '=')) _lunaLexer_addToken(self, LUNA_TK_SELF_SUB);
        else if (_lunaLexer_matchChar(self, '>')) _lunaLexer_addToken(self, LUNA_TK_RARROW);
        else _lunaLexer_addToken(self, LUNA_TK_SUB);
        break;
    case '*': 
        if (_lunaLexer_matchChar(self, '=')) _lunaLexer_addToken(self, LUNA_TK_SELF_MUL);
        else if (_lunaLexer_matchChar(self, '*')) 
        {
            if (_lunaLexer_matchChar(self, '=')) _lunaLexer_addToken(self, LUNA_TK_SELF_POW);
            else _lunaLexer_addToken(self, LUNA_TK_POW);
        }
        else _lunaLexer_addToken(self, LUNA_TK_MUL);
        break;
    case '/':
        if (_lunaLexer_matchChar(self, '/')) _lunaLexer_skipNote(self);
        else _lunaLexer_addToken(self, _lunaLexer_matchChar(self, '=') ? LUNA_TK_SELF_DIV : LUNA_TK_DIV);
        break;
    case '%': _lunaLexer_addToken(self, _lunaLexer_matchChar(self, '=') ? LUNA_TK_SELF_MOD : LUNA_TK_MOD); break;
    case '&': 
        if (_lunaLexer_matchChar(self, '=')) _lunaLexer_addToken(self, LUNA_TK_SELF_BIT_AND);
        else if (_lunaLexer_matchChar(self, '&')) _lunaLexer_addToken(self, LUNA_TK_AND);
        else _lunaLexer_addToken(self, LUNA_TK_BIT_AND);
        break;
    case '|': 
        if (_lunaLexer_matchChar(self, '=')) _lunaLexer_addToken(self, LUNA_TK_SELF_BIT_OR);
        else if (_lunaLexer_matchChar(self, '|')) _lunaLexer_addToken(self, LUNA_TK_OR);
        else _lunaLexer_addToken(self, LUNA_TK_BIT_OR);
        break;
    case '^': _lunaLexer_addToken(self, _lunaLexer_matchChar(self, '=') ? LUNA_TK_SELF_BIT_XOR : LUNA_TK_BIT_XOR); break;
    case '`': _lunaLexer_addToken(self, _lunaLexer_matchChar(self, '=') ? LUNA_TK_SELF_BIT_XNOR : LUNA_TK_BIT_XNOR); break;
    case '~': _lunaLexer_addToken(self, _lunaLexer_matchChar(self, '=') ? LUNA_TK_SELF_BIT_NOT : LUNA_TK_BIT_NOT); break;
    case '=': _lunaLexer_addToken(self, _lunaLexer_matchChar(self, '=') ? LUNA_TK_EQ : LUNA_TK_ASSIGN); break;
    case '!': _lunaLexer_addToken(self, _lunaLexer_matchChar(self, '=') ? LUNA_TK_NE : LUNA_TK_NOT); break;
    case '<':
        if (_lunaLexer_matchString(self, "<<=", 0)) _lunaLexer_addToken(self, LUNA_TK_SELF_BIT_SHL);
        else if (_lunaLexer_matchString(self, "<=", 0)) _lunaLexer_addToken(self, LUNA_TK_LE);
        else if (_lunaLexer_matchString(self, "<<", 0)) _lunaLexer_addToken(self, LUNA_TK_BIT_SHL);
        else if (_lunaLexer_matchString(self, "<-", 0)) _lunaLexer_addToken(self, LUNA_TK_LARROW);
        else _lunaLexer_addToken(self, LUNA_TK_LT);
        break;
    case '>':
        if (_lunaLexer_matchString(self, ">>=", 0)) _lunaLexer_addToken(self, LUNA_TK_SELF_BIT_SHR);
        else if (_lunaLexer_matchString(self, ">=", 0)) _lunaLexer_addToken(self, LUNA_TK_GE);
        else if (_lunaLexer_matchString(self, ">>", 0)) _lunaLexer_addToken(self, LUNA_TK_BIT_SHR);
        else _lunaLexer_addToken(self, LUNA_TK_GT);
        break;
    case '"': _lunaLexer_lexString(self, '"'); break;
    case '\'': _lunaLexer_lexString(self, '\''); break;
    case 'r': 
        if (_lunaLexer_peek(self) == '\'' || _lunaLexer_peek(self) == '"') 
            _lunaLexer_lexRawString(self, _lunaLexer_advance(self)); 
        else 
            _lunaLexer_lexOther(self, 'r'); 
        break;
    default: _lunaLexer_lexOther(self, c); break;
    }
}
static void _lunaLexer_moveCursor(luna_Lexer *self)
{
    self->cursor.curr++;
    self->cursor.pos.column++;
}
static void _lunaLexer_moveCursorln(luna_Lexer *self)
{
    self->cursor.curr++;
    self->cursor.pos.line++;
    self->cursor.pos.column = 1;
}
static void _lunaLexer_smartMoveCursor(luna_Lexer *self, char c)
{
    if (c == '\n') _lunaLexer_moveCursorln(self);
    else _lunaLexer_moveCursor(self);
}
#pragma endregion
#pragma region Kan/Move
static bool _lunaLexer_isAtEnd(luna_Lexer *self)
{
    return (size_t)(self->cursor.curr - self->source) >= self->source_len;
}
static bool _lunaLexer_isAtEndOffset(luna_Lexer *self, size_t offset)
{
    return (size_t)(self->cursor.curr - self->source) + offset >= self->source_len;
}
static char _lunaLexer_advance(luna_Lexer *self)
{
    if (_lunaLexer_isAtEnd(self)) return '\0';
    char c = *self->cursor.curr;
    _lunaLexer_smartMoveCursor(self, c);
    return c;
}
static void _lunaLexer_pass(luna_Lexer *self)
{
    if (_lunaLexer_isAtEnd(self)) return;
    _lunaLexer_smartMoveCursor(self, *self->cursor.curr);
}
static void _lunaLexer_passStep(luna_Lexer *self, size_t step)
{
    while (step > 0)
    {
        if (_lunaLexer_isAtEnd(self)) return;
        _lunaLexer_smartMoveCursor(self, *self->cursor.curr);
        step--;
    }
}
static bool _lunaLexer_matchChar(luna_Lexer *self, char c)
{
    if (_lunaLexer_isAtEnd(self) || *self->cursor.curr != c)
        return false;
    _lunaLexer_smartMoveCursor(self, c);
    return true;
}
static bool _lunaLexer_matchString(luna_Lexer *self, const char *s, size_t omit)
{
    size_t i = 0;
    const char *p = s + omit;
    for (; *p != '\0'; p++)
    {
        if (_lunaLexer_isAtEndOffset(self, i))
            return false;
        if (_lunaLexer_peekOffset(self, i) != *p)
            return false;
        i++;
    }
    _lunaLexer_passStep(self, i);
    return true;
}
static char _lunaLexer_peek(luna_Lexer *self)
{
    if (_lunaLexer_isAtEnd(self)) return '\0';
    return *self->cursor.curr;
}
static char _lunaLexer_peekOffset(luna_Lexer *self, size_t offset)
{
    if (_lunaLexer_isAtEndOffset(self, offset)) return '\0';
    return *(self->cursor.curr + offset);
}
#pragma endregion
#pragma region Skip
static void _lunaLexer_skipNote(luna_Lexer *self)
{
    while (_lunaLexer_peek(self) != '\n' && !_lunaLexer_isAtEnd(self))
        _lunaLexer_moveCursor(self);
}
static void _lunaLexer_skipWhite(luna_Lexer *self)
{
    while (!_lunaLexer_isAtEnd(self))
    {
        char c = _lunaLexer_peek(self);
        switch (c)
        {
        case ' ':
        case '\t':
        case '\r':
            _lunaLexer_moveCursor(self);
            break;
        case '\n':
            _lunaLexer_moveCursorln(self);
            break;
        default:
            return;
        }
    }
}
#pragma endregion
#pragma region Lex
static void _lunaLexer_lexOther(luna_Lexer *self, char c)
{
    if (isdigit(c))
    {
        _lunaLexer_lexNumber(self);
        return;
    }
    else if (isalpha(c) || c == '_')
    {
        _lunaLexer_lexIdentifier(self);
        return;
    }
    // TODO: Error
    fprintf(stderr, "未知的字符 '%c' (HEX: 0x%02x) [%04zu:%04zu]\n", c, (unsigned char)c, self->cursor.pos.line, self->cursor.pos.column);
}
static void _lunaLexer_lexNumber(luna_Lexer *self)
{
    bool is_float = false;
    const char *start = self->cursor.start;
    
    if (_lunaLexer_peek(self) == '0')
    {
        char next = _lunaLexer_peekOffset(self, 1);
        // 二进制
        if (next == 'b' || next == 'B')
        {
            _lunaLexer_passStep(self, 2);
            while (_isBdigit(_lunaLexer_peek(self))) _lunaLexer_pass(self);
            size_t lexeme_len = self->cursor.curr - start;
            
            long long value = 0;
            for (size_t i = 2; i < lexeme_len; ++i)
                value = (value << 1) | (start[i] - '0');
            
            luna_Value literal;
            literal.type = LUNA_VT_INTEGER;
            literal.data.i = value;
            _lunaLexer_addTokenFull(self, LUNA_TK_LIT_INTEGER, start, lexeme_len, literal);
            return;
        }
        // 八进制
        if (next == 'o' || next == 'O')
        {
            _lunaLexer_passStep(self, 2);
            while (_isOdigit(_lunaLexer_peek(self))) _lunaLexer_pass(self);
            size_t lexeme_len = self->cursor.curr - start;
            long long value = 0;
            for (size_t i = 2; i < lexeme_len; ++i)
                value = value * 8 + (start[i] - '0');
            luna_Value literal;
            literal.type = LUNA_VT_INTEGER;
            literal.data.i = value;
            _lunaLexer_addTokenFull(self, LUNA_TK_LIT_INTEGER, start, lexeme_len, literal);
            return;
        }
        // 十六进制
        if (next == 'x' || next == 'X')
        {
            _lunaLexer_passStep(self, 2);
            while (isxdigit(_lunaLexer_peek(self))) _lunaLexer_pass(self);
            size_t lexeme_len = self->cursor.curr - start;
            long long value = 0;
            for (size_t i = 2; i < lexeme_len; ++i)
            {
                char c = start[i];
                if (isdigit(c))
                    value = value * 16 + (c - '0');
                else
                    value = value * 16 + (tolower(c) - 'a' + 10);
            }
            luna_Value literal;
            literal.type = LUNA_VT_INTEGER;
            literal.data.i = value;
            _lunaLexer_addTokenFull(self, LUNA_TK_LIT_INTEGER, start, lexeme_len, literal);
            return;
        }
    }
    // 整数部分
    while (isdigit(_lunaLexer_peek(self))) _lunaLexer_pass(self);
    // 浮点数
    if (_lunaLexer_peek(self) == '.' && isdigit(_lunaLexer_peekOffset(self, 1)))
    {
        is_float = true;
        _lunaLexer_pass(self);
        while (isdigit(_lunaLexer_peek(self))) _lunaLexer_pass(self);
    }
    // 科学计数法
    if (_lunaLexer_peek(self) == 'e' || _lunaLexer_peek(self) == 'E')
    {
        char next = _lunaLexer_peekOffset(self, 1);
        if (isdigit(next) || next == '+' || next == '-')
        {
            is_float = true;
            _lunaLexer_pass(self);
            if (_lunaLexer_peek(self) == '+' || _lunaLexer_peek(self) == '-')
                _lunaLexer_pass(self);
            if (isdigit(_lunaLexer_peek(self)))
            {
                _lunaLexer_pass(self);
                while (isdigit(_lunaLexer_peek(self))) _lunaLexer_pass(self);
            }
            else
            {
                // TODO: Error
                fprintf(stderr, "科学计数法格式错误 [%04zu:%04zu]\n", self->cursor.pos.line, self->cursor.pos.column);
                return;
            }
        }
    }
    // 处理数字
    size_t lexeme_len = self->cursor.curr - start;
    char *endptr;
    
    if (is_float)
    {
        double value = strtod(start, &endptr);
        luna_Value literal;
        literal.type = LUNA_VT_NUMBER;
        literal.data.n = value;
        _lunaLexer_addTokenFull(self, LUNA_TK_LIT_NUMBER, start, lexeme_len, literal);
    }
    else
    {
        long long value = strtoll(start, &endptr, 10);
        luna_Value literal;
        literal.type = LUNA_VT_INTEGER;
        literal.data.i = value;
        _lunaLexer_addTokenFull(self, LUNA_TK_LIT_INTEGER, start, lexeme_len, literal);
    }
}
static void _lunaLexer_lexString(luna_Lexer *self, char beg)
{
    char buffer[1024];
    size_t buf_pos = 0;
    while (_lunaLexer_peek(self) != beg && !_lunaLexer_isAtEnd(self))
    {
        char c = _lunaLexer_advance(self);
        if (c == '\\')
        {
            if (_lunaLexer_isAtEnd(self)) break;
            char next = _lunaLexer_advance(self);
            switch (next)
            {
                case 'n':  buffer[buf_pos++] = '\n'; break;
                case 'r':  buffer[buf_pos++] = '\r'; break;
                case 't':  buffer[buf_pos++] = '\t'; break;
                case '\\': buffer[buf_pos++] = '\\'; break;
                case '"':  buffer[buf_pos++] = '"';  break;
                case '\'': buffer[buf_pos++] = '\''; break;
                default: 
                    buffer[buf_pos++] = c; 
                    buffer[buf_pos++] = next; 
                    break;
            }
        }
        else
        {
            buffer[buf_pos++] = c;
        }
        if (buf_pos >= sizeof(buffer) - 1) break;
    }
    if (_lunaLexer_isAtEnd(self))
    {
        // TODO: Error
        fprintf(stderr, "字符串未闭合 [%04zu:%04zu]\n", self->cursor.pos.line, self->cursor.pos.column);
        return;
    }
    _lunaLexer_pass(self);
    buffer[buf_pos] = '\0';
    
    luna_Value literal;
    // TODO: String 字面量
    // literal.string = strdup(buffer);
    _lunaLexer_addTokenLiteral(self, LUNA_TK_LIT_STRING, literal);
}
static void _lunaLexer_lexRawString(luna_Lexer *self, char beg)
{
    char buffer[1024];
    size_t buf_pos = 0;
    while (_lunaLexer_peek(self) != beg && !_lunaLexer_isAtEnd(self))
    {
        char c = _lunaLexer_advance(self);
        // 转义字符
        if (c == '\\')
        {
            if (_lunaLexer_isAtEnd(self)) break;
            char next = _lunaLexer_advance(self);
            switch (next)
            {
                case '\\': buffer[buf_pos++] = '\\'; break;
                case '"':  buffer[buf_pos++] = '"';  break;
                case '\'': buffer[buf_pos++] = '\''; break;
                default: 
                    buffer[buf_pos++] = c; 
                    buffer[buf_pos++] = next; 
                    break;
            }
        }
        else
        {
            buffer[buf_pos++] = c;
        }
        if (buf_pos >= sizeof(buffer) - 1) break;
    }
    if (_lunaLexer_isAtEnd(self))
    {
        // TODO: Error
        fprintf(stderr, "字符串未闭合 [%04zu:%04zu]\n", self->cursor.pos.line, self->cursor.pos.column);
        return;
    }
    _lunaLexer_pass(self);
    buffer[buf_pos] = '\0';
    // TODO: String 字面量
    luna_Value literal;
    // literal.string = strdup(buffer);
    _lunaLexer_addTokenLiteral(self, LUNA_TK_LIT_STRING, literal);
}
static void _lunaLexer_lexIdentifier(luna_Lexer *self)
{
    while (isalnum(_lunaLexer_peek(self)) || _lunaLexer_peek(self) == '_') _lunaLexer_pass(self);
    size_t lexeme_len = self->cursor.curr - self->cursor.start;
    const char *lexeme = self->cursor.start;
    luna_TokenType type = lunaTokenType_fromKeyword(lexeme, lexeme_len);
    luna_Value literal = lunaToken_toLiteral(type);
    _lunaLexer_addTokenFull(self, type, lexeme, lexeme_len, literal);
}
#pragma endregion
#pragma region Xie
static void _lunaLexer_addToken(luna_Lexer *self, luna_TokenType type)
{
    size_t lexeme_len = self->cursor.curr - self->cursor.start;
    const char *lexeme = self->cursor.start;
    
    luna_Token token;
    token.type = type;
    token.lexeme.start = lexeme;
    token.lexeme.len = lexeme_len;
    token.literal = (luna_Value){0};
    token.pos = self->cursor.pos;
    lunaTokenArray_write(&self->tokens, token);
}
static void _lunaLexer_addTokenLiteral(luna_Lexer *self, luna_TokenType type, luna_Value literal)
{
    size_t lexeme_len = self->cursor.curr - self->cursor.start;
    const char *lexeme = self->cursor.start;
    
    luna_Token token;
    token.type = type;
    token.lexeme.start = lexeme;
    token.lexeme.len = lexeme_len;
    token.literal = literal;
    token.pos = self->cursor.pos;
    lunaTokenArray_write(&self->tokens, token);
}
static void _lunaLexer_addTokenFull(luna_Lexer *self, luna_TokenType type, const char *lexeme, size_t lexeme_len, luna_Value literal)
{
    luna_Token token;
    token.type = type;
    token.lexeme.start = lexeme;
    token.lexeme.len = lexeme_len;
    token.literal = literal;
    token.pos = self->cursor.pos;
    lunaTokenArray_write(&self->tokens, token);
}
#pragma endregion
