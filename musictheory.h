#ifndef __MUSICTHEORY_H__
#define __MUSICTHEORY_H__

// Macros for getInter parameter "type" //

#define COMPOUND modeCompoundInter
#define SIMPLE modeSimpleInter

// Enumerators //

enum ScaleType {ASCEND, DESCEND, FULL};
enum NoteFormula {FREQUENCY, WAVELENGTH};
enum NoteOrder {C, D, E, F, G, A, B};
enum Quality {MINOR = -1, MAJOR = 0, AUGMENTED = 1, DIMINISHED = -2, PERFECT = 3};
enum Accidental {DBFLAT = -2, FLAT = -1, NONE = 0, SHARP = 1, DBSHARP = 2};
enum PitchStandard {BAROQUE = 415, STANDARD = 440, CLASSICAL = 430};

// Structures //

typedef struct {
    enum NoteOrder note;
    enum Accidental acci;
    int pitch;
} Note;

typedef struct {
    int inter;
    enum Quality quality;
} Interval;

typedef struct {
    int size;
    int inversion;
    Note* base;
    Note* notes;
} Chord;

typedef struct {
    int size;
    int type;
    Note* notes;
} Scale;

typedef struct {
    const int size;
    Interval* const steps;
} ChordBase;

typedef struct {
    const int length;
    Interval* const steps;
} ScaleBase;

// Pre-defined Scales and Chord bases //

extern const ScaleBase MAJOR_SCALE;          // Major Scale
extern const ScaleBase NATURAL_MIN_SCALE;    // Natural Minor Scale
extern const ScaleBase HARMONIC_MIN_SCALE;   // Harmonic Minor Scale
extern const ScaleBase MELODIC_MIN_SCALE;    // Melodic Minor Scale
extern const ScaleBase PENTATONIC_MIN_SCALE; // Major Pentatonic Scale
extern const ScaleBase PENTATONIC_MAJ_SCALE; // Minor Pentatonic Scale (relative minor)
extern const ChordBase MINOR_TRIAD;          // Minor Triad
extern const ChordBase MAJOR_TRIAD;          // Major Triad
extern const ChordBase AUGMENTED_TRIAD;      // Augmented Triad
extern const ChordBase DIMINISHED_TRIAD;     // Diminished Triad
extern const ChordBase DIMINISHED_7;         // Diminished 7th
extern const ChordBase HALF_DIMINISHED_7;    // Half Diminished 7th
extern const ChordBase MINOR_7;              // Minor 7th
extern const ChordBase MAJOR_7;              // Major 7th
extern const ChordBase DOMINANT_7;           // Dominant 7th

// Defined function pointers for functions parameters. See documentation :) //

typedef int (*modeInter)(int*, int*);
typedef void (*modeScale)(Note[], const ScaleBase*, Note*, int);

// Print Functions //

void printNote(char* prefix, Note note, char* suffix);
void printInter(char* prefix, Interval inter, char* suffix);
void printChord(char* prefix, Chord chord, char* suffix);
void printScale(char* prefix, Scale scale, char* suffix);

// Interval Functions //

Note getInter(enum NoteOrder root, enum Accidental acci, int pitch, int inter, enum Quality quality, modeInter type);
Note getInterStruct(Note note, Interval interval, modeInter type);
Interval returnInter(Note notea, Note noteb);

// Chord Functions //

Chord getChord(Note note, const ChordBase* type, Note base[], Note notes[]);
Chord invertChord(Chord* chord, int inversion);

// Scale Functions //

Scale getScale (Note start, const ScaleBase* type, Note notes[], enum ScaleType mode);

// Misc Note Functions //

int isEnharmonic(Note notea, Note noteb);
double getFreqOrWave(Note note, int standard, enum NoteFormula type);

#endif
