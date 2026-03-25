#pragma once
#include "msg.hpp"
#include "tick.hpp"

namespace vvmidi
{
    class MidiOut final
    {
    private:
        void* handle_;
        Tick::Val tick_ = Tick::Default;
    public:
        MidiOut();
        ~MidiOut();
    private:
        MidiOut(const MidiOut&) = delete;
        MidiOut(MidiOut&&) = delete;
        MidiOut& operator=(const MidiOut&) = delete;
        MidiOut& operator=(MidiOut&&) = delete;
    public:
        Tick::Val getTick();
        void setTick(Tick::Val val);
    public:
        void send(midi_msg message);
    public:
        // 控制音符按下/松开
        void playNote(Note::Val note, Channel::Index channel, bool toggle, uint8_t velocity = 0x7F);
        // 改变通道的乐器
        void programChange(Channel::Index channel, Instrument::Type instrument);
        // 对整个通道施加压力
        void applyAftertouch(Channel::Index channel, uint8_t pressure);
        // 对一个音符施加压力
        void applyPolyAftertouch(Channel::Index channel, Note::Val note, uint8_t pressure);
        // 改变通道的弯音
        void pitchBend(Channel::Index channel, uint16_t value);
        // 控制器消息
        void controlChange(Control::Enum control, Channel::Index channel, uint8_t volume);
    public:
        // 音量控制
        void volumeControl(Channel::Index channel, uint8_t volume);
        // 声像控制
        void panControl(Channel::Index channel, uint8_t pan);
        // 踏板控制
        void sustainControl(Channel::Index channel, bool down);
        // 松开通道上所有的音符
        void allNotesOffMsg(Channel::Index channel, bool immediately = false);
    };
}
