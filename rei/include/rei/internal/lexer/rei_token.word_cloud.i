#ifndef REI_INTERNAL_LEXER_TOKEN_WORD_CLOUD_I
#define REI_INTERNAL_LEXER_TOKEN_WORD_CLOUD_I

#include "rei/internal/lexer/rei_token.h"

static const char* wordCloud_[REI_TOKEN_KIND_MAX_COUNT] =
{
    "(", ")", "[", "]", "{", "}",
    "[[", "]]",
    ",", "?", ".", "..","...", ":", "::", ";",
    "+", "-", "*", "/", "%", "**",
    "&", "|", "^", "~", "<<", ">>",
    "&&", "||", "!",
    "<", ">", "==", "!=", "<=", ">=", "<=>",
    ":=", "=",
    "+=", "-=", "*=", "/=", "%=", "**=",
    "&=", "|=", "^=", "~=", "<<=", ">>=",
    "<-", "->",
    "#", "@",
#if REI_USE_UPPERCASE
    "NIL", "TRUE", "FALSE",
    "$INT", "$FLOAT", "$STRING",
    "$IDENTIFIER",
    "MUT", "KON",
    "IF", "ELIF", "ELSE",
    "WHILE", "UNTIL", "CONTINUE", "BREAK", "DO",
    "SWITCH", "CASE", "PASS", "DEFAULT",
    "FUNC", "RETURN",
    "NAMESPACE", "EXTERNAL", "INTERNAL",
    "ENUM", "FLAG",
    "STRUCT", "CLASS", "INTERFACE", "TRAIT", "ATTRIBUTE",
    "STATIC",
    "PUBLIC", "PROTECTED", "PRIVATE",
    "THIS", "SUPER",
    "ABSTRACT", "SEALED", "FINAL",
    "VIRTUAL", "OVERRIDE",
    "_GET_", "_SET_",
    "NEW", "DEL",
    "IS", "HAS",
    "IMPORT", "AS",
    "RENAME",
    "__MAIN__",
    "__INIT__", "__FREE__", "__OPER__",
    "#EOF"
#else
    "nil", "true", "false",
    "$int", "$float", "$string",
    "$identifier",
    "mut", "kon",
    "if", "elif", "else",
    "while", "until", "continue", "break", "do",
    "switch", "case", "pass", "default",
    "func", "return",
    "namespace", "external", "internal",
    "enum", "flag",
    "struct", "class", "interface", "trait", "attribute",
    "static",
    "public", "protected", "private",
    "this", "super",
    "abstract", "sealed", "final",
    "virtual", "override",
    "_get_", "_set_",
    "new", "del",
    "is", "has",
    "import", "as",
    "rename",
    "__main__",
    "__init__", "__free__", "__oper__",
    "#eof"
#endif
};

#endif
