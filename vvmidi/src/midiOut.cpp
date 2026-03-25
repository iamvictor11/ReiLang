#include "midiOut.hpp"
#include <windows.h>

namespace vvmidi
{
    MidiOut::MidiOut()
    {
        MMRESULT result = midiOutOpen((HMIDIOUT*)&handle_, 0, 0, 0, CALLBACK_NULL);
        if (result != MMSYSERR_NOERROR)
            printf("MidiOut 打开失败: %s\n", result);
    }
    MidiOut::~MidiOut()
    {
        midiOutClose((HMIDIOUT)handle_);
    }

    Tick::Val MidiOut::getTick()
    {
        return tick_;
    }
    void MidiOut::setTick(Tick::Val val)
    {
        tick_ = val;
    }

    void MidiOut::send(midi_msg message)
    {
        midiOutShortMsg((HMIDIOUT)handle_, (DWORD)message);
    }
    #pragma region Voice
    void MidiOut::playNote(Note::Val note, Channel::Index channel, bool toggle, uint8_t velocity)
    {
        MidiOut::send(internal::makePlayNoteMsg(note, channel, toggle, velocity));
    }
    void MidiOut::programChange(Channel::Index channel, Instrument::Type instrument)
    {
        MidiOut::send(internal::makeProgramChangeMsg(channel, instrument));
    }
    void MidiOut::applyAftertouch(Channel::Index channel, uint8_t pressure)
    {
        MidiOut::send(internal::makeApplyAftertouchMsg(channel, pressure));
    }
    void MidiOut::applyPolyAftertouch(Channel::Index channel, Note::Val note, uint8_t pressure)
    {
        MidiOut::send(internal::makeApplyPolyAftertouchMsg(channel, note, pressure));
    }
    void MidiOut::pitchBend(Channel::Index channel, uint16_t value)
    {
        MidiOut::send(internal::makePitchBendMsg(channel, value));
    }
    void MidiOut::controlChange(Control::Enum control, Channel::Index channel, uint8_t volume)
    {
        MidiOut::send(internal::makeControlChangeMsg(control, channel, volume));
    }
    #pragma endregion
    #pragma region CC
    void MidiOut::volumeControl(Channel::Index channel, uint8_t volume)
    {
        MidiOut::send(internal::makeVolumeControlMsg(channel, volume));
    }
    void MidiOut::panControl(Channel::Index channel, uint8_t pan)
    {
        MidiOut::send(internal::makePanControlMsg(channel, pan));
    }
    void MidiOut::sustainControl(Channel::Index channel, bool down)
    {
        MidiOut::send(internal::makeSustainControlMsg(channel, down));
    }
    void MidiOut::allNotesOffMsg(Channel::Index channel, bool immediately)
    {
        MidiOut::send(internal::makeAllNotesOffMsg(channel, immediately));
    }
    #pragma endregion
}
