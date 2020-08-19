#ifndef __MUSICTHEORY_H__
#define __MUSICTHEORY_H__

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
    int size;
    Interval* steps;
} ChordBase;

typedef struct {
    int length;
    Interval* steps;
} ScaleBase;

// Pre-defined Scales and Chord bases //

extern const ScaleBase MAJORSCALE;
extern const ScaleBase NATURALMINSCALE;
extern const ScaleBase HARMONICMINSCALE;
extern const ScaleBase MELODICMINSCALE;
extern const ChordBase MINORTRIAD;
extern const ChordBase MAJORTRIAD;
extern const ChordBase AUGMENTEDTRIAD;
extern const ChordBase DIMINISHEDTRIAD;
extern const ChordBase DIMINISHED7;
extern const ChordBase HALFDIMINISHED7;
extern const ChordBase MINOR7;
extern const ChordBase MAJOR7;
extern const ChordBase DOMINANT7;

// Defined function pointers for functions parameters. See documentation :) //

typedef int (*modeInter)(int*, int*);
typedef void (*modeScale)(Note[], const ScaleBase*, Note*, int);

// Print Functions //

void printNote(char* prefix, Note note, char* suffix);
void printChord(char* prefix, Chord chord, char* suffix);
void printScale(char* prefix, Scale scale, char* suffix);

// Interval Functions //

Note getInter(enum NoteOrder root, enum Accidental acci, int pitch, int inter, enum Quality quality, modeInter type);
Note getInterStruct(Note note, Interval interval, modeInter type);

// Chord Functions //

Chord getChord(Note note, const ChordBase* type, Note base[], Note notes[]);
Chord invertChord(Chord* chord, int inversion);

// Scale Functions //

Scale getScale (Note start, const ScaleBase* type, Note notes[], enum ScaleType mode);

// Misc Note Functions //

int isEnharmonic(Note notea, Note noteb);
double getFreqOrWave(Note note, int standard, enum NoteFormula type);

#endif