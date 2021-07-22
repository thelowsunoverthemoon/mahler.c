#ifndef __MUSICTHEORY_H__
#define __MUSICTHEORY_H__

#include <stdbool.h>

#define MT_DISP_LEN 8 // default print size (note + max acci (4) + max number (99) + 1 (null terminating))

// Enumerators //

enum NoteOrder {
    C, D, E, F, G, A, B
};

enum Quality {
    MINOR = -1, MAJOR = 0, AUGMENTED = 1, DIMINISHED = -2, PERFECT = 3
};

enum Accidental {
    DBFLAT = -2, FLAT = -1, NONE = 0, SHARP = 1, DBSHARP = 2
};

enum ScaleType {
    SCALE_ASCEND, SCALE_DESCEND, SCALE_FULL
};

enum MT_Error {
    MT_ERROR_NONE,
    MT_ERROR_INVALID_QUAL, MT_ERROR_INVALID_INTER, MT_ERROR_INVALID_INVERSION, MT_ERROR_INVALID_PRINT_NOTE,
    MT_ERROR_OVERFLOW_PRINT_NOTE, MT_ERROR_OVERFLOW_SCALE_RETURN, MT_ERROR_OVERFLOW_CHORD_RETURN
};

// Structures //

struct Note {
    enum NoteOrder  note;
    enum Accidental acci;
    int             pitch;
};

struct Interval {
    int          inter;
    enum Quality quality;
};

struct Chord {
    int const                         size;
    int                               inversion;
    struct Note const* const restrict base;
    struct Note* const restrict       notes;
};

struct ChordResult {
    struct Note             key;
    struct ChordBase const* chord;
};

struct ChordBase {
    char const*            name;
    int const              size;
    struct Interval const* steps;
};

struct Scale {
    int const            size;
    enum ScaleType const type;
    struct Note* const   notes;
};

struct ScaleResult {
    struct Note             key;
    struct ScaleBase const* scale;
};

struct ScaleBase {
    char const*            name;
    int const              size;
    struct Interval const* steps;
};

// Preset Chords & Scales //

extern struct ScaleBase const MAJOR_SCALE;          // Major Scale
extern struct ScaleBase const NATURAL_MIN_SCALE;    // Natural Minor Scale
extern struct ScaleBase const HARMONIC_MIN_SCALE;   // Harmonic Minor Scale
extern struct ScaleBase const MELODIC_MIN_SCALE;    // Melodic Minor Scale
extern struct ScaleBase const PENTATONIC_MAJ_SCALE; // Major Pentatonic Scale
extern struct ScaleBase const PENTATONIC_MIN_SCALE; // Minor Pentatonic Scale
extern struct ScaleBase const BLUES_SCALE;          // Blues Scale (hexatonic)

extern struct ChordBase const MAJOR_TRIAD;          // Major Triad
extern struct ChordBase const MINOR_TRIAD;          // Minor Triad
extern struct ChordBase const AUGMENTED_TRIAD;      // Augmented Triad
extern struct ChordBase const DIMINISHED_TRIAD;     // Diminished Triad
extern struct ChordBase const DIMINISHED_7;         // Diminished 7th
extern struct ChordBase const HALF_DIMINISHED_7;    // Half Diminished 7th
extern struct ChordBase const MINOR_7;              // Minor 7th
extern struct ChordBase const MAJOR_7;              // Major 7th
extern struct ChordBase const DOMINANT_7;           // Dominant 7th

// Interval Functions //

struct Note getInter(struct Note note, struct Interval interval);
struct Interval returnInter(struct Note notea, struct Note noteb);

// Chord Functions //

struct Chord getChord(struct Note root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes);
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResult list[], size_t listMax, bool useEnharmonic);
void invertChord(struct Chord* chord, int inversion);

// Scale Functions //

struct Scale getScale(struct Note start, const struct ScaleBase* type, struct Note notes[], enum ScaleType mode);
void returnScale(struct Note const note[], size_t noteNum, struct ScaleResult list[], size_t listMax, bool useEnharmonic);

// Misc Functions //

char* getMTError(void);
char* printNote(struct Note const note, char buf[], size_t size);
bool isEnharmonic(struct Note noteA, struct Note noteB);

#endif
