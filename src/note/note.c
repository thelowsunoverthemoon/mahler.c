/*

| note.c |
defines everything related to notes

*/

#include "note.h"

// Functions //

inline int
toSemitone(int const note)
{
    return note * 2 + (note <= MAHLER_E);  // "if" accounts for F (we convert from C, D, E, to C, C#, D, ect)
}

inline struct Note
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

inline bool
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