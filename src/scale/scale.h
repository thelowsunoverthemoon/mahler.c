#ifndef __MAHLER_SCL_H__
#define __MAHLER_SCL_H__

#include <stddef.h>
#include <stdbool.h>

#include "err/err.h"
#include "note/note.h"
#include "inter/inter.h"

// Enums //

enum MahlerScaleType {
    MAHLER_ASCEND, MAHLER_DESCEND, MAHLER_FULL
};

// Structures //

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

// Predefined Scales //

extern struct ScaleBase const MAHLER_MAJOR_SCALE;           // Major Scale
extern struct ScaleBase const MAHLER_NATURAL_MIN_SCALE;     // Natural Minor Scale
extern struct ScaleBase const MAHLER_HARMONIC_MIN_SCALE;    // Harmonic Minor Scale
extern struct ScaleBase const MAHLER_MELODIC_MIN_SCALE;     // Melodic Minor Scale
extern struct ScaleBase const MAHLER_PENTATONIC_MAJ_SCALE;  // Major Pentatonic Scale
extern struct ScaleBase const MAHLER_PENTATONIC_MIN_SCALE;  // Minor Pentatonic Scale
extern struct ScaleBase const MAHLER_BLUES_SCALE;           // Blues Scale (hexatonic)
extern struct ScaleBase const MAHLER_WHOLE_TONE_SCALE;      // Whole Tone Scale
extern struct ScaleBase const MAHLER_OCTATONIC_HALF_SCALE;  // Octatonic Scale (starting with half tone)
extern struct ScaleBase const MAHLER_OCTATONIC_WHOLE_SCALE; // Octatonic Scale (starting with whole tone)

// Functions //

struct Scale getScale(struct Note const start, struct ScaleBase const* type, struct Note notes[], enum MahlerScaleType mode, enum MahlerError* err);
void returnScale(struct Note const notes[], size_t noteNum, struct ScaleResultList* list, struct ScaleCheck* custom, bool enharmonic, enum MahlerError* err);

#endif