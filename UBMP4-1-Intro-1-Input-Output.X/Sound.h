#ifndef SOUND_H
#define SOUND_H
 
// This magic number is used to scale the calculated period of a note down
// in order to make the sound audible.
unsigned int PERIOD_SCALE = 1000;
 
// We use lower 5 bits of an integer to encode the note
#define MUSICAL_NOTE_BITS 5
 
// This bit mask is used to decode the note (only until application is created)
#define MUSICAL_NOTE_MASK 0b0000000000011111
#define OCTAVE_NOTE_MASK 0b0000011100000000

// bits 1-3 are for the rhythm index (as initialized at the start of the song)
// bit 4 is for indicating whether the note should be treated as silent or not
#define RHYTHM_MASK 0b00000111
// bits 5-8 are for the timbre
#define EFFECT_MASK 0b11110000
 
// Octave configuration
enum Octave
{
    //O1 is 0 << 8
    O2 = 1 << 8,
    O3 = 2 << 8,
    O4 = 3 << 8,
    O5 = 4 << 8,
    O6 = 5 << 8,
    O7 = 6 << 8,
    O8 = 7 << 8
};

// Here are the enumerated values for the notes for convenience
enum MusicalNote
{
    C = 0,
    Cs, // C sharp or D flat
    D,
    Ds,
    E,
    F,
    Fs,
    G,
    Gs,
    A,
    As,
    B,
    Rest,
    TheEnd, // this flag can be used to signal the end of a song
    Ou,     // this flag signals to move up one octave
    Od,     // this flag signals to move down one octave
    Or      // this flag signals to reset to default octave
};

// Temporary struct for testing (only until application is created)
struct Chord
{
    uint_least16_t chordNotes[3];
};

// Struct for initial values at the start of song
struct Song
{
    unsigned long periods[3];
    uint8_t silent1 : 1;
    uint8_t silent2 : 1;
    uint8_t silent3 : 1;
    unsigned char rhythmLengths[8];
    unsigned char silentRhythm;
    unsigned char firstEffects[3];
    unsigned char firstRhythms[3];
};

/**
 * Play a musical note
 *
 * @param notePlus a combined value that represent the MusicalNote and MusicalNoteLength.  
 * For example, a half note G can be encoded as notePlus = G | HalfNote
 */
void playNote(struct Chord chord);
 
/**
 * Make a noise on the buzzer with the given params a number of times (nTimes)
 **/
void _makeSound(struct Song song);

#endif