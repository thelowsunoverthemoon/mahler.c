/*

| key.c |
defines everything related to key signatures

*/

#include <stdlib.h>
#include "inter/inter.h"
#include "key.h"

// Macros //

#define KEYSIG_MAX 7    // Size of Circle of Fifths
#define KEY_FLAT_ADJ 2  // Adjust tblKey account for C and F for flats
#define KEY_SHARP_ADJ 1 // Adjust tblKey account for F for sharps
#define KEY_FLAT_ROOT 6 // Starting point for flats from sharps without F+ at tblKey
#define KEYSIG_DIF 3    // Difference of Parallel Major and Minor (G+ = 1, 1 - 3 = -2 = G-)

// Functions //

struct KeySig
getKeySig(struct Note const key, enum MahlerKeyType const type)
{
    static int const table[] = {
        0, 2, 4, -1, 1, 3, 5 // Major Key Acci Number
    };
    
    // +7 is sharp equiv, -7 is flat equiv, ect, minor equiv - 3
    return makeKeySig(key, table[key.note] + (KEYSIG_MAX * key.acci) - (type * KEYSIG_DIF), type);
}

struct KeySig
getKeyRelative(struct KeySig const* key)
{
    struct Note note = getInter(
        key->key,
        key->type == MAHLER_MINOR_KEY ? (struct Interval) {.inter = 3, .qual = MAHLER_MINOR} :
                                        (struct Interval) {.inter = 6, .qual = MAHLER_MAJOR},
        NULL
    );
    
    return getKeySig(note, key->type == MAHLER_MINOR_KEY ? MAHLER_MAJOR_KEY : MAHLER_MINOR_KEY);
    
}

struct KeySig
returnKeySig(int alter, enum MahlerKeyType const type)
{
    static enum MahlerNote const tblKey[] = {
        [0] = MAHLER_F,
        [1] = MAHLER_C,
        [2] = MAHLER_G,
        [3] = MAHLER_D,
        [4] = MAHLER_A,
        [5] = MAHLER_E,
        [6] = MAHLER_B,
    };
    
    struct Note key = {0};
    if (alter >= 0) {
        key = (struct Note) {
            .note  = tblKey[(alter + KEY_SHARP_ADJ) % KEYSIG_MAX], 
            .acci  = (alter + KEY_SHARP_ADJ) / KEYSIG_MAX,
        };
    } else {
        if (alter == -1) { // special case for F+
            key = (struct Note) {
                .note  = MAHLER_F
            };
        } else {
            key = (struct Note) {
                .note  = tblKey[(KEY_FLAT_ROOT + ((alter + KEY_FLAT_ADJ) % KEYSIG_MAX))], 
                .acci  = ((alter + KEY_FLAT_ADJ) / KEYSIG_MAX) - 1
            };
        }
    }
    
    return makeKeySig(
        type == MAHLER_MAJOR ? key : getInter(key, (struct Interval) {6, MAHLER_MAJOR}, NULL),
        alter, type
    );
}

int
queryAcci(struct KeySig const* key, enum MahlerNote const note)
{
    static int const lkpSharp[] = { // lkp = lookup
        [MAHLER_F] = 0,
        [MAHLER_C] = 1,
        [MAHLER_G] = 2,
        [MAHLER_D] = 3,
        [MAHLER_A] = 4,
        [MAHLER_E] = 5,
        [MAHLER_B] = 6
    };
    static int const lkpFlat[] = {
        [MAHLER_B] = 0,
        [MAHLER_E] = 1,
        [MAHLER_A] = 2,
        [MAHLER_D] = 3,
        [MAHLER_G] = 4,
        [MAHLER_C] = 5,
        [MAHLER_F] = 6
    };
    return key->notes[key->alter < 0 ? lkpFlat[note] : lkpSharp[note]].acci;
}

// Helper Functions //

static struct KeySig
makeKeySig(struct Note key, int alter, enum MahlerKeyType type)
{
    static enum MahlerNote const ordrSharp[] = {
        MAHLER_F, MAHLER_C, MAHLER_G, MAHLER_D, MAHLER_A, MAHLER_E, MAHLER_B
    };
    static enum MahlerNote const ordrFlat[] = {
        MAHLER_B, MAHLER_E, MAHLER_A, MAHLER_D, MAHLER_G, MAHLER_C, MAHLER_F
    };
    
    int noteSign = alter < 0 ? -1 : 1,
        noteAbs = alter * noteSign,
        acci = alter / KEYSIG_MAX;
    
    key.pitch = 0;
    struct KeySig new = {
        .key   = key,
        .type  = type,
        .alter = alter,
        .size  = noteAbs > KEYSIG_MAX ? KEYSIG_MAX : noteAbs
    };
    
    enum MahlerNote const* ordr = alter < MAHLER_NONE ? ordrFlat : ordrSharp;
    for (int i = 0; i < KEYSIG_MAX; i++) { // fill in base first
        new.notes[i] = (struct Note) {
            .note = ordr[i], .acci = acci, .pitch = 0
        };
    }

    for (int c = 0 ; c < noteAbs % KEYSIG_MAX; c++) { // do leftovers
        new.notes[c].acci += noteSign;
    }

    return new;
}