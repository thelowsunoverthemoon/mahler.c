#ifndef __MAHLER_H__
#define __MAHLER_H__

#include <stdbool.h>

#define MAHLER_DISP_LEN 8 // default print size (note + max acci (4) + max number (99) + 1 (null terminating))

// Enumerators //

enum NoteOrder {
    MAHLER_C, MAHLER_D, MAHLER_E, MAHLER_F, MAHLER_G, MAHLER_A, MAHLER_B
};

enum Quality {
    MAHLER_MINOR = -1, MAHLER_MAJOR = 0, MAHLER_AUGMENTED = 1, MAHLER_DIMINISHED = -2, MAHLER_PERFECT = 3
};

enum Accidental {
    MAHLER_DBFLAT = -2, MAHLER_FLAT = -1, MAHLER_NONE = 0, MAHLER_SHARP = 1, MAHLER_DBSHARP = 2
};

enum ScaleType {
    MAHLER_ASCEND, MAHLER_DESCEND, MAHLER_FULL
};

enum MAHLER_Error {
    MAHLER_ERROR_NONE,
    MAHLER_ERROR_INVALID_QUAL, MAHLER_ERROR_INVALID_INTER, MAHLER_ERROR_INVALID_INVERSION, MAHLER_ERROR_INVALID_PRINT_NOTE,
    MAHLER_ERROR_OVERFLOW_PRINT_NOTE, MAHLER_ERROR_OVERFLOW_SCALE_RETURN, MAHLER_ERROR_OVERFLOW_CHORD_RETURN
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

extern struct ScaleBase const MAHLER_MAJOR_SCALE;          // Major Scale
extern struct ScaleBase const MAHLER_NATURAL_MIN_SCALE;    // Natural Minor Scale
extern struct ScaleBase const MAHLER_HARMONIC_MIN_SCALE;   // Harmonic Minor Scale
extern struct ScaleBase const MAHLER_MELODIC_MIN_SCALE;    // Melodic Minor Scale
extern struct ScaleBase const MAHLER_PENTATONIC_MAJ_SCALE; // Major Pentatonic Scale
extern struct ScaleBase const MAHLER_PENTATONIC_MIN_SCALE; // Minor Pentatonic Scale
extern struct ScaleBase const MAHLER_BLUES_SCALE;          // Blues Scale (hexatonic)

extern struct ChordBase const MAHLER_MAJOR_TRIAD;          // Major Triad
extern struct ChordBase const MAHLER_MINOR_TRIAD;          // Minor Triad
extern struct ChordBase const MAHLER_AUGMENTED_TRIAD;      // Augmented Triad
extern struct ChordBase const MAHLER_DIMINISHED_TRIAD;     // Diminished Triad
extern struct ChordBase const MAHLER_DIMINISHED_7;         // Diminished 7th
extern struct ChordBase const MAHLER_HALF_DIMINISHED_7;    // Half Diminished 7th
extern struct ChordBase const MAHLER_MINOR_7;              // Minor 7th
extern struct ChordBase const MAHLER_MAJOR_7;              // Major 7th
extern struct ChordBase const MAHLER_DOMINANT_7;           // Dominant 7th

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

char* getMAHLERError(void);
char* printNote(struct Note const note, char buf[], size_t size);
bool isEnharmonic(struct Note noteA, struct Note noteB);

#endif
