#include "token.h"
#include "common/memory.h"

void lunaTokenArray_init(luna_TokenArray *self)
{
    self->data = NULL;
    self->size = 0;
    self->capacity = 0;
}
void lunaTokenArray_free(luna_TokenArray *self)
{
    LUNA_FREE_ARRAY(luna_Token, self->data);
    lunaTokenArray_init(self);
}
void lunaTokenArray_write(luna_TokenArray *self, luna_Token element)
{
    if (self->size >= self->capacity)
    {
        self->capacity = LUNA_GROW_CAPACITY(self->capacity);
        self->data = LUNA_GROW_ARRAY(luna_Token, self->data, self->capacity);
    }
    self->data[self->size] = element;
    self->size++;
}

typedef struct luna_KeywordEntry
{
    const char *keyword;
    luna_TokenType type;
} luna_KeywordEntry;

static const luna_KeywordEntry _KEYWORDS[] =
{
    {"and", LUNA_TK_AND},
    {"break", LUNA_TK_BREAK},
    {"class", LUNA_TK_CLASS},
    {"component", LUNA_TK_COMPONENT},
    {"continue", LUNA_TK_CONTINUE},
    {"def", LUNA_TK_DEF},
    {"del", LUNA_TK_DEL},
    {"elif", LUNA_TK_ELIF},
    {"else", LUNA_TK_ELSE},
    {"entity", LUNA_TK_ENTITY},
    {"false", LUNA_TK_FALSE},
    {"float", LUNA_TK_TYPE_FLOAT},
    {"func", LUNA_TK_FUNC},
    {"global", LUNA_TK_GLOBAL},
    {"has", LUNA_TK_HAS},
    {"if", LUNA_TK_IF},
    {"import", LUNA_TK_IMPORT},
    {"int", LUNA_TK_TYPE_INT},
    {"interface", LUNA_TK_INTERFACE},
    {"is", LUNA_TK_IS},
    {"let", LUNA_TK_LET},
    {"local", LUNA_TK_LOCAL},
    {"loop", LUNA_TK_LOOP},
    {"namespace", LUNA_TK_NAMESPACE},
    {"new", LUNA_TK_NEW},
    {"nil", LUNA_TK_NIL},
    {"not", LUNA_TK_NOT},
    {"or", LUNA_TK_OR},
    {"override", LUNA_TK_OVERRIDE},
    {"package", LUNA_TK_PACKAGE},
    {"print", LUNA_TK_PRINT},
    {"println", LUNA_TK_PRINTLN},
    {"private", LUNA_TK_PRIVATE},
    {"protected", LUNA_TK_PROTECTED},
    {"public", LUNA_TK_PUBLIC},
    {"ref", LUNA_TK_REF},
    {"return", LUNA_TK_RETURN},
    {"static", LUNA_TK_STATIC},
    {"string", LUNA_TK_TYPE_STRING},
    {"struct", LUNA_TK_STRUCT},
    {"super", LUNA_TK_SUPER},
    {"this", LUNA_TK_THIS},
    {"true", LUNA_TK_TRUE},
    {"var", LUNA_TK_VAR},
    {"virtual", LUNA_TK_VIRTUAL}
};
static const size_t _KEYWORD_COUNT = sizeof(_KEYWORDS) / sizeof(_KEYWORDS[0]);
luna_TokenType lunaTokenType_fromKeyword(const char *lexeme, size_t len)
{
    int left = 0;
    int right = (int)_KEYWORD_COUNT - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        int cmp = strncmp(lexeme, _KEYWORDS[mid].keyword, len);
        
        if (cmp == 0)
        {
            if (strlen(_KEYWORDS[mid].keyword) == len)
                return _KEYWORDS[mid].type;
            if (len < strlen(_KEYWORDS[mid].keyword))
                right = mid - 1;
            else
                left = mid + 1;
        }
        else if (cmp < 0)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    return LUNA_TK_IDENT;
}
luna_Value lunaToken_toLiteral(luna_TokenType type)
{
    luna_Value literal = {LUNA_VT_NIL, 0};
    switch (type)
    {
    case LUNA_TK_NIL:
        break;
    case LUNA_TK_TRUE:
        literal.data.b = true;
        break;
    case LUNA_TK_FALSE:
        literal.data.b = false;
        break;
    default:
        literal.type = LUNA_VT_NIL;
        break;
    }
    return literal;
}
