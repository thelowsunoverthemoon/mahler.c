/*

| misc.c |
defines the functions for general use (not related to chords, scales, ect)

*/

#include "misc.h"

// Macros //

#define SIZE_CHROMATIC 12 // Size of chromatic scale

// Functions //

bool
isEnharmonic(struct Note const noteA, struct Note const noteB)
{
    return (toSemitone(noteA.note) + noteA.acci + noteA.pitch * SIZE_CHROMATIC) == (toSemitone(noteB.note) + noteB.acci + noteB.pitch * SIZE_CHROMATIC);
}

char*
printNote(struct Note const note, char buf[], size_t size, enum MahlerError* err)
{
    static char const* dispNote = "CDEFGAB";
    static char const* const dispAccidental[] = {"bbbb", "bbb", "bb", "b", "", "#", "##", "###", "####"};
    
    if (note.acci < -4 || note.acci > 4) { // dispAccidental limit
        SET_ERR(MAHLER_ERROR_INVALID_PRINT_QUAL);
        return "";
    }
    
    if (note.note < MAHLER_C || note.note > MAHLER_B) {
        SET_ERR(MAHLER_ERROR_INVALID_PRINT_NOTE);
        return "";
    }
    
    if (!(snprintf(buf, size, "%c%s%d", dispNote[note.note], dispAccidental[note.acci + 4], note.pitch) < size)) {
        SET_ERR(MAHLER_ERROR_OVERFLOW_PRINT_NOTE);
    }
    return buf;
}