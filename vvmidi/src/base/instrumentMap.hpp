#pragma once
#include <string>
#include <optional>
#include "instrument.hpp"
#include <unordered_map>

namespace vvmidi::Instrument
{
    class Map final
    {
    public:
        static constexpr std::string toString(Type instrument)
        {
            switch (instrument)
            {
                case AcousticGrandPiano:    return "AcousticGrandPiano";
                case BrightAcousticPiano:   return "BrightAcousticPiano";
                case ElectricGrandPiano:    return "ElectricGrandPiano";
                case HonkyTonkPiano:        return "HonkyTonkPiano";
                case ElectricPiano:         return "ElectricPiano";
                case ElectricPianoFM:       return "ElectricPianoFM";
                case Harpsichord:           return "Harpsichord";
                case Clavinet:              return "Clavinet";
                case Celesta:               return "Celesta";
                case Glockenspiel:          return "Glockenspiel";
                case MusicBox:              return "MusicBox";
                case Vibraphone:            return "Vibraphone";
                case Marimba:               return "Marimba";
                case Xylophone:             return "Xylophone";
                case TubularBells:          return "TubularBells";
                case Dulcimer:              return "Dulcimer";
                case DrawbarOrgan:          return "DrawbarOrgan";
                case PercussiveOrgan:       return "PercussiveOrgan";
                case RockOrgan:             return "RockOrgan";
                case ChurchOrgan:           return "ChurchOrgan";
                case ReedOrgan:             return "ReedOrgan";
                case Accordion:             return "Accordion";
                case Harmonica:             return "Harmonica";
                case TangoAccordion:        return "TangoAccordion";
                case AcousticGuitarNylon:   return "AcousticGuitarNylon";
                case AcousticGuitarSteel:   return "AcousticGuitarSteel";
                case ElectricGuitarJazz:    return "ElectricGuitarJazz";
                case ElectricGuitarClean:   return "ElectricGuitarClean";
                case ElectricGuitarMuted:   return "ElectricGuitarMuted";
                case OverdrivenGuitar:      return "OverdrivenGuitar";
                case DistortionGuitar:      return "DistortionGuitar";
                case GuitarHarmonics:       return "GuitarHarmonics";
                case AcousticBass:          return "AcousticBass";
                case ElectricBassFinger:    return "ElectricBassFinger";
                case ElectricBassPick:      return "ElectricBassPick";
                case FretlessBass:          return "FretlessBass";
                case SlapBassSharp:         return "SlapBassSharp";
                case SlapBassSoft:          return "SlapBassSoft";
                case SynthBassSaw:          return "SynthBassSaw";
                case SynthBassSquare:       return "SynthBassSquare";
                case Violin:                return "Violin";
                case Viola:                 return "Viola";
                case Cello:                 return "Cello";
                case Contrabass:            return "Contrabass";
                case TremoloStrings:        return "TremoloStrings";
                case PizzicatoStrings:      return "PizzicatoStrings";
                case OrchestralHarp:        return "OrchestralHarp";
                case Timpani:               return "Timpani";
                case StringEnsembleBright:  return "StringEnsembleBright";
                case StringEnsembleWarm:    return "StringEnsembleWarm";
                case SynthStringsSaw:       return "SynthStringsSaw";
                case SynthStringsWarm:      return "SynthStringsWarm";
                case ChoirAahs:             return "ChoirAahs";
                case VoiceOohs:             return "VoiceOohs";
                case SynthChoir:            return "SynthChoir";
                case OrchestraHit:          return "OrchestraHit";
                case Trumpet:               return "Trumpet";
                case Trombone:              return "Trombone";
                case Tuba:                  return "Tuba";
                case MutedTrumpet:          return "MutedTrumpet";
                case FrenchHorn:            return "FrenchHorn";
                case BrassSection:          return "BrassSection";
                case SynthBrassBright:      return "SynthBrassBright";
                case SynthBrassWarm:        return "SynthBrassWarm";
                case SopranoSax:            return "SopranoSax";
                case AltoSax:               return "AltoSax";
                case TenorSax:              return "TenorSax";
                case BaritoneSax:           return "BaritoneSax";
                case Oboe:                  return "Oboe";
                case EnglishHorn:           return "EnglishHorn";
                case Bassoon:               return "Bassoon";
                case Clarinet:              return "Clarinet";
                case Piccolo:               return "Piccolo";
                case Flute:                 return "Flute";
                case Recorder:              return "Recorder";
                case PanFlute:              return "PanFlute";
                case BlownBottle:           return "BlownBottle";
                case Shakuhachi:            return "Shakuhachi";
                case Whistle:               return "Whistle";
                case Ocarina:               return "Ocarina";
                case SquareLead:            return "SquareLead";
                case SawLead:               return "SawLead";
                case CalliopeLead:          return "CalliopeLead";
                case ChifferLead:           return "ChifferLead";
                case CharangLead:           return "CharangLead";
                case VoiceLead:             return "VoiceLead";
                case FifthsLead:            return "FifthsLead";
                case BassLead:              return "BassLead";
                case NewAgePad:             return "NewAgePad";
                case WarmPad:               return "WarmPad";
                case PolysynthPad:          return "PolysynthPad";
                case ChoirPad:              return "ChoirPad";
                case BowedGlassPad:         return "BowedGlassPad";
                case MetallicPad:           return "MetallicPad";
                case HaloPad:               return "HaloPad";
                case SweepPad:              return "SweepPad";
                case Rain:                  return "Rain";
                case SoundTrack:            return "SoundTrack";
                case Crystal:               return "Crystal";
                case Atmosphere:            return "Atmosphere";
                case Brightness:            return "Brightness";
                case Goblin:                return "Goblin";
                case Echoes:                return "Echoes";
                case SciFi:                 return "SciFi";
                case Sitar:                 return "Sitar";
                case Banjo:                 return "Banjo";
                case Shamisen:              return "Shamisen";
                case Koto:                  return "Koto";
                case Kalimba:               return "Kalimba";
                case Bagpipe:               return "Bagpipe";
                case Fiddle:                return "Fiddle";
                case Shanai:                return "Shanai";
                case TinkleBell:            return "TinkleBell";
                case Agogo:                 return "Agogo";
                case SteelDrums:            return "SteelDrums";
                case Woodblock:             return "Woodblock";
                case TaikoDrum:             return "TaikoDrum";
                case MelodicTom:            return "MelodicTom";
                case SynthDrum:             return "SynthDrum";
                case ReverseCymbal:         return "ReverseCymbal";
                case GuitarFretNoise:       return "GuitarFretNoise";
                case BreathNoise:           return "BreathNoise";
                case Seashore:              return "Seashore";
                case BirdTweet:             return "BirdTweet";
                case TelephoneRing:         return "TelephoneRing";
                case Helicopter:            return "Helicopter";
                case Applause:              return "Applause";
                case Gunshot:               return "Gunshot";
                default:                                return "Unknown";
            }
        }

        static Type fromString(const std::string& name)
        {
            static const std::unordered_map<std::string, Type> fastMap =
            {
                { "AcousticGrandPiano",     AcousticGrandPiano },
                { "BrightAcousticPiano",    BrightAcousticPiano },
                { "ElectricGrandPiano",     ElectricGrandPiano },
                { "HonkyTonkPiano",         HonkyTonkPiano },
                { "ElectricPiano",          ElectricPiano },
                { "ElectricPianoFM",        ElectricPianoFM },
                { "Harpsichord",            Harpsichord },
                { "Clavinet",               Clavinet },
                { "Celesta",                Celesta },
                { "Glockenspiel",           Glockenspiel },
                { "MusicBox",               MusicBox },
                { "Vibraphone",             Vibraphone },
                { "Marimba",                Marimba },
                { "Xylophone",              Xylophone },
                { "TubularBells",           TubularBells },
                { "Dulcimer",               Dulcimer },
                { "DrawbarOrgan",           DrawbarOrgan },
                { "PercussiveOrgan",        PercussiveOrgan },
                { "RockOrgan",              RockOrgan },
                { "ChurchOrgan",            ChurchOrgan },
                { "ReedOrgan",              ReedOrgan },
                { "Accordion",              Accordion },
                { "Harmonica",              Harmonica },
                { "TangoAccordion",         TangoAccordion },
                { "AcousticGuitarNylon",    AcousticGuitarNylon },
                { "AcousticGuitarSteel",    AcousticGuitarSteel },
                { "ElectricGuitarJazz",     ElectricGuitarJazz },
                { "ElectricGuitarClean",    ElectricGuitarClean },
                { "ElectricGuitarMuted",    ElectricGuitarMuted },
                { "OverdrivenGuitar",       OverdrivenGuitar },
                { "DistortionGuitar",       DistortionGuitar },
                { "GuitarHarmonics",        GuitarHarmonics },
                { "AcousticBass",           AcousticBass },
                { "ElectricBassFinger",     ElectricBassFinger },
                { "ElectricBassPick",       ElectricBassPick },
                { "FretlessBass",           FretlessBass },
                { "SlapBassSharp",          SlapBassSharp },
                { "SlapBassSoft",           SlapBassSoft },
                { "SynthBassSaw",           SynthBassSaw },
                { "SynthBassSquare",        SynthBassSquare },
                { "Violin",                 Violin },
                { "Viola",                  Viola },
                { "Cello",                  Cello },
                { "Contrabass",             Contrabass },
                { "TremoloStrings",         TremoloStrings },
                { "PizzicatoStrings",       PizzicatoStrings },
                { "OrchestralHarp",         OrchestralHarp },
                { "Timpani",                Timpani },
                { "StringEnsembleBright",   StringEnsembleBright },
                { "StringEnsembleWarm",     StringEnsembleWarm },
                { "SynthStringsSaw",        SynthStringsSaw },
                { "SynthStringsWarm",       SynthStringsWarm },
                { "ChoirAahs",              ChoirAahs },
                { "VoiceOohs",              VoiceOohs },
                { "SynthChoir",             SynthChoir },
                { "OrchestraHit",           OrchestraHit },
                { "Trumpet",                Trumpet },
                { "Trombone",               Trombone },
                { "Tuba",                   Tuba },
                { "MutedTrumpet",           MutedTrumpet },
                { "FrenchHorn",             FrenchHorn },
                { "BrassSection",           BrassSection },
                { "SynthBrassBright",       SynthBrassBright },
                { "SynthBrassWarm",         SynthBrassWarm },
                { "SopranoSax",             SopranoSax },
                { "AltoSax",                AltoSax },
                { "TenorSax",               TenorSax },
                { "BaritoneSax",            BaritoneSax },
                { "Oboe",                   Oboe },
                { "EnglishHorn",            EnglishHorn },
                { "Bassoon",                Bassoon },
                { "Clarinet",               Clarinet },
                { "Piccolo",                Piccolo },
                { "Flute",                  Flute },
                { "Recorder",               Recorder },
                { "PanFlute",               PanFlute },
                { "BlownBottle",            BlownBottle },
                { "Shakuhachi",             Shakuhachi },
                { "Whistle",                Whistle },
                { "Ocarina",                Ocarina },
                { "SquareLead",             SquareLead },
                { "SawLead",                SawLead },
                { "CalliopeLead",           CalliopeLead },
                { "ChifferLead",            ChifferLead },
                { "CharangLead",            CharangLead },
                { "VoiceLead",              VoiceLead },
                { "FifthsLead",             FifthsLead },
                { "BassLead",               BassLead },
                { "NewAgePad",              NewAgePad },
                { "WarmPad",                WarmPad },
                { "PolysynthPad",           PolysynthPad },
                { "ChoirPad",               ChoirPad },
                { "BowedGlassPad",          BowedGlassPad },
                { "MetallicPad",            MetallicPad },
                { "HaloPad",                HaloPad },
                { "SweepPad",               SweepPad },
                { "Rain",                   Rain },
                { "SoundTrack",             SoundTrack },
                { "Crystal",                Crystal },
                { "Atmosphere",             Atmosphere },
                { "Brightness",             Brightness },
                { "Goblin",                 Goblin },
                { "Echoes",                 Echoes },
                { "SciFi",                  SciFi },
                { "Sitar",                  Sitar },
                { "Banjo",                  Banjo },
                { "Shamisen",               Shamisen },
                { "Koto",                   Koto },
                { "Kalimba",                Kalimba },
                { "Bagpipe",                Bagpipe },
                { "Fiddle",                 Fiddle },
                { "Shanai",                 Shanai },
                { "TinkleBell",             TinkleBell },
                { "Agogo",                  Agogo },
                { "SteelDrums",             SteelDrums },
                { "Woodblock",              Woodblock },
                { "TaikoDrum",              TaikoDrum },
                { "MelodicTom",             MelodicTom },
                { "SynthDrum",              SynthDrum },
                { "ReverseCymbal",          ReverseCymbal },
                { "GuitarFretNoise",        GuitarFretNoise },
                { "BreathNoise",            BreathNoise },
                { "Seashore",               Seashore },
                { "BirdTweet",              BirdTweet },
                { "TelephoneRing",          TelephoneRing },
                { "Helicopter",             Helicopter },
                { "Applause",               Applause },
                { "Gunshot",                Gunshot }
            };
            auto it = fastMap.find(name);
            return (it != fastMap.end()) ? it->second : AcousticGrandPiano;
        }
    };
}