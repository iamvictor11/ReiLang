#pragma once

namespace vic
{
    template <class... Ts>
    struct LambdaOverloaded : Ts...
    {
        using Ts::operator()...;
    };
    template <class... Ts>
    LambdaOverloaded(Ts...) -> LambdaOverloaded<Ts...>;
}