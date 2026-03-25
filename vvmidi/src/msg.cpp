#include "msg.hpp"

namespace vvmidi
{
    namespace internal
    {
        static uint8_t clamp7(uint8_t x)
        {
            if (x > 127) return 127;
            return x;
        }
        static uint16_t clamp14(uint16_t x)
        {
            if (x > 16383) return 16383;
            return x;
        }

        midi_msg makeMsg(uint8_t status, uint8_t data1, uint8_t data2)
        {
            return ((uint32_t)status)
                | (((uint32_t)data1) << 8)
                | (((uint32_t)data2) << 16);
        }
        void divi_msg(midi_msg msg, uint8_t& status, uint8_t& data1, uint8_t data2)
        {
            status = (msg >> 0) & 0xFF;
            data1 = (msg >> 8) & 0xFF;
            data2 = (msg >> 16) & 0xFF;
        }
    #pragma region Voice
        midi_msg makeVoiceMsg(Voice::Enum voice, Channel::Index channel, uint8_t lsb, uint8_t msb)
        {
            return makeMsg(voice | (uint8_t)channel, clamp7((uint8_t)lsb), clamp7((uint8_t)msb));
        }
        midi_msg makePlayNoteMsg(Note::Val note, Channel::Index channel, bool toggle, uint8_t velocity)
        {
            return makeVoiceMsg(toggle ? Voice::NoteOn : Voice::NoteOff, channel, note, velocity);
        }
        midi_msg makeProgramChangeMsg(Channel::Index channel, Instrument::Type instrument)
        {
            return makeVoiceMsg(Voice::ProgramChange, channel, instrument, 0);
        }
        midi_msg makeApplyAftertouchMsg(Channel::Index channel, uint8_t pressure)
        {
            return makeVoiceMsg(Voice::ChannelPressure, channel, pressure, 0);
        }
        midi_msg makeApplyPolyAftertouchMsg(Channel::Index channel, Note::Val note, uint8_t pressure)
        {
            return makeVoiceMsg(Voice::PolyphonicKeyPressure, channel, note, pressure);
        }
        midi_msg makePitchBendMsg(Channel::Index channel, uint16_t value)
        {
            uint8_t lsb = value & 0x7F;
            uint8_t msb = (value >> 7) & 0x7F;
            return makeVoiceMsg(Voice::PitchBend, channel, lsb, msb);
        }
        midi_msg makeControlChangeMsg(Control::Enum control, Channel::Index channel, uint8_t value)
        {
            return makeVoiceMsg(Voice::ControlChange, channel, control, value);
        }
    #pragma endregion
    #pragma region CC
        midi_msg makeVolumeControlMsg(Channel::Index channel, uint8_t volume)
        {
            return makeControlChangeMsg(Control::Volume, channel, volume);
        }
        midi_msg makePanControlMsg(Channel::Index channel, uint8_t pan)
        {
            return makeControlChangeMsg(Control::Pan, channel, pan);
        }
        midi_msg makeSustainControlMsg(Channel::Index channel, bool down)
        {
            return makeControlChangeMsg(Control::SustainPedal, channel, down ? 127 : 0);
        }
        midi_msg makeAllNotesOffMsg(Channel::Index channel, bool immediately)
        {
            return makeControlChangeMsg(immediately ? Control::AllSoundOff : Control::AllNotesOff, channel, 0);
        }
    #pragma endregion
    }
}
