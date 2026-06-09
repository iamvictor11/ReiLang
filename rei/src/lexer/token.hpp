#pragma once
#include <cstdint>

namespace rei
{
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
        While, Until, Continue, Break, Do,
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

        INCULDE,
        IMPORT, USE, AS,

        Nil,
        True, False,
        Int, Float,
        String,

        EOF
    };
}
