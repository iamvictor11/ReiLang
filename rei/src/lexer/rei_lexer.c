#include "rei_lexer.h"

static const char* wordCloud_[] =
#ifndef REI_NOT_USE_UPPERCASE
{
    "NIL", "TRUE", "FALSE",
    "(", ")", "[", "]", "{", "}",
    "[[", "]]",
    ",", "?", ".", "...", ":", "::", ";",
    "+", "-", "*", "/", "%", "**",
    "&", "|", "~", "<<", ">>",
    "&&", "||", "!",
    "<", ">", "<=>",
    "=", "==", "!=", "<=", ">=",
    ":=", "$=",
    "+=", "-=", "*=", "/=", "%=", "**=",
    "&=", "|=", "~=", "<<=", ">>=",
    "<-", "->",
    "#", "@",
    "CONST", "STATIC",
    "IF", "ELIF", "ELSE",
    "WHILE", "UNTIL", "CONTINUE", "BREAK",
    "DO",
    "SWITCH", "CASE", "PASS", "DEFAULT",
    "FUNC", "RETURN",
    "NAMESPACE", "EXTERNAL", "INTERNAL",
    "ENUM", "FLAG",
    "STRUCT", "CLASS", "INTERFACE", "TRAIT", "ATTRIBUTE",
    "PUBLIC", "PROTECTED", "PRIVATE",
    "SUPER", "THIS",
    "ABSTRACT", "SEALED", "FINAL",
    "VIRTUAL", "OVERRIDE",
    "NEW", "DEL",
    "IS", "HAS",
    "INCLUDE", "IMPORT", "AS",
    "RENAME",
    "__MAIN__",
    "__INIT__", "__FREE__", "__CALL__", "__OPER__",
    "__EOF__"
};
#else
{
    "nil", "true", "false",
    "(", ")", "[", "]", "{", "}",
    "[[", "]]",
    ",", "?", ".", "...", ":", "::", ";",
    "+", "-", "*", "/", "%", "**",
    "&", "|", "~", "<<", ">>",
    "&&", "||", "!",
    "<", ">", "<=>",
    "=", "==", "!=", "<=", ">=",
    ":=", "$=",
    "+=", "-=", "*=", "/=", "%=", "**=",
    "&=", "|=", "~=", "<<=", ">>=",
    "<-", "->",
    "#", "@",
    "const", "static",
    "if", "elif", "else",
    "while", "until", "continue", "break",
    "do",
    "switch", "case", "pass", "default",
    "func", "return",
    "namespace", "external", "internal",
    "enum", "flag",
    "struct", "class", "interface", "trait", "attribute",
    "public", "protected", "private",
    "super", "this",
    "abstract", "sealed", "final",
    "virtual", "override",
    "new", "del",
    "is", "has",
    "import", "as",
    "rename",
    "__main__",
    "__init__", "__free__", "__call__", "__oper__",
    "__eof__"
};
#endif

void reiLexerInit(ReiLexer* me, const char* source)
{
    me->source = source;
    me->tokens = reiTokenBufferCreate(NULL);
}
void reiLexerFree(ReiLexer* me)
{
    reiTokenBufferDestroy(me->tokens);
}
void reiLexerStart(ReiLexer* me)
{
}
