/*

| mahler.c |
defines the basic Note, as well as basic macros/helpers/enums
#includes code for all other parts of library

*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Macros //

#define SIZE_CHROMATIC 12    // Size of normal chromatic scale
#define ADJUST_NOTE 7        // Size of note scale (enum MahlerNote)
#define SIMPLE_INTER_MAX 8   // Simple Interval maximum
#define KEYSIG_MAX 7         // Size of Circle of Fifths
#define KEYSIG_DIF 3         // Difference of Parallel Major and Minor (G+ = 1, 1 - 3 = -2 = G-)
#define SCALE_DEFAULT_NUM 4  // Default Scale List Number for returnScale
#define CHORD_DEFAULT_NUM 6  // Default Chord List Number for returnChord

#define RETURN_OVERFLOW(err) {\
    if (list->size == list->max) {\
        SET_ERROR(err);\
        return;\
    }\
} while(0)

#define CHECK_MATCH(type, match, num, pos, err) {\
    if (matchNote(notes, noteNum, match, num)) {\
        RETURN_OVERFLOW(err);\
        list->results[list->size++] = (type) {test, pos};\
        if (enharmonic && a != MAHLER_NONE) {\
            RETURN_OVERFLOW(err);\
            list->results[list->size++] = (type) {getEnharmonic(test), pos};\
        }\
    }\
} while(0)

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

static inline int toSemitone(int note);
static inline struct Note getEnharmonic(struct Note note);
static inline bool matchNote(struct Note const notes[], size_t noteNum, struct Note scale[], size_t scaleNum);

// Includes //

#include "err.c"
#include "inter.c"
#include "chord.c"
#include "scale.c"
#include "key.c"
#include "misc.c"

// Helper Function Definitions //

static inline int
toSemitone(int const note)
{
    return note * 2 + (note <= MAHLER_E);  // "if" accounts for F (we convert from C, D, E, to C, C#, D, ect)
}

static inline struct Note
getEnharmonic(struct Note note) // only works for sharps and flats
{
    enum MahlerNote tone = note.note;
    
    int enharmonic = tone + note.acci;
    if (enharmonic > MAHLER_B) {
        enharmonic = MAHLER_C;
    } else if (enharmonic < MAHLER_C) {
        enharmonic = MAHLER_B;
    }
    
    int acci = note.acci;
    return (struct Note) {
        .note  = enharmonic,
        .acci  = -acci * (
                     (acci == MAHLER_SHARP && tone != MAHLER_B && tone != MAHLER_E) ||
                     (acci == MAHLER_FLAT && tone != MAHLER_C && tone != MAHLER_F)
                 ),
        .pitch = 0
    };
}

static inline bool
matchNote(struct Note const notes[], size_t noteNum, struct Note comp[], size_t compNum)
{
    size_t checkNote = noteNum;
    for (size_t s = 0; s < compNum; s++) {
        for (size_t c = 0; c < noteNum; c++) {
            if (comp[s].note == notes[c].note && comp[s].acci == notes[c].acci) {
                checkNote--;
                if (!checkNote) {
                    return true;
                }
            }
        }
    }
    return false;
}