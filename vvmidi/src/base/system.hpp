#pragma once
#include <cstdint>

namespace vvmidi
{
    namespace System
    {
        namespace Common
        {
            enum Enun : uint8_t
            {
                SysExStart              = 0xF0,// 系统独占消息开始
                MTCQuarterFrame         = 0xF1,// 时间码
                SongPositionPointer     = 0xF2,// 歌曲位置指针
                SongSelect              = 0xF3,// 选择歌曲
                UndefinedF4             = 0xF4,// 未定义
                UndefinedF5             = 0xF5,// 未定义
                TuneRequest             = 0xF6,// 调音请求
                EndOfSysEx              = 0xF7,// 系统独占消息结束
            };
        }
        namespace RealTime
        {
            enum Enum : uint8_t
            {
                TimingClock             = 0xF8,// 定时时钟
                UndefinedF9             = 0xF9,// 未定义
                Start                   = 0xFA,// 开始播放
                Continue                = 0xFB,// 继续播放
                Stop                    = 0xFC,// 停止播放
                UndefinedFD             = 0xFD,// 未定义
                ActiveSensing           = 0xFE,// 主动感知
                SystemReset             = 0xFF,// 系统复位
            };
        }
    }
}
