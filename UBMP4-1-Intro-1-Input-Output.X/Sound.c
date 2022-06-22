#include    "xc.h"
#include    "stdint.h"
#include    "stdbool.h"
#include    "UBMP410.h"
#include    "Sound.h"
 
#define MIN(x, y) x < y ? x : y
#define MAX(x, y) x < y ? y : x
#define TOTALNOTES (period == 0 ? 0 : 1) + (period2 == 0 ? 0 : 1) + (period3 == 0 ? 0 : 1)
#define ISREST(x) x == Rest ? 1 : 0

// x is whether it is silent or not, y is the period, z is the amount to bitshift
#define PERIODGATE(x, y, z) !x ? y << z : 1
#define WAVEFORMGATE(x, y, z) !x ? y >> z : 0
 
 
unsigned long ipow(unsigned long base, unsigned char exp) {
    unsigned long result = base;
    for (unsigned char i = 1; i < exp; i++) {
        result *= base;
    }
    return result;
}

unsigned char findOctave(enum Octave note) {
    switch (note)
    {
    case O2:
        return 2;
    case O3:
        return 3;
    case O4:
        return 4;
    case O5:
        return 5;
    case O6:
        return 6;
    case O7:
        return 7;
    case O8:
        return 8;
    default:
        return 1;
    }
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

void receivePitchData(unsigned char sendHigh, unsigned char sendLow, unsigned long* truePeriod, unsigned long* pitch, unsigned long* waveForm, unsigned char silent, uint8_t effect) {
    unsigned long pitchData[2] = {0, 0};
    // asks for pitch info
    TXREG = sendHigh;
    while (!TRMT);
    while (!RCIF);
    pitchData[0] = bluetooth_getChar();
    TXREG = sendLow;
    while (!TRMT);
    while (!RCIF);
    pitchData[1] = bluetooth_getChar();
    *truePeriod = PERIODGATE(silent, ((pitchData[0] << 8) | pitchData[1]), 1); // Stitches together the 8-bit data into a 16-bit int
    *pitch = *truePeriod;
    *waveForm = WAVEFORMGATE(silent, *truePeriod, effect);
}

void receiveRhythmData(unsigned char send, unsigned char* coreRhythm, unsigned int* trueRhythm, unsigned char* silent, uint8_t* effect) {
    // asks for rhythm, silent, and effect info
    TXREG = send;
    while (!TRMT);
    while (!RCIF);
    char tempData = bluetooth_getChar();
    coreRhythm += (tempData & RHYTHM_MASK);
    *trueRhythm = *coreRhythm;
    *effect = (tempData & EFFECT_MASK) >> 4;
    *silent = tempData & (1 << 3);
}
 
void _makeSound(struct Song song)
{
    // The full length periods of each note
    unsigned long truePeriods[3] = { PERIODGATE(song.silent1, song.periods[0], 1),
        PERIODGATE(song.silent2, song.periods[1], 1),
        PERIODGATE(song.silent3, song.periods[2], 1) };

    // The duty cycle of each note (e.g. if truePeriod[n] = 100 and firstEffects[n] = 4 then waveForms[n] = 6)
    unsigned long waveForms[3] = { WAVEFORMGATE(song.silent1, truePeriods[0], song.firstEffects[0]),
        WAVEFORMGATE(song.silent2, truePeriods[1], song.firstEffects[1]),
        WAVEFORMGATE(song.silent3, truePeriods[2], song.firstEffects[2])};

    // Counters for the pitch of each note
    unsigned long pitch[3] = { truePeriods[0], truePeriods[1], truePeriods[2]};

    // The core specified rhythms (index 7 = whole note, index 6 = dotted half, index 5 = half...)
    unsigned char coreRhythms[8];

    for (int i = 7; i != -1; i--) {
        coreRhythms[i] = song.rhythmLengths[i];
    }

    // Rhythm counters for each note
    unsigned int rhythms[3] = { song.firstRhythms[0], song.firstRhythms[1], song.firstRhythms[2] };

    // By using this we can prevent the silent rhythm and non-silent rhythms changing
    unsigned int trueRhythms[3] = { song.firstRhythms[0], song.firstRhythms[1], song.firstRhythms[2] };

    // Effects counters for each note
    uint8_t effects[3] = { song.firstEffects, song.firstEffects, song.firstEffects };

    // 1 if note is rest else false
    unsigned char silents[3] = { song.silent1, song.silent2, song.silent3 };

    // FOSC / sample rate (48000000 / 220000)
    unsigned long masterCount = 218;

    // Variable for storing rhythm and effect info from bluetooth
    uint16_t tempData = 0;

    // Array for storing pitch info from bluetooth (bluetooth only allows 8-bit characters)
    uint16_t pitchData[2] = { 0, 0 };

    // Output [n][0] is for pitch and effect domain and output [n][1] is for rhythm domain
    char outputs[3][2] = { {0, 0},
                            {0, 0},
                            {0, 0} };

    do {
        BEEPER = outputs[0][0] & outputs[0][1]; // ANDs the pitch/effect domains with the rhythm domain to produce sound on the beeper
        LED3 = outputs[0][0] & outputs[0][1];
        // Decrements pitch counter every cycle of the while loop and if it is less than the duty cycle (i.e 6.25%) then the output is one else it is 0
        if (pitch[0]-- == 0) {
            pitch[0] = truePeriods[0];
        }
        if (pitch[0] <= waveForms[0]) {
            outputs[0][0] = 1;
        }
        if (pitch[0] >= waveForms[0]) {
            outputs[0][0] = 0;
        }


        BEEPER = outputs[1][0] & outputs[1][1];
        LED3 = outputs[1][0] & outputs[1][1];
        if (pitch[1]-- == 0) {
            pitch[1] = truePeriods[1];
        }
        if (pitch[1] <= waveForms[1]) {
            outputs[1][0] = 1;
        }
        if (pitch[1] >= waveForms[1]) {
            outputs[1][0] = 0;
        }

        BEEPER = outputs[2][0] & outputs[2][1];
        LED3 = outputs[2][0] & outputs[2][1];
        if (pitch[2]-- == 0) {
            pitch[2] = truePeriods[2];
        }
        if (pitch[2] <= waveForms[2]) {
            outputs[2][0] = 1;
        }
        if (pitch[2] >= waveForms[2]) {
            outputs[2][0] = 0;
        }

        // This counter syncs the rhythms of each note
        if (masterCount-- == 0) {
            masterCount = 218;
            if (rhythms[0]-- == 0) {
                rhythms[0] = outputs[0][1] ? song.silentRhythm : trueRhythms[0];
                outputs[0][1] ^= 1;
                if (!outputs[0][1]) {

                    receiveRhythmData(88, coreRhythms, &trueRhythms[0], &silents[0], &effects[0]);

                    receivePitchData(97, 98, &truePeriods[0], &pitch[0], &waveForms[0], silents[0], effects[0]);
                }
            }
            if (rhythms[1]-- == 0) {
                rhythms[1] = outputs[1][1] ? song.silentRhythm : trueRhythms[1];
                outputs[1][1] ^= 1;
                if (!outputs[1][1]) {

                    receiveRhythmData(89, coreRhythms, &trueRhythms[1], &silents[1], &effects[1]);

                    receivePitchData(99, 100, &truePeriods[1], &pitch[1], &waveForms[1], silents[1], effects[1]);
                }
            }
            if (rhythms[2]-- == 0) {
                rhythms[2] = outputs[2][1] ? song.silentRhythm : trueRhythms[2];
                outputs[2][1] ^= 1;
                if (!outputs[2][1]) {

                    receiveRhythmData(90, coreRhythms, &trueRhythms[2], &silents[2], &effects[2]);

                    receivePitchData(101, 102, &truePeriods[2], &pitch[2], &waveForms[2], silents[2], effects[2]);
                }
            }
        }
    } while (1);
    BEEPER = 0;
}
 
void playNote(void)
{
    while (!RCIF) {
        TXREG = 1;
        while (!TRMT);
    }
    __delay_ms(500);
    char throwAway = bluetooth_getChar();

    struct Song song;

    // Initializes core rhythm lengths by asking for them from connected bluetooth device
    for (int i = 7; i != -1; i--) {
        TXREG = i + 48; // sends numbers 7-1 in ascii
        while (!TRMT); // Waits for TXREG to be empty
        while (!RCIF); // Waits for RCREG to be not empty
        song.rhythmLengths[i] = bluetooth_getChar();
    }
    
    receiveRhythmData(88, &song.rhythmLengths, &song.firstRhythms[0], &song.silent1, &song.firstEffects[0]);
    receiveRhythmData(89, &song.rhythmLengths, &song.firstRhythms[1], &song.silent2, &song.firstEffects[1]);
    receiveRhythmData(90, &song.rhythmLengths, &song.firstRhythms[2], &song.silent3, &song.firstEffects[2]);

    unsigned long throwAwayLong = 0;
    receivePitchData(97, 98, &song.periods[0], &throwAwayLong, &throwAwayLong, song.silent1, 0);
    receivePitchData(99, 100, &song.periods[1], &throwAwayLong, &throwAwayLong, song.silent2, 0);
    receivePitchData(101, 102, &song.periods[2], &throwAwayLong, &throwAwayLong, song.silent3, 0);

    // Initializes the length of silence between notes
    TXREG = 58; // sends ':' in ascii
    while (!RCIF);
    song.silentRhythm = bluetooth_getChar();

    _makeSound(song);
}