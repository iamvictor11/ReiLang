#include "rei_lexer.h"
#include "rei_token.word_cloud.i"
#include <ctype.h>
#include <string.h>
#include "utils/rei_str.h"
#include "../rei_debug.h"

typedef struct Rei_LexerState
{
    const char* start;
    const char* prev;
    const char* curr;
    uint32_t line;
    ReiResult res;
} Rei_LexerState;

static Rei_LexerState lexerState_ = {0};

#pragma region Private
static char peekPrev_(void);
static char peek_(void);
static char peekNext_(void);
static char peekNextNext_(void);
static char advance_(void);
static bool isAtEnd_(void);
static bool isDigit_(char ch);
static bool isCDigit_(char ch);
static bool isAlpha_(char ch);
static bool isAlphaDigit_(char ch);
static void skipWhitespace_(void);
static ReiToken makeToken_(ReiTokenKind kind);
static ReiToken makeErrorToken_(const char* message);
static ReiTokenKind identifierType_(void);
static ReiToken scanString_(char quote);
static ReiToken scanNumber_(void);
static ReiToken scanIdentifier_(void);
static ReiToken scanOperator_(void);
static ReiToken scanToken_(void);
#pragma endregion
#pragma region Public
bool reiLexerInit(ReiLexer* me, const char* source)
{
    me->source = source;
    me->tokens = reiTokenBufferCreate(NULL);
    if (me->tokens == NULL)
        return false;
    lexerState_.start = me->source;
    lexerState_.curr = me->source;
    lexerState_.line = 1;
    lexerState_.res = REI_RESULT_SUCCESS;
    return true;
}
void reiLexerFree(ReiLexer* me)
{
    reiTokenBufferDestroy(me->tokens);
}
ReiResult reiLexerStart(ReiLexer* me)
{
    if (me == NULL || me->source == NULL) return REI_RESULT_LEXER_ERROR;
    while (!isAtEnd_())
    {
        ReiToken token = scanToken_();
        reiTokenBufferPush(me->tokens, &token);
        if (token.kind == REI_TOKEN_KIND_EOF) break;
    }
    ReiToken* lastToken = reiTokenBufferBack(me->tokens);
    if (reiTokenBufferEmpty(me->tokens) || lastToken->kind != REI_TOKEN_KIND_EOF)
    {
        ReiToken eof;
        eof.kind = REI_TOKEN_KIND_EOF;
        eof.lexeme.start = "";
        eof.lexeme.length = 0;
        eof.line = lexerState_.line;
        eof.literal = REI_MK_NIL;
        reiTokenBufferPush(me->tokens, &eof);
    }
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_TRACE
    if (lexerState_.res == REI_RESULT_SUCCESS)
    {
        printf("lexer tokens: %zu\n", me->tokens->size);
        char buff[REI_MAX_BUFFER_SIZE] = {0};
        C_TEMPLATE_VECTOR_FOREACH(ReiToken, token, me->tokens)
            REI_DEBUG_LOG_TRACE(reiTokenToCstr(token, buff, REI_MAX_BUFFER_SIZE));
    }
#endif
    return lexerState_.res;
}
#pragma endregion
#pragma region PIMPL
static char peekPrev_(void)
{
    if (lexerState_.prev == NULL) return '\0';
    return *lexerState_.prev;
}
static char peek_(void)
{
    return *lexerState_.curr;
}
static char peekNext_(void)
{
    if (peek_() == '\0') return '\0';
    return lexerState_.curr[1];
}
static char peekNextNext_(void)
{
    if (peekNext_() == '\0') return '\0';
    return lexerState_.curr[2];
}
static char advance_(void)
{
    char ch = peek_();
    lexerState_.curr++;
    if (ch == '\n') lexerState_.line++;
    return ch;
}
static bool isAtEnd_(void)
{
    return peek_() == '\0';
}
static bool isDigit_(char ch)
{
    return ch >= '0' && ch <= '9';
}
static bool isCDigit_(char ch)
{
    return ch == '\'' || isDigit_(ch);
}
static bool isAlpha_(char ch)
{
    return (ch >= 'a' && ch <= 'z') ||
           (ch >= 'A' && ch <= 'Z') ||
           ch == '_';
}
static bool isAlphaDigit_(char ch)
{
    return isAlpha_(ch) || isDigit_(ch);
}
static void skipWhitespace_(void)
{
    while (!isAtEnd_())
    {
        char ch = peek_();
        if (ch == ' ' || ch == '\r' || ch == '\t')
        {
            advance_();
        }
        else if (ch == '\n')
        {
            advance_();
        }
        else if (ch == '/' && peekNext_() == '/')
        {
            while (!isAtEnd_() && peek_() != '\n') advance_();
            if (!isAtEnd_()) advance_();
        }
        else if (ch == '/' && peekNext_() == '*')
        {
            advance_(); advance_();
            while (!isAtEnd_())
            {
                if (peek_() == '*' && peekNext_() == '/')
                {
                    advance_(); advance_();
                    break;
                }
                advance_();
            }
        }
        else
        {
            break;
        }
    }
}
static ReiToken makeToken_(ReiTokenKind kind)
{
    ReiToken token;
    token.kind = kind;
    token.lexeme.start = lexerState_.start;
    token.lexeme.length = (int)(lexerState_.curr - lexerState_.start);
    token.line = lexerState_.line;
    token.literal = REI_MK_NIL;
    return token;
}
static ReiToken makeErrorToken_(const char* message)
{
    REI_DEBUG_LOG_ERROR(message);
    lexerState_.res = REI_RESULT_LEXER_ERROR;
    ReiToken token;
    token.kind = REI_TOKEN_KIND_EOF;
    token.lexeme.start = lexerState_.start;
    token.lexeme.length = (int)(lexerState_.curr - lexerState_.start);
    token.line = lexerState_.line;
    token.literal = REI_MK_NIL;
    return token;
}
static ReiTokenKind identifierType_(void)
{
    int length = lexerState_.curr - lexerState_.start;
    char buffer[REI_MAX_IDENTIFIER_NAME_LEN] = {0};
    if (length >= (int)sizeof(buffer)) return REI_TOKEN_KIND_IDENTIFIER;
    memcpy(buffer, lexerState_.start, length);
    for (int i = 0; wordCloud_[i] != NULL; i++)
        if (strcmp(buffer, wordCloud_[i]) == 0)
            return (ReiTokenKind)(i);
    return REI_TOKEN_KIND_IDENTIFIER;
}
static ReiToken scanString_(char quote)
{
    lexerState_.start = lexerState_.curr - 1;
    
    while (!isAtEnd_() && peek_() != quote)
    {
        if (peek_() == '\\')
        {
            advance_();
            if (!isAtEnd_()) advance_();
        }
        else
        {
            advance_();
        }
    }
    if (isAtEnd_())
    {
        return makeErrorToken_("字符串 未闭合！");
    }
    advance_();
    ReiToken token = makeToken_(REI_TOKEN_KIND_STRING);
    token.literal = REI_MK_NIL;
    
    return token;
}
static ReiToken scanNumber_(void)
{
    lexerState_.start = lexerState_.curr - 1;
    while (isCDigit_(peek_())) advance_();
    ReiTokenKind kind = REI_TOKEN_KIND_INT;
    if ((peek_() == 'b' || peek_() == 'B' ||
        peek_() == 'o' || peek_() == 'O' ||
        peek_() == 'x' || peek_() == 'X') &&
        isCDigit_(peekNext_()))
    {
        advance_();
        while (isCDigit_(peek_())) advance_();
    }
    else if (peek_() == '.' && isCDigit_(peekNext_()))
    {
        kind = REI_TOKEN_KIND_FLOAT;
        advance_();
        while (isCDigit_(peek_())) advance_();
    }
    else if ((peek_() == 'e' || peek_() == 'E') &&
        (peekNext_() == '+' || peekNext_() == '-') &&
        isCDigit_(peekNextNext_()))
    {
        kind = REI_TOKEN_KIND_FLOAT;
        advance_();
        advance_();
        while (isCDigit_(peek_())) advance_();
    }
    ReiToken token = makeToken_(kind);
    int length = lexerState_.curr - lexerState_.start;
    char buffer[length];
    memset(buffer, 0, length);
    memcpy(buffer, lexerState_.start, length);
    if (kind == REI_TOKEN_KIND_INT)
        token.literal = REI_MK_INTEGER(reiCstrToInt(buffer));
    else if (kind == REI_TOKEN_KIND_FLOAT)
        token.literal = REI_MK_FLOATING(reiCharsToFloat(buffer));
    return token;
}
static ReiToken scanIdentifier_(void)
{
    lexerState_.start = lexerState_.curr - 1;
    while (isAlphaDigit_(peek_())) advance_();
    ReiTokenKind kind = identifierType_();
    return makeToken_(kind);
}
static ReiToken scanOperator_(void)
{
    lexerState_.start = lexerState_.curr - 1;
    char ch = lexerState_.start[0];
    switch (ch)
    {
        case '(': return makeToken_(REI_TOKEN_KIND_LEFT_PAREN);
        case ')': return makeToken_(REI_TOKEN_KIND_RIGHT_PAREN);
        case '[':
            if (peek_() == '[') { advance_(); return makeToken_(REI_TOKEN_KIND_LEFT_BRACKET_BRACKET); }
            return makeToken_(REI_TOKEN_KIND_LEFT_BRACKET);
        case ']':
            if (peek_() == ']') { advance_(); return makeToken_(REI_TOKEN_KIND_RIGHT_BRACKET_BRACKET); }
            return makeToken_(REI_TOKEN_KIND_RIGHT_BRACKET);
        case '{': return makeToken_(REI_TOKEN_KIND_LEFT_BRACE);
        case '}': return makeToken_(REI_TOKEN_KIND_RIGHT_BRACE);
        case ',': return makeToken_(REI_TOKEN_KIND_COMMA);
        case '?': return makeToken_(REI_TOKEN_KIND_QUESTION);
        case ';': return makeToken_(REI_TOKEN_KIND_SEMICOLON);
        case '#': return makeToken_(REI_TOKEN_KIND_POUND);
        case '@': return makeToken_(REI_TOKEN_KIND_AT);
        case '.':
            if (peek_() == '.' && lexerState_.curr[1] == '.')
            {
                advance_(); advance_();
                return makeToken_(REI_TOKEN_KIND_DOT_DOT_DOT);
            }
            return makeToken_(REI_TOKEN_KIND_DOT);
        case ':':
            if (peek_() == ':') { advance_(); return makeToken_(REI_TOKEN_KIND_COLON_COLON); }
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_WALRUS); }
            return makeToken_(REI_TOKEN_KIND_COLON);
        case '+':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_PLUS_EQUAL); }
            return makeToken_(REI_TOKEN_KIND_PLUS);
        case '-':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_MINUS_EQUAL); }
            if (peek_() == '>') { advance_(); return makeToken_(REI_TOKEN_KIND_RIGHT_ARROW); }
            return makeToken_(REI_TOKEN_KIND_MINUS);
        case '*':
            if (peek_() == '*')
            {
                advance_();
                if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_STAR_STAR_EQUAL); }
                return makeToken_(REI_TOKEN_KIND_STAR_STAR);
            }
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_STAR_EQUAL); }
            return makeToken_(REI_TOKEN_KIND_STAR);
        case '/':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_SLASH_EQUAL); }
            return makeToken_(REI_TOKEN_KIND_SLASH);
        case '%':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_MODULO_EQUAL); }
            return makeToken_(REI_TOKEN_KIND_MODULO);
        case '&':
            if (peek_() == '&') { advance_(); return makeToken_(REI_TOKEN_KIND_AND_AND); }
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_AND_EQUAL); }
            return makeToken_(REI_TOKEN_KIND_AND);
        case '|':
            if (peek_() == '|') { advance_(); return makeToken_(REI_TOKEN_KIND_PIPE_PIPE); }
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_PIPE_EQUAL); }
            return makeToken_(REI_TOKEN_KIND_PIPE);
        case '~':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_WAVE_EQUAL); }
            return makeToken_(REI_TOKEN_KIND_WAVE);
        case '!':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_BANG_EQUAL); }
            return makeToken_(REI_TOKEN_KIND_BANG);
        case '<':
            if (peek_() == '<')
            {
                advance_();
                if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_LESS_LESS_EQUAL); }
                return makeToken_(REI_TOKEN_KIND_LESS_LESS);
            }
            if (peek_() == '=')
            {
                advance_();
                if (peek_() == '>') { advance_(); return makeToken_(REI_TOKEN_KIND_COMPARE); }
                return makeToken_(REI_TOKEN_KIND_LESS_EQUAL);
            }
            if (peek_() == '-') { advance_(); return makeToken_(REI_TOKEN_KIND_LEFT_ARROW); }
            return makeToken_(REI_TOKEN_KIND_LESS);
        case '>':
            if (peek_() == '>')
            {
                advance_();
                if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_GREATER_GREATER_EQUAL); }
                return makeToken_(REI_TOKEN_KIND_GREATER_GREATER);
            }
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_GREATER_EQUAL); }
            return makeToken_(REI_TOKEN_KIND_GREATER);
        case '=':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_KIND_EQUAL_EQUAL); }
            return makeToken_(REI_TOKEN_KIND_EQUAL);
        default:
            return makeErrorToken_("未定义的 符号！");
    }
}
static ReiToken scanToken_(void)
{
    skipWhitespace_();
    if (isAtEnd_())
    {
        lexerState_.start = lexerState_.curr;
        ReiToken token;
        token.kind = REI_TOKEN_KIND_EOF;
        token.lexeme.start = "";
        token.lexeme.length = 0;
        token.line = lexerState_.line;
        token.literal = REI_MK_NIL;
        return token;
    }
    lexerState_.start = lexerState_.curr;
    char ch = advance_();
    if (isDigit_(ch)) return scanNumber_();
    if (isAlpha_(ch)) return scanIdentifier_();
    if (ch == '"' || ch == '\'')
        return scanString_(ch);
    return scanOperator_();
}
#pragma endregion
