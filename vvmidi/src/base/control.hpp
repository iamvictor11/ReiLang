#pragma once
#include <cstdint>

namespace vvmidi
{
    namespace Control
    {
        enum Enum : uint8_t
        {
            BankSelectMSB       = 0,// 银行选择 高位
            ModulationWheel     = 1,// 调制轮（颤音）
            BreathControl       = 2,// 吹奏强度（管乐）
            Undefined3          = 3,// 未定义（下同）
            FootController      = 4,// 脚踏控制
            PortamentoTime      = 5,// 滑音时间
            DataEntryMSB        = 6,// 数据输入 高位（RPN/NRPN）
            Volume              = 7,// 通道音量
            Balance             = 8,// 平衡（左右）
            Undefined9          = 9,
            Pan                 = 10,// 声像/左右声道
            Expression          = 11,// 表情（次级音量）
            EffectControl1      = 12,// 效果控制 1
            EffectControl2      = 13,// 效果控制 2
            Undefined14         = 14,
            Undefined15         = 15,
            GeneralPurpose1     = 16,// 通用控制 1
            GeneralPurpose2     = 17,// 通用控制 2
            GeneralPurpose3     = 18,// 通用控制 3
            GeneralPurpose4     = 19,// 通用控制 4
            Undefined20         = 20,
            Undefined21         = 21,
            Undefined22         = 22,
            Undefined23         = 23,
            Undefined24         = 24,
            Undefined25         = 25,
            Undefined26         = 26,
            Undefined27         = 27,
            Undefined28         = 28,
            Undefined29         = 29,
            Undefined30         = 30,
            Undefined31         = 31,
            BankSelectLSB       = 32,// 银行选择 低位
            ModulationWheelLSB  = 33,// 调制轮 低位
            BreathControlLSB    = 34,// 吹奏强度 低位（管乐演奏控制）
            Undefined35         = 35,
            FootControllerLSB   = 36,// 脚踏控制 低位
            PortamentoTimeLSB   = 37,// 滑音时间 低位
            DataEntryLSB        = 38,// 数据输入 低位
            VolumeLSB           = 39,// 音量 低位
            BalanceLSB          = 40,// 左右声道平衡 低位
            Undefined41         = 41,
            PanLSB              = 42,// 声像位置 低位
            ExpressionLSB       = 43,// 表情 低位
            EffectControl1LSB   = 44,// 效果控制器 1 低位（混响）
            EffectControl2LSB   = 45,// 效果控制器 2 低位（颤音）
            Undefined46         = 46,
            Undefined47         = 47,
            GeneralPurpose1LSB  = 48,// 通用控制器 1 低位
            GeneralPurpose2LSB  = 49,// 通用控制器 2 低位
            GeneralPurpose3LSB  = 50,// 通用控制器 3 低位
            GeneralPurpose4LSB  = 51,// 通用控制器 4 低位
            Undefined52         = 52,
            Undefined53         = 53,
            Undefined54         = 54,
            Undefined55         = 55,
            Undefined56         = 56,
            Undefined57         = 57,
            Undefined58         = 58,
            Undefined59         = 59,
            Undefined60         = 60,
            Undefined61         = 61,
            Undefined62         = 62,
            Undefined63         = 63,
            SustainPedal        = 64,// 延音踏板（>=64 开）
            PortamentoSwitch    = 65,// 滑音开关
            Sostenuto           = 66,// 保持踏板
            SoftPedal           = 67,// 软踏板
            LegatoSwitch        = 68,// 连奏开关
            Hold2               = 69,// 第二延音
            SoundVariation      = 70,// 声音变化
            Resonance           = 71,// 滤波器共振
            SoundReleaseTime    = 72,// 音符释放时间
            SoundAttackTime     = 73,// 音符起音时间
            Brightness          = 74,// 滤波器截止频率
            SoundControl6       = 75,// 合成器控制 6
            SoundControl7       = 76,// 合成器控制 7
            SoundControl8       = 77,// 合成器控制 8
            SoundControl9       = 78,// 合成器控制 9
            SoundControl10      = 79,// 合成器控制 10
            GeneralPurpose5     = 80,// 通用控制 5
            GeneralPurpose6     = 81,// 通用控制 6
            GeneralPurpose7     = 82,// 通用控制 7
            GeneralPurpose8     = 83,// 通用控制 8
            PortamentoControl   = 84,// 滑音控制
            Undefined85         = 85,
            Undefined86         = 86,
            Undefined87         = 87,
            Undefined88         = 88,
            Undefined89         = 89,
            Undefined90         = 90,
            ReverbLevel         = 91,// 混响深度
            TremoloLevel        = 92,// 颤音深度
            ChorusLevel         = 93,// 合唱效果深度
            DetuneDepth         = 94,// 颤音/轻音深度
            PhaserLevel         = 95,// 移相器深度
            DataIncrement       = 96,// 数据+1
            DataDecrement       = 97,// 数据–1
            NRPN_LSB            = 98,// NRPN 参数 低位
            NRPN_MSB            = 99,// NRPN 参数 高位
            RPN_LSB             = 100,// RPN 参数 低位
            RPN_MSB             = 101,// RPN 参数 高位
            Undefined102        = 102,
            Undefined103        = 103,
            Undefined104        = 104,
            Undefined105        = 105,
            Undefined106        = 106,
            Undefined107        = 107,
            Undefined108        = 108,
            Undefined109        = 109,
            Undefined110        = 110,
            Undefined111        = 111,
            Undefined112        = 112,
            Undefined113        = 113,
            Undefined114        = 114,
            Undefined115        = 115,
            Undefined116        = 116,
            Undefined117        = 117,
            Undefined118        = 118,
            Undefined119        = 119,

            AllSoundOff         = 120,// 所有声音立即关闭
            ResetAllControllers = 121,// 重置控制器
            LocalControl        = 122,// 是否本地键盘控制合成器
            AllNotesOff         = 123,// 关闭所有音符
            OmniModeOff         = 124,// 多通道模式关闭
            OmniModeOn          = 125,// 全通道接收
            MonoModeOn          = 126,// 单音模式
            PolyModeOn          = 127// 多音模式
        };
    }
}
