/*

| key.c |
Defines everything related to key signatures

*/

#include "key/key.h"
#include "inter/inter.h"

// Macros //

#define KEY_SIG_MAX 7   // Size of Circle of Fifths
#define KEY_FLAT_ADJ 2  // Adjust tblKey account for C and F for flats
#define KEY_SHARP_ADJ 1 // Adjust tblKey account for F for sharps
#define KEY_FLAT_ROOT 6 // Starting point for flats from sharps without F+ at tblKey
#define KEY_SIG_DIF 3   // Difference of Parallel Major and Minor (G+ = 1, 1 - 3 = -2 = G-)

// Internal Functions //

static struct mah_key_sig
make_key_sig(struct mah_note key, int const alter, enum mah_key_type const type)
{
    static enum mah_tone const ordr_sharp[] = {
        MAH_F, MAH_C, MAH_G, MAH_D, MAH_A, MAH_E, MAH_B
    };
    static enum mah_tone const ordr_flat[] = {
        MAH_B, MAH_E, MAH_A, MAH_D, MAH_G, MAH_C, MAH_F
    };
    
    int note_sign = alter < 0 ? -1 : 1,
        note_abs  = alter * note_sign,
        acci      = alter / KEY_SIG_MAX;
    
    key.pitch = 0;
    struct mah_key_sig new = {
        .key   = key,
        .type  = type,
        .alter = alter,
        .size  = note_abs > KEY_SIG_MAX ? KEY_SIG_MAX : note_abs // get overflow of accidentals
    };
    
    enum mah_tone const* ordr = alter < MAH_NATURAL ? ordr_flat : ordr_sharp;
    for (int i = 0; i < KEY_SIG_MAX; i++) { // fill in base first
        new.notes[i] = (struct mah_note) {
            .tone = ordr[i], .acci = acci
        };
    }

    for (int c = 0 ; c < note_abs % KEY_SIG_MAX; c++) { // do leftovers
        new.notes[c].acci += note_sign;
    }

    return new;
}

// Functions //

struct mah_key_sig
mah_get_key_sig(struct mah_note const key, enum mah_key_type const type)
{
    static int const table[] = {
        0, 2, 4, -1, 1, 3, 5 // Major Key Acci Number
    };
    
    // +7 is sharp equiv, -7 is flat equiv, ect, minor equiv - 3
    return make_key_sig(key, table[key.tone] + (KEY_SIG_MAX * key.acci) - (type * KEY_SIG_DIF), type);
}

struct mah_key_sig
mah_get_key_relative(struct mah_key_sig const* key)
{
    struct mah_note note = mah_get_inter(
        key->key,
        key->type == MAH_MINOR_KEY ? (struct mah_interval) {.steps = 3, .qual = MAH_MINOR} :
                                     (struct mah_interval) {.steps = 6, .qual = MAH_MAJOR},
        NULL
    );
    
    return mah_get_key_sig(note, key->type == MAH_MINOR_KEY ? MAH_MAJOR_KEY : MAH_MINOR_KEY);
    
}

struct mah_key_sig
mah_return_key_sig(int const alter, enum mah_key_type const type)
{
    static enum mah_tone const tbl_key[] = {
        [0] = MAH_F,
        [1] = MAH_C,
        [2] = MAH_G,
        [3] = MAH_D,
        [4] = MAH_A,
        [5] = MAH_E,
        [6] = MAH_B,
    };
    
    struct mah_note key = {0};
    if (alter >= 0) {
        key.tone  = tbl_key[(alter + KEY_SHARP_ADJ) % KEY_SIG_MAX]; 
        key.acci  = (alter + KEY_SHARP_ADJ) / KEY_SIG_MAX;
    } else {
        if (alter == -1) { // special case for F+
            key.tone  = MAH_F;
        } else {
            key.tone  = tbl_key[(KEY_FLAT_ROOT + ((alter + KEY_FLAT_ADJ) % KEY_SIG_MAX))]; 
            key.acci  = ((alter + KEY_FLAT_ADJ) / KEY_SIG_MAX) - 1;
        }
    }
    
    return make_key_sig(
        type == MAH_MAJOR ? key : mah_get_inter(key, (struct mah_interval) {6, MAH_MAJOR}, NULL),
        alter, type
    );
}

int
mah_query_acci(struct mah_key_sig const* key, enum mah_tone const note)
{
    static int const lkp_sharp[] = { // look up table for Circle of 5ths -> indices
        [MAH_F] = 0,
        [MAH_C] = 1,
        [MAH_G] = 2,
        [MAH_D] = 3,
        [MAH_A] = 4,
        [MAH_E] = 5,
        [MAH_B] = 6
    };
    static int const lkp_flat[] = {
        [MAH_B] = 0,
        [MAH_E] = 1,
        [MAH_A] = 2,
        [MAH_D] = 3,
        [MAH_G] = 4,
        [MAH_C] = 5,
        [MAH_F] = 6
    };
    return key->notes[key->alter < 0 ? lkp_flat[note] : lkp_sharp[note]].acci;
}