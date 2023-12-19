/*

| misc.c |
Defines the functions for general use (not related to chords, scales, ect)

*/

#include <stdio.h>
#include "misc/misc.h"
#include "shared/shared.h"

// Functions //

char*
mah_write_note(struct mah_note const note, char buf[], size_t const size, enum mah_error* err)
{
    static char const* disp_note = "CDEFGAB";
    static char const* const disp_acci[] = {"bbbb", "bbb", "bb", "b", "", "#", "##", "###", "####"};
    
    if (note.acci < -4 || note.acci > 4) { // dispAccidental limit
        SET_ERR(MAH_ERROR_INVALID_PRINT_QUAL);
        return "";
    }
    
    if (note.tone < MAH_C || note.tone > MAH_B) {
        SET_ERR(MAH_ERROR_INVALID_PRINT_NOTE);
        return "";
    }
    
    if (!(snprintf(buf, size, "%c%s%d", disp_note[note.tone], disp_acci[note.acci + 4], note.pitch) < size)) {
        SET_ERR(MAH_ERROR_OVERFLOW_PRINT_NOTE);
    }
    return buf;
}

bool
mah_is_enharmonic(struct mah_note const note_a, struct mah_note const note_b)
{
    return (to_semitone(note_a.tone) + note_a.acci + note_a.pitch * SIZE_CHROMATIC) ==
           (to_semitone(note_b.tone) + note_b.acci + note_b.pitch * SIZE_CHROMATIC);
}