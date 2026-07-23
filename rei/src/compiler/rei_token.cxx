#include "rei/internal/compiler/rei_token.hxx"
#include <array>
#include <algorithm>
#include <functional>

namespace rei
{
auto keywordToTokenKind(std::string_view keyword) -> TokenKind
{
    static const auto keywords = []()
    {
        auto arr = std::to_array<std::pair<std::string_view, TokenKind>>({
            {"void", TK_VOID},
            {"char", TK_CHAR},
            {"short", TK_SHORT},
            {"int", TK_INT},
            {"long", TK_LONG},
            {"float", TK_FLOAT},
            {"double", TK_DOUBLE},
            {"signed", TK_SIGNED},
            {"unsigned", TK_UNSIGNED},
            {"kon", TK_KON},
            {"mut", TK_MUT},
            {"if", TK_IF},
            {"elif", TK_ELIF},
            {"else", TK_ELSE},
            {"for", TK_FOR},
            {"while", TK_WHILE},
            {"do", TK_DO},
            {"continue", TK_CONTINUE},
            {"break", TK_BREAK},
            {"switch", TK_SWITCH},
            {"case", TK_CASE},
            {"default", TK_DEFAULT},
            {"fn", TK_FN},
            {"return", TK_RETURN},
            {"enum", TK_ENUM},
            {"struct", TK_STRUCT},
            {"alignas", TK_ALIGNAS},
            {"typedef", TK_TYPEDEF},
            {"sizeof", TK_SIZEOF},
            {"alignof", TK_ALIGNOF},
            {"offsetof", TK_OFFSETOF},
            {"typeof", TK_TYPEOF}
        });
        std::ranges::sort(arr, std::less<>{}, &std::pair<std::string_view, TokenKind>::first);
        return arr;
    }();
    auto it = std::lower_bound(
        std::begin(keywords), std::end(keywords), keyword,
        [](const auto& pair, std::string_view val)
        { return pair.first < val; }
    );
    if (it != std::end(keywords) && it->first == keyword)
        return it->second;
    return TK_IDENTIFIER;
}
}
