#ifndef __MAHLER_NOTE_H__
#define __MAHLER_NOTE_H__

#include <stdbool.h>

// Enums //

enum MahlerNote {
    MAHLER_C, MAHLER_D, MAHLER_E, MAHLER_F, MAHLER_G, MAHLER_A, MAHLER_B
};

enum MahlerAcci {
    MAHLER_DBFLAT = -2, MAHLER_FLAT = -1, MAHLER_NONE = 0, MAHLER_SHARP = 1, MAHLER_DBSHARP = 2
};

// Structures //

struct Note {
    enum MahlerNote note;
    int             acci;
    int             pitch;
};

// Helper Functions //

extern inline int toSemitone(int note);
extern inline struct Note getEnharmonic(struct Note note);
extern inline bool matchNote(struct Note const notes[], size_t noteNum, struct Note scale[], size_t scaleNum);

#endif