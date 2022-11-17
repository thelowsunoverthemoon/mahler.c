#ifndef __MAHLER_CHD_H__
#define __MAHLER_CHD_H__

#include <stddef.h>
#include <stdbool.h>

#include "err/err.h"
#include "note/note.h"
#include "inter/inter.h"

// Structures //

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

// Preset Chords //

extern struct ChordBase const MAHLER_MAJOR_TRIAD;       // Major Triad
extern struct ChordBase const MAHLER_MINOR_TRIAD;       // Minor Triad
extern struct ChordBase const MAHLER_AUGMENTED_TRIAD;   // Augmented Triad
extern struct ChordBase const MAHLER_DIMINISHED_TRIAD;  // Diminished Triad
extern struct ChordBase const MAHLER_DIMINISHED_7;      // Diminished 7th
extern struct ChordBase const MAHLER_HALF_DIMINISHED_7; // Half Diminished 7th
extern struct ChordBase const MAHLER_MINOR_7;           // Minor 7th
extern struct ChordBase const MAHLER_MAJOR_7;           // Major 7th
extern struct ChordBase const MAHLER_DOMINANT_7;        // Dominant 7th

// Functions //

struct Chord getChord(struct Note const root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes, enum MahlerError* err);
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResultList* list, struct ChordCheck* custom, bool enharmonic, enum MahlerError* err);
void invertChord(struct Chord* chord, int inv, enum MahlerError* err);

#endif