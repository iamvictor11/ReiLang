#include "token.hpp"
#include <array>
#include <algorithm>

namespace rei::Token
{
    Type toTypeFromKeyword(std::string_view s)
    {
        static const auto keywords = []()
        {
            auto arr = std::to_array<std::pair<std::string_view, Type>>
            ({
                {"and", TK_AND},
                {"or", TK_OR},
                {"not", TK_NOT},

                {"nil", TK_NIL},
                {"true", TK_TRUE},
                {"false", TK_FALSE},
                {"def", TK_DEF},
                {"var", TK_VAR},
                {"let", TK_LET},
                {"ref", TK_REF},
                {"int", TK_TYPE_INT},
                {"float", TK_TYPE_FLOAT},
                {"string", TK_TYPE_STRING},
                {"package", TK_PACKAGE},
                {"import", TK_IMPORT},
                {"if", TK_IF},
                {"elif", TK_ELIF},
                {"else", TK_ELSE},
                {"loop", TK_LOOP},
                {"until", TK_UNTIL},
                {"continue", TK_CONTINUE},
                {"break", TK_BREAK},
                {"func", TK_FUNC},
                {"return", TK_RETURN},
                {"struct", TK_STRUCT},
                {"namespace", TK_NAMESPACE},
                {"global", TK_GLOBAL},
                {"local", TK_LOCAL},
                {"interface", TK_INTERFACE},
                {"class", TK_CLASS},
                {"this", TK_THIS},
                {"super", TK_SUPER},
                {"static", TK_STATIC},
                {"public", TK_PUBLIC},
                {"protected", TK_PROTECTED},
                {"private", TK_PRIVATE},
                {"virtual", TK_VIRTUAL},
                {"override", TK_OVERRIDE},
                {"is", TK_IS},
                {"entity", TK_ENTITY},
                {"component", TK_COMPONENT},
                {"has", TK_HAS},
                {"new", TK_NEW},
                {"del", TK_DEL},
                {"print", TK_PRINT},
                {"println", TK_PRINTLN},

                {"when", TK_IF},
                {"while", TK_LOOP},
                {"echo", TK_PRINTLN},

                {"do", TK_LBRACE},
                {"beg", TK_LBRACE},
                {"then", TK_LBRACE},
                {"end", TK_RBRACE},
                {"done", TK_RBRACE},

                {"pass", TK_SEMICOLON}
            });
            std::ranges::sort(arr, std::less<>{}, &std::pair<std::string_view, Type>::first);
            return arr;
        }();
        auto it = std::lower_bound(
            std::begin(keywords), std::end(keywords), s,
            [](const auto& pair, std::string_view val)
            {
                return pair.first < val;
            }
        );
        if (it != std::end(keywords) && it->first == s)
            return it->second;
        return TK_IDENT;
    }
    Value::Data toLiteralFromKeyword(Type t)
    {
        switch (t)
        {
        case Type::TK_NIL:
            return Nil{};
        case Type::TK_TRUE:
            return true;
        case Type::TK_FALSE:
            return false;
        default:
            return Nil{};
        }
    }
    #define REI_TOKEN_TYPE_LIST \
        /* 注释 */ \
        REI_TOKEN_X(NOTE, "//") \
        /* 基础 */ \
        REI_TOKEN_X(ASSIGN, "=") \
        REI_TOKEN_X(WALRUS, ":=") \
        /* 标识符 */ \
        REI_TOKEN_X(IDENT, "<ident>") \
        /* 字面量 */ \
        REI_TOKEN_X(LIT_INT, "<int>") \
        REI_TOKEN_X(LIT_FLOAT, "<float>") \
        REI_TOKEN_X(LIT_STRING, "<string>") \
        /* 数学运算 */ \
        REI_TOKEN_X(ADD, "+") \
        REI_TOKEN_X(SUB, "-") \
        REI_TOKEN_X(MUL, "*") \
        REI_TOKEN_X(DIV, "/") \
        REI_TOKEN_X(MOD, "%") \
        REI_TOKEN_X(POW, "**") \
        REI_TOKEN_X(SELF_ADD, "+=") \
        REI_TOKEN_X(SELF_SUB, "-=") \
        REI_TOKEN_X(SELF_MUL, "*=") \
        REI_TOKEN_X(SELF_DIV, "/=") \
        REI_TOKEN_X(SELF_MOD, "%=") \
        REI_TOKEN_X(SELF_POW, "**=") \
        /* 位运算 */ \
        REI_TOKEN_X(BIT_AND, "&") \
        REI_TOKEN_X(BIT_OR, "|") \
        REI_TOKEN_X(BIT_XOR, "^") \
        REI_TOKEN_X(BIT_XNOR, "`") \
        REI_TOKEN_X(BIT_NOT, "~") \
        REI_TOKEN_X(BIT_SHL, "<<") \
        REI_TOKEN_X(BIT_SHR, ">>") \
        REI_TOKEN_X(SELF_BIT_AND, "&=") \
        REI_TOKEN_X(SELF_BIT_OR, "|=") \
        REI_TOKEN_X(SELF_BIT_XOR, "^=") \
        REI_TOKEN_X(SELF_BIT_XNOR, "`=") \
        REI_TOKEN_X(SELF_BIT_NOT, "~=") \
        REI_TOKEN_X(SELF_BIT_SHL, "<<=") \
        REI_TOKEN_X(SELF_BIT_SHR, ">>=") \
        /* 比较运算 */ \
        REI_TOKEN_X(EQ, "==") \
        REI_TOKEN_X(NE, "!=") \
        REI_TOKEN_X(LT, "<") \
        REI_TOKEN_X(LE, "<=") \
        REI_TOKEN_X(GT, ">") \
        REI_TOKEN_X(GE, ">=") \
        /* 逻辑运算 */ \
        REI_TOKEN_X(AND, "&&") \
        REI_TOKEN_X(OR, "||") \
        REI_TOKEN_X(NOT, "!") \
        /* 保留词 */ \
        REI_TOKEN_X(NIL, "nil") \
        REI_TOKEN_X(TRUE, "true") \
        REI_TOKEN_X(FALSE, "false") \
        REI_TOKEN_X(DEF, "def") \
        REI_TOKEN_X(VAR, "var") \
        REI_TOKEN_X(LET, "let") \
        REI_TOKEN_X(REF, "ref") \
        REI_TOKEN_X(TYPE_INT, "int") \
        REI_TOKEN_X(TYPE_FLOAT, "float") \
        REI_TOKEN_X(TYPE_STRING, "string") \
        REI_TOKEN_X(PACKAGE, "package") \
        REI_TOKEN_X(IMPORT, "import") \
        REI_TOKEN_X(IF, "if") \
        REI_TOKEN_X(ELIF, "elif") \
        REI_TOKEN_X(ELSE, "else") \
        REI_TOKEN_X(LOOP, "loop") \
        REI_TOKEN_X(UNTIL, "until") \
        REI_TOKEN_X(CONTINUE, "continue") \
        REI_TOKEN_X(BREAK, "break") \
        REI_TOKEN_X(FUNC, "func") \
        REI_TOKEN_X(RETURN, "return") \
        REI_TOKEN_X(STRUCT, "struct") \
        REI_TOKEN_X(NAMESPACE, "namespace") \
        REI_TOKEN_X(GLOBAL, "global") \
        REI_TOKEN_X(LOCAL, "local") \
        REI_TOKEN_X(INTERFACE, "interface") \
        REI_TOKEN_X(CLASS, "class") \
        REI_TOKEN_X(THIS, "this") \
        REI_TOKEN_X(SUPER, "super") \
        REI_TOKEN_X(STATIC, "static") \
        REI_TOKEN_X(PUBLIC, "public") \
        REI_TOKEN_X(PROTECTED, "protected") \
        REI_TOKEN_X(PRIVATE, "private") \
        REI_TOKEN_X(VIRTUAL, "virual") \
        REI_TOKEN_X(OVERRIDE, "override") \
        REI_TOKEN_X(IS, "is")\
        REI_TOKEN_X(ENTITY, "entity") \
        REI_TOKEN_X(COMPONENT, "component") \
        REI_TOKEN_X(HAS, "has") \
        REI_TOKEN_X(NEW, "new") \
        REI_TOKEN_X(DEL, "del") \
        REI_TOKEN_X(PRINT, "print") \
        REI_TOKEN_X(PRINTLN, "println") \
        /* 区域 */ \
        REI_TOKEN_X(LPAREN, "(") \
        REI_TOKEN_X(RPAREN, ")") \
        REI_TOKEN_X(LBRACKET, "[") \
        REI_TOKEN_X(RBRACKET, "]") \
        REI_TOKEN_X(LBRACE, "{") \
        REI_TOKEN_X(RBRACE, "}") \
        /* 其他 */ \
        REI_TOKEN_X(DOT, ".") \
        REI_TOKEN_X(COMMA, ",") \
        REI_TOKEN_X(SEMICOLON, ";") \
        REI_TOKEN_X(COLON, ":") \
        REI_TOKEN_X(DCOLON, "::") \
        REI_TOKEN_X(RARROW, "->") \
        REI_TOKEN_X(LARROW, "<-") \
        /* 结束 */ \
        REI_TOKEN_X(EOF, "<eof>")
    static std::string _toSymbol(Type type)
    {
        switch (type)
        {
            #define REI_TOKEN_X(name, symbol) case TK_##name: return symbol;
            REI_TOKEN_TYPE_LIST
            #undef REI_TOKEN_X
            default:
                return "<?>";
        }
    }
    static std::string _toString(Type type)
    {
        switch (type)
        {
            #define REI_TOKEN_X(name, symbol) case TK_##name: return #name;
            REI_TOKEN_TYPE_LIST
            #undef REI_TOKEN_X
            default:
                return "UNKNOWN";
        }
    }
    std::string toString(Type t)
    {
        return _toString(t);
    }
    std::string toSymbol(Type t)
    {
        return _toSymbol(t);
    }
    std::string Unit::toString() const
    {
        std::string res = _toString(type);
        int tabCount = 2 - res.length() / 8;
        for (int i = 0; i < tabCount; i++)
            res += "\t";
        res += "\033[1m\033[36m";
        if (type == TK_LIT_INT || type == TK_LIT_FLOAT || type == TK_LIT_STRING)
            res += Value::getDebugString(literal);
        else if (type == TK_IDENT)
            res += std::string(lexeme);
        else
            res += _toSymbol(type);
        res += "\033[0m";
        return res;
    }
    std::string Unit::toSymbol() const
    {
        return _toSymbol(type);
    }
}
