#pragma once
#include <cstdint>

namespace vvmidi
{
    namespace Channel
    {
        enum Index : uint8_t
        {
            Zero=0, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Eleven, Twelve, Thirteen, Fourteen, Fifteen,
            Count
        };
    }
    namespace Voice
    {
        enum Enum : uint8_t
        {
            NoteOff                 = 0x80,// 松开音符
            NoteOn                  = 0x90,// 按下音符
            PolyphonicKeyPressure   = 0xA0,// 键压力
            ControlChange           = 0xB0,// 控制器消息
            ProgramChange           = 0xC0,// 改变乐器音色
            ChannelPressure         = 0xD0,// 通道压力
            PitchBend               = 0xE0,// 音高弯曲
        };
    }
}
