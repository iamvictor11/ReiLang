#include "token.hpp"
#include <array>
#include <algorithm>

namespace luna::Token
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
                {"println", TK_PRINTLN}
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
    #define LUNA_TOKEN_TYPE_LIST \
        /* 注释 */ \
        LUNA_TOKEN_X(NOTE, "//") \
        /* 基础 */ \
        LUNA_TOKEN_X(ASSIGN, "=") \
        LUNA_TOKEN_X(WALRUS, ":=") \
        /* 标识符 */ \
        LUNA_TOKEN_X(IDENT, "<ident>") \
        /* 字面量 */ \
        LUNA_TOKEN_X(LIT_INT, "<int>") \
        LUNA_TOKEN_X(LIT_FLOAT, "<float>") \
        LUNA_TOKEN_X(LIT_STRING, "<string>") \
        /* 数学运算 */ \
        LUNA_TOKEN_X(ADD, "+") \
        LUNA_TOKEN_X(SUB, "-") \
        LUNA_TOKEN_X(MUL, "*") \
        LUNA_TOKEN_X(DIV, "/") \
        LUNA_TOKEN_X(MOD, "%") \
        LUNA_TOKEN_X(POW, "**") \
        LUNA_TOKEN_X(SELF_ADD, "+=") \
        LUNA_TOKEN_X(SELF_SUB, "-=") \
        LUNA_TOKEN_X(SELF_MUL, "*=") \
        LUNA_TOKEN_X(SELF_DIV, "/=") \
        LUNA_TOKEN_X(SELF_MOD, "%=") \
        LUNA_TOKEN_X(SELF_POW, "**=") \
        /* 位运算 */ \
        LUNA_TOKEN_X(BIT_AND, "&") \
        LUNA_TOKEN_X(BIT_OR, "|") \
        LUNA_TOKEN_X(BIT_XOR, "^") \
        LUNA_TOKEN_X(BIT_XNOR, "`") \
        LUNA_TOKEN_X(BIT_NOT, "~") \
        LUNA_TOKEN_X(BIT_SHL, "<<") \
        LUNA_TOKEN_X(BIT_SHR, ">>") \
        LUNA_TOKEN_X(SELF_BIT_AND, "&=") \
        LUNA_TOKEN_X(SELF_BIT_OR, "|=") \
        LUNA_TOKEN_X(SELF_BIT_XOR, "^=") \
        LUNA_TOKEN_X(SELF_BIT_XNOR, "`=") \
        LUNA_TOKEN_X(SELF_BIT_NOT, "~=") \
        LUNA_TOKEN_X(SELF_BIT_SHL, "<<=") \
        LUNA_TOKEN_X(SELF_BIT_SHR, ">>=") \
        /* 比较运算 */ \
        LUNA_TOKEN_X(EQ, "==") \
        LUNA_TOKEN_X(NE, "!=") \
        LUNA_TOKEN_X(LT, "<") \
        LUNA_TOKEN_X(LE, "<=") \
        LUNA_TOKEN_X(GT, ">") \
        LUNA_TOKEN_X(GE, ">=") \
        /* 逻辑运算 */ \
        LUNA_TOKEN_X(AND, "&&") \
        LUNA_TOKEN_X(OR, "||") \
        LUNA_TOKEN_X(NOT, "!") \
        /* 保留词 */ \
        LUNA_TOKEN_X(NIL, "nil") \
        LUNA_TOKEN_X(TRUE, "true") \
        LUNA_TOKEN_X(FALSE, "false") \
        LUNA_TOKEN_X(DEF, "def") \
        LUNA_TOKEN_X(VAR, "var") \
        LUNA_TOKEN_X(LET, "let") \
        LUNA_TOKEN_X(REF, "ref") \
        LUNA_TOKEN_X(TYPE_INT, "int") \
        LUNA_TOKEN_X(TYPE_FLOAT, "float") \
        LUNA_TOKEN_X(TYPE_STRING, "string") \
        LUNA_TOKEN_X(PACKAGE, "package") \
        LUNA_TOKEN_X(IMPORT, "import") \
        LUNA_TOKEN_X(IF, "if") \
        LUNA_TOKEN_X(ELIF, "elif") \
        LUNA_TOKEN_X(ELSE, "else") \
        LUNA_TOKEN_X(LOOP, "loop") \
        LUNA_TOKEN_X(CONTINUE, "continue") \
        LUNA_TOKEN_X(BREAK, "break") \
        LUNA_TOKEN_X(FUNC, "func") \
        LUNA_TOKEN_X(RETURN, "return") \
        LUNA_TOKEN_X(STRUCT, "struct") \
        LUNA_TOKEN_X(NAMESPACE, "namespace") \
        LUNA_TOKEN_X(GLOBAL, "global") \
        LUNA_TOKEN_X(LOCAL, "local") \
        LUNA_TOKEN_X(INTERFACE, "interface") \
        LUNA_TOKEN_X(CLASS, "class") \
        LUNA_TOKEN_X(THIS, "this") \
        LUNA_TOKEN_X(SUPER, "super") \
        LUNA_TOKEN_X(STATIC, "static") \
        LUNA_TOKEN_X(PUBLIC, "public") \
        LUNA_TOKEN_X(PROTECTED, "protected") \
        LUNA_TOKEN_X(PRIVATE, "private") \
        LUNA_TOKEN_X(VIRTUAL, "virual") \
        LUNA_TOKEN_X(OVERRIDE, "override") \
        LUNA_TOKEN_X(IS, "is")\
        LUNA_TOKEN_X(ENTITY, "entity") \
        LUNA_TOKEN_X(COMPONENT, "component") \
        LUNA_TOKEN_X(HAS, "has") \
        LUNA_TOKEN_X(NEW, "new") \
        LUNA_TOKEN_X(DEL, "del") \
        LUNA_TOKEN_X(PRINT, "print") \
        LUNA_TOKEN_X(PRINTLN, "println") \
        /* 区域 */ \
        LUNA_TOKEN_X(LPAREN, "(") \
        LUNA_TOKEN_X(RPAREN, ")") \
        LUNA_TOKEN_X(LBRACKET, "[") \
        LUNA_TOKEN_X(RBRACKET, "]") \
        LUNA_TOKEN_X(LBRACE, "{") \
        LUNA_TOKEN_X(RBRACE, "}") \
        /* 其他 */ \
        LUNA_TOKEN_X(DOT, ".") \
        LUNA_TOKEN_X(COMMA, ",") \
        LUNA_TOKEN_X(SEMICOLON, ";") \
        LUNA_TOKEN_X(COLON, ":") \
        LUNA_TOKEN_X(DCOLON, "::") \
        LUNA_TOKEN_X(RARROW, "->") \
        LUNA_TOKEN_X(LARROW, "<-") \
        /* 结束 */ \
        LUNA_TOKEN_X(EOF, "<eof>")
    static std::string _toSymbol(Type type)
    {
        switch (type)
        {
            #define LUNA_TOKEN_X(name, symbol) case TK_##name: return symbol;
            LUNA_TOKEN_TYPE_LIST
            #undef LUNA_TOKEN_X
            default:
                return "<?>";
        }
    }
    static std::string _toString(Type type)
    {
        switch (type)
        {
            #define LUNA_TOKEN_X(name, symbol) case TK_##name: return #name;
            LUNA_TOKEN_TYPE_LIST
            #undef LUNA_TOKEN_X
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
