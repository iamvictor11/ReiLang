#pragma once
#include <cstdint>

namespace vvmidi
{
    enum class Letter : uint8_t
    {
        C = 0, D = 2, E = 4, F = 5, G = 7, A = 9, B = 11
    };
    enum class Accidental : int8_t
    {
        DoubleFlat  = -2,
        Flat        = -1,
        Natural     = 0,
        Sharp       = 1,
        DoubleSharp = 2
    };
    struct Tonic final
    {
    public:
        Letter letter = Letter::C;
        Accidental accidental = Accidental::Natural;
    public:
        constexpr uint8_t pitchClass() const noexcept
        {
            uint8_t pc = static_cast<uint8_t>(letter);
            pc += static_cast<uint8_t>(accidental);
            return (pc % 12 + 12) % 12;
        }
    };
    enum class Diatonic : uint8_t
    {
        Major = 0, Ionian = Major,
        Minor = 1, Aeolian = Minor,
        // Dorian, Phrygian, Lydian, Mixolydian, Locrian,
        // HarmonicMinor, MelodicMinor
    };
    struct Key final
    {
    public:
        enum class Signature : int8_t
        {
            Cb = -7, Gb = -6, Db = -5, Ab = -4, Eb = -3, Bb = -2, F = -1,
            C  = 0,
            G  = 1, D  = 2, A  = 3, E  = 4, B  = 5, Fs = 6, Cs = 7
        };
    public:
        Tonic tonic = {Letter::C, Accidental::Natural};
        Diatonic diatonic = Diatonic::Major;
    public:
        constexpr Signature signature() const noexcept
        {
            int8_t s = (tonic.pitchClass() * 7) % 12;
            if (s > 6) s -= 12;
            return static_cast<Signature>(s);
        }
    };
    template<Diatonic D> struct Intervals;
    template<> struct Intervals<Diatonic::Major> { static constexpr uint8_t value[7] = {0, 2, 4, 5, 7, 9, 11}; };
    template<> struct Intervals<Diatonic::Minor> { static constexpr uint8_t value[7] = {0, 2, 3, 5, 7, 8, 10}; };

    constexpr Key C_Major   = {{Letter::C, Accidental::Natural}, Diatonic::Major};
    constexpr Key G_Major   = {{Letter::G, Accidental::Natural}, Diatonic::Major};
    constexpr Key D_Major   = {{Letter::D, Accidental::Natural}, Diatonic::Major};
    constexpr Key A_Major   = {{Letter::A, Accidental::Natural}, Diatonic::Major};
    constexpr Key E_Major   = {{Letter::E, Accidental::Natural}, Diatonic::Major};
    constexpr Key B_Major   = {{Letter::B, Accidental::Natural}, Diatonic::Major};
    constexpr Key Fs_Major  = {{Letter::F, Accidental::Sharp}, Diatonic::Major};
    constexpr Key Cs_Major  = {{Letter::C, Accidental::Sharp}, Diatonic::Major};
    constexpr Key F_Major   = {{Letter::F, Accidental::Natural}, Diatonic::Major};
    constexpr Key Bb_Major  = {{Letter::B, Accidental::Flat}, Diatonic::Major};
    constexpr Key Eb_Major  = {{Letter::E, Accidental::Flat}, Diatonic::Major};
    constexpr Key Ab_Major  = {{Letter::A, Accidental::Flat}, Diatonic::Major};
    constexpr Key Db_Major  = {{Letter::D, Accidental::Flat}, Diatonic::Major};
    constexpr Key Gb_Major  = {{Letter::G, Accidental::Flat}, Diatonic::Major};
    constexpr Key Cb_Major  = {{Letter::C, Accidental::Flat}, Diatonic::Major};
    constexpr Key A_Minor   = {{Letter::A, Accidental::Natural}, Diatonic::Minor};
    constexpr Key E_Minor   = {{Letter::E, Accidental::Natural}, Diatonic::Minor};
    constexpr Key B_Minor   = {{Letter::B, Accidental::Natural}, Diatonic::Minor};
    constexpr Key Fs_Minor  = {{Letter::F, Accidental::Sharp}, Diatonic::Minor};
    constexpr Key Cs_Minor  = {{Letter::C, Accidental::Sharp}, Diatonic::Minor};
    constexpr Key Gs_Minor  = {{Letter::G, Accidental::Sharp}, Diatonic::Minor};
    constexpr Key Ds_Minor  = {{Letter::D, Accidental::Sharp}, Diatonic::Minor};
    constexpr Key As_Minor  = {{Letter::A, Accidental::Sharp},  Diatonic::Minor};
    constexpr Key D_Minor   = {{Letter::D, Accidental::Natural}, Diatonic::Minor};
    constexpr Key G_Minor   = {{Letter::G, Accidental::Natural}, Diatonic::Minor};
    constexpr Key C_Minor   = {{Letter::C, Accidental::Natural}, Diatonic::Minor};
    constexpr Key F_Minor   = {{Letter::F, Accidental::Natural}, Diatonic::Minor};
    constexpr Key Bb_Minor  = {{Letter::B, Accidental::Flat}, Diatonic::Minor};
    constexpr Key Eb_Minor  = {{Letter::E, Accidental::Flat}, Diatonic::Minor};
    constexpr Key Ab_Minor  = {{Letter::A, Accidental::Flat}, Diatonic::Minor};
    constexpr Key Db_Minor  = {{Letter::D, Accidental::Flat}, Diatonic::Minor};
    constexpr Key Gb_Minor  = {{Letter::G, Accidental::Flat}, Diatonic::Minor};
    constexpr Key Cb_Minor  = {{Letter::C, Accidental::Flat}, Diatonic::Minor};
}
