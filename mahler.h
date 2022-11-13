#ifndef __MAHLER_H__
#define __MAHLER_H__

#include <stdbool.h>

#define MAHLER_DISP_LEN 8              // default print size (note + max acci (4) + max number (99) + 1 (null terminating))
#define MAHLER_CHORD_LIST_DEFAULT NULL // default chord list for returnChord()
#define MAHLER_SCALE_LIST_DEFAULT NULL // default scale list for returnChord()

// Enumerators //

enum MahlerNote {
    MAHLER_C, MAHLER_D, MAHLER_E, MAHLER_F, MAHLER_G, MAHLER_A, MAHLER_B
};

enum MahlerQuality {
    MAHLER_MINOR = -1, MAHLER_MAJOR = 0, MAHLER_AUGMENTED = 1, MAHLER_DIMINISHED = -2, MAHLER_PERFECT = 3
};

enum MahlerAcci {
    MAHLER_DBFLAT = -2, MAHLER_FLAT = -1, MAHLER_NONE = 0, MAHLER_SHARP = 1, MAHLER_DBSHARP = 2
};

enum MahlerKeyType {
    MAHLER_MAJOR_KEY, MAHLER_MINOR_KEY
};

enum MahlerScaleType {
    MAHLER_ASCEND, MAHLER_DESCEND, MAHLER_FULL
};

enum MahlerError {
    MAHLER_ERROR_NONE,
    MAHLER_ERROR_INVALID_QUAL, MAHLER_ERROR_INVALID_RANGE, MAHLER_ERROR_INVALID_INTER,
    MAHLER_ERROR_INVALID_INVERSION, MAHLER_ERROR_INVALID_PRINT_NOTE, MAHLER_ERROR_INVALID_PRINT_QUAL,
    MAHLER_ERROR_OVERFLOW_PRINT_NOTE, MAHLER_ERROR_OVERFLOW_SCALE_RETURN, MAHLER_ERROR_OVERFLOW_CHORD_RETURN
};

// Structures //

struct Note {
    enum MahlerNote note;
    int             acci;
    int             pitch;
};

struct Interval {
    int                inter;
    enum MahlerQuality qual;
};

struct Chord {
    int                   size;
    int                   inv;
    struct Note* restrict base;
    struct Note* restrict notes;
};

struct ChordBase {
    char const*      name;
    int              size;
    struct Interval* steps;
};

struct ChordResult {
    struct Note             key;
    struct ChordBase const* chord;
};

struct ChordResultList {
    size_t              max;
    size_t              size;
    struct ChordResult* results;
};

struct ChordCheck {
    struct ChordBase const** pos;
    size_t                   size;
    struct Note* restrict    base;
    struct Note* restrict    notes;
};

struct Scale {
    int                  size;
    enum MahlerScaleType type;
    struct Note*         notes;
};

struct ScaleBase {
    char const*      name;
    int              size;
    struct Interval* steps;
};

struct ScaleResult {
    struct Note             key;
    struct ScaleBase const* scale;
};

struct ScaleResultList {
    size_t              max;
    size_t              size;
    struct ScaleResult* results;
};

struct ScaleCheck {
    struct ScaleBase const** pos;
    size_t                   size;
    struct Note*             notes;
};

struct KeySig {
    enum MahlerKeyType type;
    int                alter; // alteration
    int                size;
    struct Note        key;
    struct Note        notes[7];
};
// Preset Chords & Scales //

extern struct ScaleBase const MAHLER_MAJOR_SCALE;          // Major Scale
extern struct ScaleBase const MAHLER_NATURAL_MIN_SCALE;    // Natural Minor Scale
extern struct ScaleBase const MAHLER_HARMONIC_MIN_SCALE;   // Harmonic Minor Scale
extern struct ScaleBase const MAHLER_MELODIC_MIN_SCALE;    // Melodic Minor Scale
extern struct ScaleBase const MAHLER_PENTATONIC_MAJ_SCALE; // Major Pentatonic Scale
extern struct ScaleBase const MAHLER_PENTATONIC_MIN_SCALE; // Minor Pentatonic Scale
extern struct ScaleBase const MAHLER_BLUES_SCALE;          // Blues Scale (hexatonic)
extern struct ScaleBase const MAHLER_WHOLE_TONE_SCALE;     // Whole Tone Scale
extern struct ScaleBase const MAHLER_OCTATONIC_HALF_SCALE; // Octatonic Scale (starting with half tone)
extern struct ScaleBase const MAHLER_OCTATONIC_WHOLE_SCALE;// Octatonic Scale (starting with whole tone)

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

struct Note getInter(struct Note const note, struct Interval const interval, enum MahlerError* err);
struct Interval returnInter(struct Note const noteA, struct Note const noteB, enum MahlerError* err);

// Chord Functions //

struct Chord getChord(struct Note const root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes, enum MahlerError* err);
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResultList* list, struct ChordCheck* custom, bool enharmonic, enum MahlerError* err);
void invertChord(struct Chord* chord, int inv, enum MahlerError* err);

// Scale Functions //

struct Scale getScale(struct Note const start, struct ScaleBase const* type, struct Note notes[], enum MahlerScaleType mode, enum MahlerError* err);
void returnScale(struct Note const notes[], size_t noteNum, struct ScaleResultList* list, struct ScaleCheck* custom, bool enharmonic, enum MahlerError* err);

// Key Signature Functions //

struct KeySig getKeySig(struct Note key, enum MahlerKeyType type);
struct KeySig returnKeySig(int alter, enum MahlerKeyType type);
struct KeySig getKeyRelative(struct KeySig const* key);
int queryAcci(struct KeySig const* key, enum MahlerNote note);

// Misc Functions //

char const* getMahlerError(enum MahlerError err);
char* printNote(struct Note const note, char buf[], size_t size, enum MahlerError* err);
bool isEnharmonic(struct Note const noteA, struct Note const noteB);

#endif