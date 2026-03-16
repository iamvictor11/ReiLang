#include "token.hpp"
#include <array>
#include <algorithm>

namespace vic::Token
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

                {"do", TK_LBRACE},
                {"then", TK_LBRACE},
                {"beg", TK_LBRACE},
                {"end", TK_RBRACE}
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
    #define VIC_TOKEN_TYPE_LIST \
        /* 注释 */ \
        VIC_TOKEN_X(NOTE, "//") \
        /* 基础 */ \
        VIC_TOKEN_X(ASSIGN, "=") \
        VIC_TOKEN_X(WALRUS, ":=") \
        /* 标识符 */ \
        VIC_TOKEN_X(IDENT, "<ident>") \
        /* 字面量 */ \
        VIC_TOKEN_X(LIT_INT, "<int>") \
        VIC_TOKEN_X(LIT_FLOAT, "<float>") \
        VIC_TOKEN_X(LIT_STRING, "<string>") \
        /* 数学运算 */ \
        VIC_TOKEN_X(ADD, "+") \
        VIC_TOKEN_X(SUB, "-") \
        VIC_TOKEN_X(MUL, "*") \
        VIC_TOKEN_X(DIV, "/") \
        VIC_TOKEN_X(MOD, "%") \
        VIC_TOKEN_X(POW, "**") \
        VIC_TOKEN_X(SELF_ADD, "+=") \
        VIC_TOKEN_X(SELF_SUB, "-=") \
        VIC_TOKEN_X(SELF_MUL, "*=") \
        VIC_TOKEN_X(SELF_DIV, "/=") \
        VIC_TOKEN_X(SELF_MOD, "%=") \
        VIC_TOKEN_X(SELF_POW, "**=") \
        /* 位运算 */ \
        VIC_TOKEN_X(BIT_AND, "&") \
        VIC_TOKEN_X(BIT_OR, "|") \
        VIC_TOKEN_X(BIT_XOR, "^") \
        VIC_TOKEN_X(BIT_XNOR, "`") \
        VIC_TOKEN_X(BIT_NOT, "~") \
        VIC_TOKEN_X(BIT_SHL, "<<") \
        VIC_TOKEN_X(BIT_SHR, ">>") \
        VIC_TOKEN_X(SELF_BIT_AND, "&=") \
        VIC_TOKEN_X(SELF_BIT_OR, "|=") \
        VIC_TOKEN_X(SELF_BIT_XOR, "^=") \
        VIC_TOKEN_X(SELF_BIT_XNOR, "`=") \
        VIC_TOKEN_X(SELF_BIT_NOT, "~=") \
        VIC_TOKEN_X(SELF_BIT_SHL, "<<=") \
        VIC_TOKEN_X(SELF_BIT_SHR, ">>=") \
        /* 比较运算 */ \
        VIC_TOKEN_X(EQ, "==") \
        VIC_TOKEN_X(NE, "!=") \
        VIC_TOKEN_X(LT, "<") \
        VIC_TOKEN_X(LE, "<=") \
        VIC_TOKEN_X(GT, ">") \
        VIC_TOKEN_X(GE, ">=") \
        /* 逻辑运算 */ \
        VIC_TOKEN_X(AND, "&&") \
        VIC_TOKEN_X(OR, "||") \
        VIC_TOKEN_X(NOT, "!") \
        /* 保留词 */ \
        VIC_TOKEN_X(NIL, "nil") \
        VIC_TOKEN_X(TRUE, "true") \
        VIC_TOKEN_X(FALSE, "false") \
        VIC_TOKEN_X(DEF, "def") \
        VIC_TOKEN_X(VAR, "var") \
        VIC_TOKEN_X(LET, "let") \
        VIC_TOKEN_X(REF, "ref") \
        VIC_TOKEN_X(TYPE_INT, "int") \
        VIC_TOKEN_X(TYPE_FLOAT, "float") \
        VIC_TOKEN_X(TYPE_STRING, "string") \
        VIC_TOKEN_X(PACKAGE, "package") \
        VIC_TOKEN_X(IMPORT, "import") \
        VIC_TOKEN_X(IF, "if") \
        VIC_TOKEN_X(ELIF, "elif") \
        VIC_TOKEN_X(ELSE, "else") \
        VIC_TOKEN_X(LOOP, "loop") \
        VIC_TOKEN_X(CONTINUE, "continue") \
        VIC_TOKEN_X(BREAK, "break") \
        VIC_TOKEN_X(FUNC, "func") \
        VIC_TOKEN_X(RETURN, "return") \
        VIC_TOKEN_X(STRUCT, "struct") \
        VIC_TOKEN_X(NAMESPACE, "namespace") \
        VIC_TOKEN_X(GLOBAL, "global") \
        VIC_TOKEN_X(LOCAL, "local") \
        VIC_TOKEN_X(INTERFACE, "interface") \
        VIC_TOKEN_X(CLASS, "class") \
        VIC_TOKEN_X(THIS, "this") \
        VIC_TOKEN_X(SUPER, "super") \
        VIC_TOKEN_X(STATIC, "static") \
        VIC_TOKEN_X(PUBLIC, "public") \
        VIC_TOKEN_X(PROTECTED, "protected") \
        VIC_TOKEN_X(PRIVATE, "private") \
        VIC_TOKEN_X(VIRTUAL, "virual") \
        VIC_TOKEN_X(OVERRIDE, "override") \
        VIC_TOKEN_X(IS, "is")\
        VIC_TOKEN_X(ENTITY, "entity") \
        VIC_TOKEN_X(COMPONENT, "component") \
        VIC_TOKEN_X(HAS, "has") \
        VIC_TOKEN_X(NEW, "new") \
        VIC_TOKEN_X(DEL, "del") \
        VIC_TOKEN_X(PRINT, "print") \
        VIC_TOKEN_X(PRINTLN, "println") \
        /* 区域 */ \
        VIC_TOKEN_X(LPAREN, "(") \
        VIC_TOKEN_X(RPAREN, ")") \
        VIC_TOKEN_X(LBRACKET, "[") \
        VIC_TOKEN_X(RBRACKET, "]") \
        VIC_TOKEN_X(LBRACE, "{") \
        VIC_TOKEN_X(RBRACE, "}") \
        /* 其他 */ \
        VIC_TOKEN_X(DOT, ".") \
        VIC_TOKEN_X(COMMA, ",") \
        VIC_TOKEN_X(SEMICOLON, ";") \
        VIC_TOKEN_X(COLON, ":") \
        VIC_TOKEN_X(DCOLON, "::") \
        VIC_TOKEN_X(RARROW, "->") \
        VIC_TOKEN_X(LARROW, "<-") \
        /* 结束 */ \
        VIC_TOKEN_X(EOF, "<eof>")
    static std::string _toSymbol(Type type)
    {
        switch (type)
        {
            #define VIC_TOKEN_X(name, symbol) case TK_##name: return symbol;
            VIC_TOKEN_TYPE_LIST
            #undef VIC_TOKEN_X
            default:
                return "<?>";
        }
    }
    static std::string _toString(Type type)
    {
        switch (type)
        {
            #define VIC_TOKEN_X(name, symbol) case TK_##name: return #name;
            VIC_TOKEN_TYPE_LIST
            #undef VIC_TOKEN_X
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
