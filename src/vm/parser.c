#include "parser.h"
#include "common/precedence.h"
#include <stdio.h>

// Expr
static void expression(luna_Parser *self);
static void grouping(luna_Parser *self);
static void _lunaParser_precedence(luna_Parser *self, luna_Precedence precedence);
static void unary(luna_Parser *self);
static void binary(luna_Parser *self);
static void number(luna_Parser *self);
// Stmt
// Kan/Move
static bool isAtEnd(luna_Parser *self);
static luna_Token *advance(luna_Parser *self);
static void pass(luna_Parser *self);
static void passMult(luna_Parser *self, size_t count);
static void consume(luna_Parser *self, luna_TokenType type, const char *message);
// Emit
static void emitByte(luna_Parser *self, luna_Byte byte);
#pragma region Prec
typedef void (*luna_ParseFnuc)(luna_Parser *self);
typedef struct luna_ParseRule
{
    luna_ParseFnuc prefix;
    luna_ParseFnuc infix;
    luna_ParseFnuc suffix;
    luna_Precedence precdence;
} luna_ParseRule;
static luna_ParseRule rules[] =
{
    /* 注释 */
    [LUNA_TK_NOTE]  = {NULL, NULL, NULL, LUNA_PREC_NONE},
    /* 基础 */
    [LUNA_TK_ASSIGN]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_WALRUS]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    /* 标识符 */
    [LUNA_TK_IDENT] = {NULL, NULL, NULL, LUNA_PREC_NONE},
    /* 字面量 */
    [LUNA_TK_LIT_INTEGER]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_LIT_NUMBER]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_LIT_STRING]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    /* 数学运算 */
    [LUNA_TK_ADD]   = {NULL, binary, NULL, LUNA_PREC_TERM},
    [LUNA_TK_SUB]   = {unary, binary, NULL, LUNA_PREC_TERM},
    [LUNA_TK_MUL]   = {NULL, binary, NULL, LUNA_PREC_FACTOR},
    [LUNA_TK_DIV]   = {NULL, binary, NULL, LUNA_PREC_FACTOR},
    [LUNA_TK_MOD]   = {NULL, binary, NULL, LUNA_PREC_FACTOR},
    [LUNA_TK_POW]   = {NULL, binary, NULL, LUNA_PREC_POW},
    [LUNA_TK_SELF_ADD]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_SUB]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_MUL]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_DIV]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_MOD]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_POW]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    /* 位运算 */
    [LUNA_TK_BIT_AND]   = {NULL, NULL, NULL, LUNA_PREC_BAND},
    [LUNA_TK_BIT_OR]    = {NULL, NULL, NULL, LUNA_PREC_BOR},
    [LUNA_TK_BIT_XOR]   = {NULL, NULL, NULL, LUNA_PREC_BXOR},
    [LUNA_TK_BIT_XNOR]  = {NULL, NULL, NULL, LUNA_PREC_BXOR},
    [LUNA_TK_BIT_NOT]   = {NULL, NULL, NULL, LUNA_PREC_UNARY},
    [LUNA_TK_BIT_SHL]   = {NULL, NULL, NULL, LUNA_PREC_SHIFT},
    [LUNA_TK_BIT_SHR]   = {NULL, NULL, NULL, LUNA_PREC_SHIFT},
    [LUNA_TK_SELF_BIT_AND]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_BIT_OR]   = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_BIT_XOR]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_BIT_XNOR] = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_BIT_NOT]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_BIT_SHL]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    [LUNA_TK_SELF_BIT_SHR]  = {NULL, NULL, NULL, LUNA_PREC_ASSIGN},
    /* 比较运算 */
    [LUNA_TK_EQ]    = {NULL, NULL, NULL, LUNA_PREC_EQUAL},
    [LUNA_TK_NE]    = {NULL, NULL, NULL, LUNA_PREC_EQUAL},
    [LUNA_TK_LT]    = {NULL, NULL, NULL, LUNA_PREC_COMPAR},
    [LUNA_TK_LE]    = {NULL, NULL, NULL, LUNA_PREC_COMPAR},
    [LUNA_TK_GT]    = {NULL, NULL, NULL, LUNA_PREC_COMPAR},
    [LUNA_TK_GE]    = {NULL, NULL, NULL, LUNA_PREC_COMPAR},
    /* 逻辑运算 */
    [LUNA_TK_AND]   = {NULL, NULL, NULL, LUNA_PREC_LAND},
    [LUNA_TK_OR]    = {NULL, NULL, NULL, LUNA_PREC_LOR},
    [LUNA_TK_NOT]   = {NULL, NULL, NULL, LUNA_PREC_UNARY},
    /* 保留词 */
    [LUNA_TK_NIL]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_TRUE]  = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_FALSE] = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_DEF]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_VAR]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_LET]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_REF]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_TYPE_INT]      = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_TYPE_FLOAT]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_TYPE_STRING]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_PACKAGE]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_IMPORT]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_IF]        = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_ELIF]      = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_ELSE]      = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_LOOP]      = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_CONTINUE]  = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_BREAK]     = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_FUNC]      = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_RETURN]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_STRUCT]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_NAMESPACE] = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_GLOBAL]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_LOCAL]     = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_INTERFACE] = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_CLASS]     = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_THIS]      = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_SUPER]     = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_STATIC]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_PUBLIC]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_PROTECTED] = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_PRIVATE]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_VIRTUAL]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_OVERRIDE]  = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_IS]        = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_ENTITY]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_COMPONENT] = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_HAS]       = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_NEW]       = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_DEL]       = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_PRINT]     = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_PRINTLN]   = {NULL, NULL, NULL, LUNA_PREC_NONE},
    /* 区域 */
    [LUNA_TK_LPAREN]    = {grouping, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_RPAREN]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_LBRACKET]  = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_RBRACKET]  = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_LBRACE]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_RBRACE]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    /* 其他 */
    [LUNA_TK_DOT]       = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_COMMA]     = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_SEMICOLON] = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_COLON]     = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_DCOLON]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_RARROW]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    [LUNA_TK_LARROW]    = {NULL, NULL, NULL, LUNA_PREC_NONE},
    /* 结束 */
    [LUNA_TK_EOF]   = {NULL, NULL, NULL, LUNA_PREC_NONE}
};
#pragma endregion
void lunaParser_start(luna_Parser *self, luna_TokenArray tokens)
{
    self->tokens = tokens;
    self->cursor.prev = NULL;
    self->cursor.curr = tokens.data;
    self->cursor.next = tokens.data + 1;
    lunaChunk_init(&self->chunk);
}
#pragma region Expr
static void expression(luna_Parser *self)
{
    _lunaParser_precedence(self, LUNA_PREC_ASSIGN);
}
static void grouping(luna_Parser *self)
{
    expression(self);
    consume(self, LUNA_TK_RPAREN, "表达式括号未闭合");
}
static void _lunaParser_precedence(luna_Parser *self, luna_Precedence precedence)
{
}
static void unary(luna_Parser *self)
{
    luna_TokenType op = self->cursor.prev->type;
    expression(self);
    switch (op)
    {
    case LUNA_TK_SUB: emitByte(self, LUNA_OP_NEG); break;
    case LUNA_TK_BIT_NOT: emitByte(self, LUNA_OP_BIT_NOT); break;
    case LUNA_TK_NOT: emitByte(self, LUNA_OP_NOT); break;
    default: break;
    }
}
static void binary(luna_Parser *self)
{
    luna_TokenType op = self->cursor.prev->type;
    switch (op)
    {
    case LUNA_TK_ADD: emitByte(self, LUNA_OP_ADD); break;
    case LUNA_TK_SUB: emitByte(self, LUNA_OP_SUB); break;
    case LUNA_TK_MUL: emitByte(self, LUNA_OP_MUL); break;
    case LUNA_TK_DIV: emitByte(self, LUNA_OP_DIV); break;
    case LUNA_TK_MOD: emitByte(self, LUNA_OP_MOD); break;
    case LUNA_TK_POW: emitByte(self, LUNA_OP_POW); break;
    case LUNA_TK_BIT_AND: emitByte(self, LUNA_OP_BIT_AND); break;
    case LUNA_TK_BIT_OR: emitByte(self, LUNA_OP_BIT_OR); break;
    case LUNA_TK_BIT_XOR: emitByte(self, LUNA_OP_BIT_XOR); break;
    case LUNA_TK_BIT_XNOR: emitByte(self, LUNA_OP_BIT_XNOR); break;
    case LUNA_TK_BIT_SHL: emitByte(self, LUNA_OP_BIT_SHL); break;
    case LUNA_TK_BIT_SHR: emitByte(self, LUNA_OP_BIT_SHR); break;
    case LUNA_TK_EQ: emitByte(self, LUNA_OP_EQ); break;
    case LUNA_TK_NE: emitByte(self, LUNA_OP_NE); break;
    case LUNA_TK_LT: emitByte(self, LUNA_OP_LT); break;
    case LUNA_TK_LE: emitByte(self, LUNA_OP_LE); break;
    case LUNA_TK_GT: emitByte(self, LUNA_OP_GT); break;
    case LUNA_TK_GE: emitByte(self, LUNA_OP_GE); break;
    case LUNA_TK_AND: emitByte(self, LUNA_OP_AND); break;
    case LUNA_TK_OR: emitByte(self, LUNA_OP_OR); break;
    default: break;
    }
}
static void number(luna_Parser *self)
{
    luna_Value value = self->cursor.prev->literal;
    emitByte(self, LUNA_OP_CONSTANT);
    size_t i = lunaChunk_writeConstant(&self->chunk, value);
    emitByte(self, i);
}
#pragma endregion
#pragma region Kan/Move
static bool isAtEnd(luna_Parser *self)
{
    return self->cursor.curr->type == LUNA_TK_EOF;
}
static luna_Token *advance(luna_Parser *self)
{
    self->cursor.prev = self->cursor.curr;
    if (isAtEnd(self))
        return self->cursor.curr;
    self->cursor.curr++;
    self->cursor.next++;
    return self->cursor.prev;
}
static void pass(luna_Parser *self)
{
    self->cursor.prev = self->cursor.curr;
    if (isAtEnd(self))
        return;
    self->cursor.curr++;
    self->cursor.next++;
}
static void passMult(luna_Parser *self, size_t count)
{
    for (size_t i = 0; i < count; i++)
        pass(self);
}
static void consume(luna_Parser *self, luna_TokenType type, const char *message)
{
    if (self->cursor.curr->type == type)
    {
        pass(self);
        return;
    }
    // TODO: 错误处理
    fprintf(stderr, message);
}
#pragma endregion
#pragma region Emit
static void emitByte(luna_Parser *self, luna_Byte byte)
{
    if (self->cursor.prev)
        lunaChunk_writeBytecode(&self->chunk, byte, self->cursor.prev->pos);
}
#pragma endregion
