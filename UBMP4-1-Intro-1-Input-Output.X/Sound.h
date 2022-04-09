#ifndef SOUND_H
#define SOUND_H
 
// This magic number is used to scale the calculated period of a note down
// in order to make the sound audible.
unsigned int PERIOD_SCALE = 1000;
 
// We use lower 5 bits of an integer to encode the note
#define MUSICAL_NOTE_BITS 5
 
// This bit mask is used to decode the note
#define MUSICAL_NOTE_MASK 0b00011111
 
// Octave configuration
#define DEFAULT_OCTAVE 4
#define MAX_OCTAVE 8
char currentOctave = DEFAULT_OCTAVE;
char secondaryOctave = DEFAULT_OCTAVE;
cahr tertiaryOctave = DEFAULT_OCTAVE;

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
 
// Here are the enumerated values for the standard lengths of a notes.
// Note that an eighth-note is the default length so does not need to expressed explicitly.
enum MusicalNoteLength
{
    //EighthNote = 0 << MUSICAL_NOTE_BITS, // This the default
    QuarterNote = 1 << MUSICAL_NOTE_BITS,
    ThreeEighthNote = 2 << MUSICAL_NOTE_BITS,
    HalfNote = 3 << MUSICAL_NOTE_BITS,
    SixEighthNote = 4 << MUSICAL_NOTE_BITS,
    FullNote = 5 << MUSICAL_NOTE_BITS
};
// This is the duration of a quarter note expressed in program-execution cycles.
// The actual duration of the note played will depend on the processor speed/frequency.
unsigned long EIGHTH_NOTE_DURATION_CYCLES = 70000;
 
/**
 * Play a musical note
 *
 * @param notePlus a combined value that represent the MusicalNote and MusicalNoteLength.  
 * For example, a half note G can be encoded as notePlus = G | HalfNote
 */
void playNote(char notePlus);
 
/**
 * Make a noise on the buzzer with the given params a number of times (nTimes)
 **/
void makeSound(unsigned long cycles, unsigned long period);
 
#endif