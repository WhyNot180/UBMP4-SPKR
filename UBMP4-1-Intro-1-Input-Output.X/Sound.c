#include    "xc.h"
#include    "stdbool.h"
#include    "UBMP410.h"
#include    "Sound.h"
 
#define MIN(x, y) x < y ? x : y
#define MAX(x, y) x < y ? y : x
 
 
unsigned long ipow(unsigned long base, unsigned char exp) {
    unsigned long result = base;
    for (unsigned char i = 1; i < exp; i++) {
        result *= base;
    }
    return result;
}
 
#define CLOCK_FREQ 48000000
unsigned long lowerNotePeriods[] = {
    // These period values must align with the MusicalNote indexes
    // Note frequency values attained from https://pages.mtu.edu/~suits/notefreqs.html
    // The period is calculated by dividing the clock frequency the by note frequency
    CLOCK_FREQ / 1635 * 100, // C
    CLOCK_FREQ / 1732 * 100, // Cs
    CLOCK_FREQ / 1835 * 100, // D
    CLOCK_FREQ / 1945 * 100, // Ds
    CLOCK_FREQ / 2060 * 100, // E
    CLOCK_FREQ / 2183 * 100, // F
    CLOCK_FREQ / 2312 * 100, // Fs
    CLOCK_FREQ / 2450 * 100, // G
    CLOCK_FREQ / 2596 * 100, // Gs
    CLOCK_FREQ / 2750 * 100, // A
    CLOCK_FREQ / 2914 * 100, // As
    CLOCK_FREQ / 3087 * 100, // B
};
 
void _makeSound(unsigned long cycles, unsigned long period, bool silent, unsigned long period2, bool silent2, unsigned long period3, bool silent3)
{
    unsigned long truePeriod;
    unsigned char alternate;
    bool trueSilent;
    for (unsigned int c = 0; c < cycles; c++)
    {
        alternate = c % 3;
        switch (alternate){
            case (0):
                truePeriod = period3;
                trueSilent = silent3;
                break;
            case (1):
                truePeriod = period;
                trueSilent = silent;
                break;
            case (2):
                truePeriod = period2;
                trueSilent = silent2;
                break;
            default:
                break;
        }
        if (!trueSilent)
            BEEPER = !BEEPER;
        for (unsigned long p = 0; p < truePeriod; p++)
            ;
    }
}
 
void playNote(char notePlus, char note2Plus, char note3Plus)
{
    enum MusicalNote note = notePlus & MUSICAL_NOTE_MASK;
    unsigned long period = 0;

    enum MusicalNote note2 = note2Plus & MUSICAL_NOTE_MASK;
    unsigned long period2 = 0;

    enum MusicalNote note3 = note3Plus & MUSICAL_NOTE_MASK;
    unsigned long period3 = 0;
 
    switch (note)
    {
    case Ou:
        currentOctave = MIN(MAX_OCTAVE, currentOctave + 1);
        note = Rest;
        break;
    case Od:
        currentOctave = MAX(1, currentOctave - 1);
        note = Rest;
        break;
    case Or:
        currentOctave = DEFAULT_OCTAVE;
        note = Rest;
        break;
    case Rest:
        // It shouldn't matter what the period is, as long as the total cycles normalizes to the proper
        // note length.  We just want the Rest to be silent for the correct length of time.
        period = CLOCK_FREQ / 62;
        break;
    default:
        if (note <= B)
            period = lowerNotePeriods[note];
        break;
    }

    // For secondary note
    switch (note2)
    {
    case Ou:
        secondaryOctave = MIN(MAX_OCTAVE, secondaryOctave + 1);
        note2 = Rest;
        break;
    case Od:
        secondaryOctave = MAX(1, secondaryOctave - 1);
        note2 = Rest;
        break;
    case Or:
        secondaryOctave = DEFAULT_OCTAVE;
        note2 = Rest;
        break;
    case Rest:
        // It shouldn't matter what the period is, as long as the total cycles normalizes to the proper
        // note length.  We just want the Rest to be silent for the correct length of time.
        period2 = CLOCK_FREQ / 62;
        break;
    default:
        if (note2 <= B)
            period2 = lowerNotePeriods[note2];
        break;
    }
    
    // For tertiary note
    switch (note3)
    {
    case Ou:
        tertiaryOctave = MIN(MAX_OCTAVE, tertiaryOctave + 1);
        note3 = Rest;
        break;
    case Od:
        tertiaryOctave = MAX(1, tertiaryOctave - 1);
        note3 = Rest;
        break;
    case Or:
        tertiaryOctave = DEFAULT_OCTAVE;
        note3 = Rest;
        break;
    case Rest:
        // It shouldn't matter what the period is, as long as the total cycles normalizes to the proper
        // note length.  We just want the Rest to be silent for the correct length of time.
        period3 = CLOCK_FREQ / 62;
        break;
    default:
        if (note3 <= B)
            period3 = lowerNotePeriods[note3];
        break;
    }
 
    enum MusicalNoteLength noteLength = notePlus & ~MUSICAL_NOTE_MASK;
    unsigned long length = EIGHTH_NOTE_DURATION_CYCLES;

    enum MusicalNoteLength note2Length = note2Plus & ~MUSICAL_NOTE_MASK;
    unsigned long length2 = EIGHTH_NOTE_DURATION_CYCLES;

    enum MusicalNoteLength note3Length = note3Plus & ~MUSICAL_NOTE_MASK;
    unsigned long length3 = EIGHTH_NOTE_DURATION_CYCLES;
 
    switch (noteLength)
    {
    case QuarterNote:
        length = length * 2;
        break;
    case ThreeEighthNote:
        length = length * 3;
        break;
    case HalfNote:
        length = length * 4;
        break;
    case SixEighthNote:
        length = length * 6;
        break;
    case FullNote:
        length = length * 8;
        break;
    }

    switch (note2Length)
    {
    case QuarterNote:
        length2 = length2 * 2;
        break;
    case ThreeEighthNote:
        length2 = length2 * 3;
        break;
    case HalfNote:
        length2 = length2 * 4;
        break;
    case SixEighthNote:
        length2 = length2 * 6;
        break;
    case FullNote:
        length2 = length2 * 8;
        break;
    }

    switch (note3Length)
    {
    case QuarterNote:
        length3 = length3 * 2;
        break;
    case ThreeEighthNote:
        length3 = length3 * 3;
        break;
    case HalfNote:
        length3 = length3 * 4;
        break;
    case SixEighthNote:
        length3 = length3 * 6;
        break;
    case FullNote:
        length3 = length3 * 8;
        break;
    }
 
    // We need to adjust the period by the octave (and a preferred scaling value)
    // Also, we want the note to play for the precise length of time regardless of the period
    // so we have to adjust the number of cycles by the period
    unsigned long adjustedPeriod = period / ipow(2, currentOctave) / PERIOD_SCALE;
    unsigned long adjustedPeriod2 = period2 / ipow(2, secondaryOctave) / PERIOD_SCALE;
    unsigned long adjustedPeriod3 = period3 / ipow(2, tertiaryOctave) / PERIOD_SCALE;
    _makeSound(length / (adjustedPeriod + adjustedPeriod2 + adjustedPeriod3), adjustedPeriod, note == Rest ? true : false,
            adjustedPeriod2, note2 == Rest ? true : false,
            adjustedPeriod3, note3 == Rest ? true : false);
 
    __delay_ms(16);
}