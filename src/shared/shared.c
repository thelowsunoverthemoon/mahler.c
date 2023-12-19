#include <stdlib.h>
#include "shared/shared.h"

// Macros //

#define SEMITONE_E 4 // semitone conversion of E
    
// Functions //

int
to_semitone(int tone)
{
    return tone * 2 + (tone <= MAH_E);  // "if" accounts for F (we convert from C, D, E, to C, C#, D, ect)
}

int
to_semitone_adj(struct mah_note note) {
    int semi = (note.tone * 2) - (note.tone > MAH_E) + note.acci;
    return constrain_semitone(semi);
}

int
constrain_semitone(int semi) {
    return semi < 0 ? SIZE_CHROMATIC - (abs(semi) % SIZE_CHROMATIC) : semi % SIZE_CHROMATIC; // constrain from 0 -> 11
}

struct mah_note
from_semitone(int semi) {
    return (struct mah_note) {
        .tone = (semi + (semi > SEMITONE_E)) / 2,
        .acci = semi % 2 == (semi <= SEMITONE_E)
    };
}

void
fill_semi_table(bool* semi, struct mah_note* notes, int size) {
    for (int i = 0; i < SIZE_CHROMATIC; i++) {
        semi[i] = false;
    }
    for (int n = 0; n < size; n++) {
        semi[to_semitone_adj(notes[n])] = true;
    }
}

bool
has_shifted_matches(struct mah_note const notes[], int num, bool* semi, int shift) {
    for (int n = 0; n < num; n++) { // instead of shifting scale up to match, shift semitones down
        if (!semi[constrain_semitone(to_semitone_adj(notes[n]) - shift)]) {
            return false;
        }
    }
    return true;
}

struct mah_note
get_enharmonic(struct mah_note note) // only works for sharps and flats
{
    enum mah_tone tone = note.tone;
    
    int enharmonic = tone + note.acci;
    if (enharmonic > MAH_B) {
        enharmonic = MAH_C;
    } else if (enharmonic < MAH_C) {
        enharmonic = MAH_B;
    }
    
    int acci = note.acci;
    return (struct mah_note) {
        .tone  = enharmonic,
        .acci  = -acci * (
                     (acci == MAH_SHARP && tone != MAH_B && tone != MAH_E) || // special cases for notes with no black keys between
                     (acci == MAH_FLAT && tone != MAH_C && tone != MAH_F)
                 )
    };
}