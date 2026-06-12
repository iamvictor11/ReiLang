#pragma once
#include "../type/value.hxx"
#include <string>
#include <string_view>

namespace rei
{
    struct Location final
    {
    public:
        uint64_t line = 1, column = 1;
    public:
        std::string toString() const { return "[" + std::to_string(line) + ":" + std::to_string(column) + "]"; }
    };

    /*
    ( [ {
    ) ] }
    , ? .   :  ;
        ... ::
    + - * / %
    &  |  ~
    && || !
    <  >
    << >>
    =  != <= >= :=
    ==
    += -= *= /= %=
    &= |= ~=
    <<= >>=
    <- ->
    # @
    */
    enum class TokenSymbol : uint8_t
    {
        Nil,
        True, False,
        Int, Float,
        String,

        LeftParen,  LeftBracket,    LeftBrace,
        RightParen, RightBracket,   RightBrace,
        Comma, Question,    Dot,        Colon,      Semicolon,
                            DotDotDot,  ColonColon,
        Plus, Minus, Star, Slash, Modulo,
        And,    Pipe,       Wave,
        AndAnd, PipePipe,   Bang,
        Less,       Greater,
        LessLess,   GreaterGreater,
        Equal,      BangEqual, LessEqual, GreaterEqual, ColonEqual,
        EqualEqual,
        PlusEqual, MinusEqual, StarEqual, SlashEqual, ModuloEqual,
        AndEqual, PipeEqual, WaveEqual,
        LessLessEqual, GreaterGreaterEqual,
        LeftArrow, RightArrow,
        Pound, At,

        Const, Static, Runtime,

        If, Elif, Else,
        While, Until, Continue, Break,
        Do,
        Switch, Case, Pass, Default,
        Func, Return,
        Namespace, External, Internal,

        Enum, Flag,
        Struct,
        Class, Own, Our,
        Public, Protected, Private,
        Super, This,
        Is,
        Abstract, Sealed,
        Virtual, Override,
        New, Del,
        Interface, Trait, Has,
        Generics, Where,
        Attribute,

        Include,
        Import, Use, As,

        Eof
    };
    std::string toString(TokenSymbol symbol);
    struct Token final
    {
    public:
        TokenSymbol symbol;
        std::string_view lexeme;
        Value literal;
        Location location;
    public:
        std::string toString() const;
    };
    using TokenArray = std::vector<Token>;
}
