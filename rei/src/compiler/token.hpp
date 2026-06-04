#pragma once

namespace rei
{
    /*
    ( [ {
    ) ] }
    , ? .   :  ;
        ... ::
    + - * / %
    &  |  !
    && ||
    <  >
    << >>
    =  != <= >= :=
    ==
    */
    enum class TokenSymbol
    {
        LeftParen,  LeftBracket,    LeftBrace,
        RightParen, RightBracket,   RightBrace,
        Comma, Question,    Dot,        Colon,      Semicolon,
                            DotDotDot,  ColonColon,
        Plus, Minus, Star, Slash, Modulo,
        And,    Pipe,       Bang,
        AndAnd, PipePipe,
        Less,       Greater,
        LessLess,   GreaterGreater,
        Equal,      BangEqual, LessEqual, GreaterEqual, ColonEqual,
        EqualEqual
    };
}
