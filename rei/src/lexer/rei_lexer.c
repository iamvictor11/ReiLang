#include "rei_lexer.h"
#include <ctype.h>
#include <string.h>
#include "rei_word_cloud.i"
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
static ReiToken makeToken_(ReiTokenType type);
static ReiToken makeErrorToken_(void);
static ReiTokenType identifierType_(void);
static ReiToken scanString_(char quote);
static ReiToken scanNumber_(void);
static ReiToken scanIdentifier_(void);
static ReiToken scanOperator_(void);
static ReiToken scanToken_(void);
#pragma endregion
#pragma region Public
void reiLexerInit(ReiLexer* me, const char* source)
{
    me->source = source;
    me->tokens = reiTokenBufferCreate(NULL);
}
void reiLexerFree(ReiLexer* me)
{
    reiTokenBufferDestroy(me->tokens);
}
ReiResult reiLexerStart(ReiLexer* me)
{
    if (me == NULL || me->source == NULL) return (ReiResult){REI_STATUS_LEXER_ERROR, REI_MK_NIL};
    lexerState_.start = me->source;
    lexerState_.curr = me->source;
    lexerState_.line = 1;
    lexerState_.res = (ReiResult){REI_STATUS_SUCCESS, REI_MK_NIL};
    while (!isAtEnd_())
    {
        ReiToken token = scanToken_();
        reiTokenBufferPush(me->tokens, &token);
        if (token.type == REI_TOKEN_TYPE_EOF) break;
    }
    ReiToken* lastToken = reiTokenBufferBack(me->tokens);
    if (reiTokenBufferEmpty(me->tokens) || lastToken->type != REI_TOKEN_TYPE_EOF)
    {
        ReiToken eof;
        eof.type = REI_TOKEN_TYPE_EOF;
        eof.lexeme.start = "";
        eof.lexeme.length = 0;
        eof.line = lexerState_.line;
        eof.literal = REI_MK_NIL;
        reiTokenBufferPush(me->tokens, &eof);
    }
#if REI_ENABLE_DEBUG >= REI_DEBUG_LEVEL_TRACE
    if (lexerState_.res.status == REI_STATUS_SUCCESS)
    {
        printf("tokens: %zu\n", me->tokens->size);
        char buff[REI_MAX_BUFFER_SIZE] = {0};
        C_TEMPLATE_VECTOR_FOREACH(ReiToken, token, me->tokens)
            REI_DEBUG_LOG_TRACE(reiTokenToCstr(token, buff, REI_MAX_BUFFER_SIZE));
    }
#endif
    return lexerState_.res;
}
#pragma endregion
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
    if (peek_() == '\0') return '\0';
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
static ReiToken makeToken_(ReiTokenType type)
{
    ReiToken token;
    token.type = type;
    token.lexeme.start = lexerState_.start;
    token.lexeme.length = (int)(lexerState_.curr - lexerState_.start);
    token.line = lexerState_.line;
    token.literal = REI_MK_NIL;
    return token;
}
static ReiToken makeErrorToken_(void)
{
    lexerState_.res = (ReiResult){REI_STATUS_LEXER_ERROR, REI_MK_NIL};
    ReiToken token;
    token.type = REI_TOKEN_TYPE_EOF;
    token.lexeme.start = lexerState_.start;
    token.lexeme.length = (int)(lexerState_.curr - lexerState_.start);
    token.line = lexerState_.line;
    token.literal = REI_MK_NIL;
    return token;
}
static ReiTokenType identifierType_(void)
{
    int length = lexerState_.curr - lexerState_.start;
    char buffer[REI_MAX_IDENTIFIER_NAME_LEN] = {0};
    if (length >= (int)sizeof(buffer)) return REI_TOKEN_TYPE_IDENTIFIER;
    memcpy(buffer, lexerState_.start, length);
    for (int i = 0; wordCloud_[i] != NULL; i++)
        if (strcmp(buffer, wordCloud_[i]) == 0)
            return (ReiTokenType)(REI_TOKEN_TYPE_NIL + i);
    return REI_TOKEN_TYPE_IDENTIFIER;
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
        return makeErrorToken_();
    }
    advance_();
    ReiToken token = makeToken_(REI_TOKEN_TYPE_STRING);
    token.literal = REI_MK_NIL;
    
    return token;
}
static ReiToken scanNumber_(void)
{
    lexerState_.start = lexerState_.curr - 1;
    while (isCDigit_(peek_())) advance_();
    ReiTokenType type = REI_TOKEN_TYPE_INT;
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
        type = REI_TOKEN_TYPE_FLOAT;
        advance_();
        while (isCDigit_(peek_())) advance_();
    }
    else if ((peek_() == 'e' || peek_() == 'E') &&
        (peekNext_() == '+' || peekNext_() == '-') &&
        isCDigit_(peekNextNext_()))
    {
        type = REI_TOKEN_TYPE_FLOAT;
        advance_();
        advance_();
        while (isCDigit_(peek_())) advance_();
    }
    ReiToken token = makeToken_(type);
    int length = lexerState_.curr - lexerState_.start;
    char buffer[length];
    memset(buffer, 0, length);
    memcpy(buffer, lexerState_.start, length);
    if (type == REI_TOKEN_TYPE_INT)
        token.literal = REI_MK_INT(reiCstrToInt(buffer));
    else if (type == REI_TOKEN_TYPE_FLOAT)
        token.literal = REI_MK_FLOAT(reiCharsToFloat(buffer));
    return token;
}
static ReiToken scanIdentifier_(void)
{
    lexerState_.start = lexerState_.curr - 1;
    while (isAlphaDigit_(peek_())) advance_();
    ReiTokenType type = identifierType_();
    return makeToken_(type);
}
static ReiToken scanOperator_(void)
{
    lexerState_.start = lexerState_.curr - 1;
    char ch = lexerState_.start[0];
    switch (ch)
    {
        case '(': return makeToken_(REI_TOKEN_TYPE_LEFT_PAREN);
        case ')': return makeToken_(REI_TOKEN_TYPE_RIGHT_PAREN);
        case '[':
            if (peek_() == '[') { advance_(); return makeToken_(REI_TOKEN_TYPE_LEFT_BRACKET_BRACKET); }
            return makeToken_(REI_TOKEN_TYPE_LEFT_BRACKET);
        case ']':
            if (peek_() == ']') { advance_(); return makeToken_(REI_TOKEN_TYPE_RIGHT_BRACKET_BRACKET); }
            return makeToken_(REI_TOKEN_TYPE_RIGHT_BRACKET);
        case '{': return makeToken_(REI_TOKEN_TYPE_LEFT_BRACE);
        case '}': return makeToken_(REI_TOKEN_TYPE_RIGHT_BRACE);
        case ',': return makeToken_(REI_TOKEN_TYPE_COMMA);
        case '?': return makeToken_(REI_TOKEN_TYPE_QUESTION);
        case ';': return makeToken_(REI_TOKEN_TYPE_SEMICOLON);
        case '#': return makeToken_(REI_TOKEN_TYPE_POUND);
        case '@': return makeToken_(REI_TOKEN_TYPE_AT);
        case '.':
            if (peek_() == '.' && lexerState_.curr[1] == '.')
            {
                advance_(); advance_();
                return makeToken_(REI_TOKEN_TYPE_DOT_DOT_DOT);
            }
            return makeToken_(REI_TOKEN_TYPE_DOT);
        case ':':
            if (peek_() == ':') { advance_(); return makeToken_(REI_TOKEN_TYPE_COLON_COLON); }
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_COLON_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_COLON);
        case '+':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_PLUS_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_PLUS);
        case '-':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_MINUS_EQUAL); }
            if (peek_() == '>') { advance_(); return makeToken_(REI_TOKEN_TYPE_RIGHT_ARROW); }
            return makeToken_(REI_TOKEN_TYPE_MINUS);
        case '*':
            if (peek_() == '*')
            {
                advance_();
                if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_STAR_STAR_EQUAL); }
                return makeToken_(REI_TOKEN_TYPE_STAR_STAR);
            }
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_STAR_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_STAR);
        case '/':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_SLASH_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_SLASH);
        case '%':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_MODULO_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_MODULO);
        case '&':
            if (peek_() == '&') { advance_(); return makeToken_(REI_TOKEN_TYPE_AND_AND); }
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_AND_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_AND);
        case '|':
            if (peek_() == '|') { advance_(); return makeToken_(REI_TOKEN_TYPE_PIPE_PIPE); }
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_PIPE_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_PIPE);
        case '~':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_WAVE_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_WAVE);
        case '!':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_BANG_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_BANG);
        case '<':
            if (peek_() == '<')
            {
                advance_();
                if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_LESS_LESS_EQUAL); }
                return makeToken_(REI_TOKEN_TYPE_LESS_LESS);
            }
            if (peek_() == '=')
            {
                advance_();
                if (peek_() == '>') { advance_(); return makeToken_(REI_TOKEN_TYPE_COMPARE); }
                return makeToken_(REI_TOKEN_TYPE_LESS_EQUAL);
            }
            if (peek_() == '-') { advance_(); return makeToken_(REI_TOKEN_TYPE_LEFT_ARROW); }
            return makeToken_(REI_TOKEN_TYPE_LESS);
        case '>':
            if (peek_() == '>')
            {
                advance_();
                if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_GREATER_GREATER_EQUAL); }
                return makeToken_(REI_TOKEN_TYPE_GREATER_GREATER);
            }
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_GREATER_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_GREATER);
        case '=':
            if (peek_() == '=') { advance_(); return makeToken_(REI_TOKEN_TYPE_EQUAL_EQUAL); }
            return makeToken_(REI_TOKEN_TYPE_EQUAL);
        default:
            return makeErrorToken_();
    }
}
static ReiToken scanToken_(void)
{
    skipWhitespace_();
    if (isAtEnd_())
    {
        lexerState_.start = lexerState_.curr;
        ReiToken token;
        token.type = REI_TOKEN_TYPE_EOF;
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
