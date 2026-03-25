#pragma once
#include <cstdint>

namespace vvmidi
{
    namespace Tick
    {
        using Val = uint32_t;
        static constexpr Val BPM = 120; // 拍/分钟
        static constexpr Val TPQN = 480;// tick/四分音符
        enum Const : Val
        {
            WHOLE           = TPQN * 4, // 全音符
            HALF            = TPQN * 2, // 二分音符
            QUARTER         = TPQN,     // 四分音符
            QUAVER          = TPQN / 2, // 八分音符
            SIXTEENTH       = TPQN / 4, // 十六分音符
            THIRTY_SECOND   = TPQN / 8, // 三十二分音符
            Default         = QUARTER   // 默认（四分音符）
        };
    };
}