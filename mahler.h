#ifndef __MAHLER_H__
#define __MAHLER_H__

#include <stdbool.h>

#define MAHLER_DISP_LEN 8 // default print size (note + max acci (4) + max number (99) + 1 (null terminating))

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

enum MahlerDegree {
    MAHLER_TONIC = 1, MAHLER_SUPERTONIC = 2, MAHLER_MEDIANT = 3, MAHLER_SUBDOMINANT = 4,
    MAHLER_DOMINANT = 5, MAHLER_SUBMEDIANT = 6, MAHLER_LEADING_TONE = 7, MAHLER_SUBTONIC = 8
};

enum MahlerNumeral {
    MAHLER_I = 1, MAHLER_II = 2, MAHLER_III = 3, MAHLER_IV = 4,
    MAHLER_V = 5, MAHLER_VI = 6, MAHLER_VII = 7, MAHLER_VIII = 8
};

enum MahlerMode {
    MAHLER_IONIAN = 1, MAHLER_DORIAN = 2, MAHLER_PHRYGIAN = 3, MAHLER_LYDIAN = 4,
    MAHLER_MIXOLYDIAN = 5, MAHLER_AEOLIAN = 6, MAHLER_LOCRIAN = 7
};

enum MahlerChordType {
     MAHLER_TRIAD = 3, MAHLER_SEVENTH_CHORD = 4, MAHLER_NINTH_CHORD = 5, MAHLER_ELEVENTH_CHORD = 6
};

enum MahlerError {
    MAHLER_ERROR_NONE,
    MAHLER_ERROR_INVALID_QUAL, MAHLER_ERROR_INVALID_INTER, MAHLER_ERROR_INVALID_INVERSION, MAHLER_ERROR_INVALID_PRINT_NOTE,
    MAHLER_ERROR_OVERFLOW_PRINT_NOTE, MAHLER_ERROR_OVERFLOW_SCALE_RETURN, MAHLER_ERROR_OVERFLOW_CHORD_RETURN
};

// Structures //

struct Note {
    enum MahlerNote note;
    int             acci; // use int instead? Since only guaranteed enum to hold char, could have way higher flats
    int             pitch;
};

struct Interval {
    int                inter;
    enum MahlerQuality quality;
};

struct Chord {
    int const                size;
    int                      inversion;
    struct Note const* const base; // include restrict?...
    struct Note* const       notes;
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

struct ChordList {
    struct ChordBase const** chordPos;
    size_t const             chordNum;
    struct Note* restrict    chordBase;
    struct Note* restrict    chordNotes;
};

struct Scale {
    int const            size;
    enum MahlerScaleType const type;
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

struct ScaleList {
    struct ScaleBase const** scalePos;
    size_t const             scaleNum;
    struct Note*             scaleArray;
};

struct KeySig {
    enum MahlerKeyType const type;
    int const                alter; // alteration
    int const                size;
    struct Note const        key;
    struct Note              notes[7];
};

// Preset Chords & Scales //

extern struct ScaleBase const MAHLER_MAJOR_SCALE;          // Major Scale
extern struct ScaleBase const MAHLER_NATURAL_MIN_SCALE;    // Natural Minor Scale
extern struct ScaleBase const MAHLER_HARMONIC_MIN_SCALE;   // Harmonic Minor Scale
extern struct ScaleBase const MAHLER_MELODIC_MIN_SCALE;    // Melodic Minor Scale
extern struct ScaleBase const MAHLER_PENTATONIC_MAJ_SCALE; // Major Pentatonic Scale
extern struct ScaleBase const MAHLER_PENTATONIC_MIN_SCALE; // Minor Pentatonic Scale
extern struct ScaleBase const MAHLER_BLUES_SCALE;          // Blues Scale (hexatonic)
extern struct ScaleBase const MAHLER_WHOLE_TONE_SCALE;     // Octatonic Scale (starting with half tone)
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

struct Note getInter(struct Note note, struct Interval interval);
struct Interval returnInter(struct Note noteA, struct Note noteB);

// Chord Functions //

struct Chord getKeyChord(struct KeySig const* key, size_t index, size_t size, struct Note* restrict base, struct Note* restrict notes);
struct Chord getChord(struct Note root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes);
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResult list[], size_t listMax, struct ChordList const* custom, bool enharmonic);
void invertChord(struct Chord* chord, int inversion);

// Scale Functions //

struct Scale getKeyScale(struct KeySig const* key, size_t index, enum MahlerScaleType mode, struct ScaleBase const* type, struct Note* notes);
struct Scale getScale(struct Note start, const struct ScaleBase* type, struct Note notes[], enum MahlerScaleType mode);
void returnScale(struct Note const notes[], size_t noteNum, struct ScaleResult list[], size_t listMax, struct ScaleList const* custom, bool enharmonic);

// Key Signature Functions //

struct KeySig getKeySig(struct Note key, enum MahlerKeyType type);
struct KeySig returnKeySig(char const* str, enum MahlerKeyType type);
struct KeySig getKeyRelative(struct KeySig const* key);
int queryAcci(struct KeySig const* key, enum MahlerNote note);

// Misc Functions //

char* getMahlerError(void);
char* printNote(struct Note const note, char buf[], size_t size);
bool isEnharmonic(struct Note notea, struct Note noteb);

#endif
