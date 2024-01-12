/*

| inter.c |
Defines everything related to intervals
Defines global variable INTER_STEPS used by various functions

*/

#include "inter/inter.h"
#include "shared/shared.h"
#include <stdbool.h>

// Macros //

#define ADJUST_NOTE 7      // Size of note scale (enum mahler_note)
#define SIMPLE_INTER_MAX 8 // Simple Interval maximum
#define SEMI_DIFF 3        // adjust for semitone difference
#define TONE_DIFF 9        // adjust for tone (enum mahler_note) difference

// Global Variables //

static int const INTER_STEPS[] = {
    // Semitone distance between major intervals
    0, 2, 4, 5, 7, 9, 11, 12,
};

// Functions //

struct mah_note
mah_get_inter(struct mah_note const note, struct mah_interval const interval, enum mah_error* err)
{
    if (interval.steps <= 0)
    {
        RETURN_EMPTY_STRUCT_ERR(mah_note, MAH_ERROR_INVALID_RANGE);
    }
    int inter   = (interval.steps - 1) % ADJUST_NOTE;
    int pitch   = note.pitch + (interval.steps / SIMPLE_INTER_MAX);
    int root    = note.tone;
    int quality = interval.qual;

    int semi = 0;
    if (inter == 0 || inter == 3 || inter == 4 || inter == 7)
    { // if perfect interval
        switch (quality)
        { // special semitone adjustment for perfect interval
        case MAH_AUGMENTED:
            semi = 0;
            break;
        case MAH_DIMINISHED:
            semi = 1;
            break;
        case MAH_PERFECT:
            semi = -3;
            break;
        default:
            RETURN_EMPTY_STRUCT_ERR(mah_note, MAH_ERROR_INVALID_QUAL);
        }
    }
    else if (quality == MAH_PERFECT)
    {
        RETURN_EMPTY_STRUCT_ERR(mah_note, MAH_ERROR_INVALID_QUAL);
    }

    int enharmonic = root + inter; // find enharmonicically correct note by ignoring acci
    bool above     = false;
    if (enharmonic >= ADJUST_NOTE)
    {
        above = true;
        pitch += 1;
    }
    semi += to_semitone(root) + INTER_STEPS[inter] + note.acci + quality - 1;

    return (struct mah_note) {
        .tone  = enharmonic - (above * ADJUST_NOTE),
        .acci  = above ? semi - (to_semitone(enharmonic) - SEMI_DIFF) - (enharmonic <= TONE_DIFF)
                       : semi - (to_semitone(enharmonic) - 1), // 7 -> 9 is C to E (see toSemitone),
        .pitch = pitch,
    };
}

struct mah_interval
mah_return_inter(struct mah_note const note_a, struct mah_note const note_b, enum mah_error* err)
{

    int inter = (note_b.tone + note_b.pitch * ADJUST_NOTE) - (note_a.tone + note_a.pitch * ADJUST_NOTE) + 1;
    int pitch = inter > SIMPLE_INTER_MAX ? note_b.pitch - (inter / SIMPLE_INTER_MAX) : note_b.pitch;

    if (inter <= 0)
    {
        RETURN_EMPTY_STRUCT_ERR(mah_interval, MAH_ERROR_INVALID_RANGE);
    }

    int steps = (inter % ADJUST_NOTE) - 1; // - 1 since used for array indice of INTER_STEPS
    if (steps == -1)
    {
        steps = 6;
    }

    int quality = (to_semitone(note_b.tone) + note_b.acci + pitch * SIZE_CHROMATIC) -
                  (to_semitone(note_a.tone) + note_a.acci + note_a.pitch * SIZE_CHROMATIC) - INTER_STEPS[steps];

    if (quality >= 11 && quality <= 13)
    { // treated as 1 so must adjust (C0 12 = C1 0)
        quality -= SIZE_CHROMATIC;
    }
    else if (quality < -2 || quality > 1)
    { // between diminished and augmented
        RETURN_EMPTY_STRUCT_ERR(mah_interval, MAH_ERROR_INVALID_QUAL);
    }

    int chk_perf = inter % ADJUST_NOTE;
    if (chk_perf == 1 || chk_perf == 4 || chk_perf == 5)
    {
        if (quality == MAH_MAJOR)
        {
            quality = MAH_PERFECT;
        }
        else if (quality == MAH_MINOR)
        {
            quality = MAH_DIMINISHED;
        }
    }

    return (struct mah_interval) {
        .steps = inter,
        .qual  = quality,
    };
}