#ifndef __MAHLER_INTER_H__
#define __MAHLER_INTER_H__

#include "err/err.h"
#include "note/note.h"

// Enums //

enum MahlerQuality {
    MAHLER_MINOR = -1, MAHLER_MAJOR = 0, MAHLER_AUGMENTED = 1, MAHLER_DIMINISHED = -2, MAHLER_PERFECT = 3
};

// Structures //

struct Interval {
    int                inter;
    enum MahlerQuality qual;
};

// Functions //

struct Note getInter(struct Note const note, struct Interval const interval, enum MahlerError* err);
struct Interval returnInter(struct Note const noteA, struct Note const noteB, enum MahlerError* err);

#endif