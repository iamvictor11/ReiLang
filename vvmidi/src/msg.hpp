#pragma once
#include "base/key.hpp"
#include "base/note.hpp"
#include "base/channel.hpp"
#include "base/control.hpp"
#include "base/instrumentMap.hpp"

namespace vvmidi
{
    typedef uint32_t midi_msg;

    namespace internal
    {
        midi_msg makeMsg(uint8_t status, uint8_t data1, uint8_t data2);
        void divi_msg(midi_msg msg, uint8_t& status, uint8_t& data1, uint8_t data2);
    #pragma region Voice
        // 控制器消息
        midi_msg makeVoiceMsg(Voice::Enum voice, Channel::Index channel, uint8_t lsb, uint8_t msb);
        // 控制音符按下/松开
        midi_msg makePlayNoteMsg(Note::Val note, Channel::Index channel, bool toggle, uint8_t velocity = 0x7F);
        // 改变通道的乐器
        midi_msg makeProgramChangeMsg(Channel::Index channel, Instrument::Type instrument);
        // 对整个通道施加压力
        midi_msg makeApplyAftertouchMsg(Channel::Index channel, uint8_t pressure);
        // 对一个音符施加压力
        midi_msg makeApplyPolyAftertouchMsg(Channel::Index channel, Note::Val note, uint8_t pressure);
        // 改变通道的弯音
        midi_msg makePitchBendMsg(Channel::Index channel, uint16_t value);
        // 控制器消息
        midi_msg makeControlChangeMsg(Control::Enum control, Channel::Index channel, uint8_t value);
    #pragma endregion
    #pragma region CC
        // 音量控制
        midi_msg makeVolumeControlMsg(Channel::Index channel, uint8_t volume);
        // 声像控制
        midi_msg makePanControlMsg(Channel::Index channel, uint8_t pan);
        // 踏板控制
        midi_msg makeSustainControlMsg(Channel::Index channel, bool down);
        // 松开通道上所有的音符
        midi_msg makeAllNotesOffMsg(Channel::Index channel, bool immediately = false);
    #pragma endregion
    }
}
