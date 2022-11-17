/*

| inter.c |
defines everything related to intervals
defines global variable INTER_STEPS used by various functions

*/

#include <stdbool.h>
#include "inter.h"

// Macros //

#define ADJUST_NOTE 7      // Size of note scale (enum MahlerNote)
#define SIMPLE_INTER_MAX 8 // Simple Interval maximum
#define SIZE_CHROMATIC 12  // Size of chromatic scale

// INTER_STEPS is amount of steps for major interval

static int const INTER_STEPS[] = {
    0, 2, 4, 5, 7, 9, 11, 12
};

// Functions //

struct Note
getInter(struct Note const note, struct Interval const interval, enum MahlerError* err)
{
    if (interval.inter <= 0) {
        RETURN_EMPTY_STRUCT_ERR(Note, MAHLER_ERROR_INVALID_RANGE);
    }
    int inter = (interval.inter - 1) % ADJUST_NOTE,
        pitch = note.pitch + (interval.inter / SIMPLE_INTER_MAX),
        root = note.note,
        quality = interval.qual;

    int end = 0;
    if (inter == 0 || inter == 3 || inter == 4 || inter == 7) { // if perfect interval
        switch (quality) {
            case MAHLER_AUGMENTED:
                end = 0;
                break;
            case MAHLER_DIMINISHED:
                end = 1;
                break;
            case MAHLER_PERFECT:
                end = -3;
                break;
            default:
                RETURN_EMPTY_STRUCT_ERR(Note, MAHLER_ERROR_INVALID_QUAL);
        }
    } else if (quality == MAHLER_PERFECT) {
        RETURN_EMPTY_STRUCT_ERR(Note, MAHLER_ERROR_INVALID_QUAL);
    }
    
    int enharmonic = root + inter; // finding correct "note" with enum
    bool above = false;
    if (enharmonic >= ADJUST_NOTE) {
        above = true;
        pitch += 1;
    }
    end += toSemitone(root) + INTER_STEPS[inter] + note.acci + quality - 1;

    return (struct Note) {
        .note  = enharmonic - (above * ADJUST_NOTE),
        .acci  = above ? end - (toSemitone(enharmonic) - 3) - (enharmonic <= 9) :
                         end - (toSemitone(enharmonic) - 1), // 7 -> 9 is C to E (see toSemitone),
        .pitch = pitch
    };
}

struct Interval
returnInter(struct Note const noteA, struct Note const noteB, enum MahlerError* err)
{

    int inter = (noteB.note + noteB.pitch * ADJUST_NOTE) - (noteA.note + noteA.pitch * ADJUST_NOTE) + 1;
    int pitch = inter > SIMPLE_INTER_MAX ? noteB.pitch - (inter / SIMPLE_INTER_MAX) : noteB.pitch;
    
    if (inter <= 0) {
        RETURN_EMPTY_STRUCT_ERR(Interval, MAHLER_ERROR_INVALID_RANGE);
    }
    
    int steps = (inter % ADJUST_NOTE) - 1;
    if (steps == -1) {
        steps = 6;
    }
    
    int quality = (toSemitone(noteB.note) + noteB.acci + pitch * SIZE_CHROMATIC) -
                  (toSemitone(noteA.note) + noteA.acci + noteA.pitch * SIZE_CHROMATIC) -
                  INTER_STEPS[steps];

    if (quality >= 11 && quality <= 13) { // treated as 1 so must adjust (C0 12 = C1 0)
        quality -= 12;
    } else if (quality < -2 || quality > 1) {
        RETURN_EMPTY_STRUCT_ERR(Interval, MAHLER_ERROR_INVALID_QUAL);
    }
    
    int chkPerfect = inter % ADJUST_NOTE;
    if (chkPerfect == 1 || chkPerfect == 4 || chkPerfect == 5) {
        if (quality == MAHLER_MAJOR) {
            quality = MAHLER_PERFECT;
        } else if (quality == MAHLER_MINOR) {
            quality = MAHLER_DIMINISHED;
        }
    }

    return (struct Interval) {
        .inter   = inter,
        .qual = quality
    };
}