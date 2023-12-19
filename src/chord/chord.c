/*

| chord.c |
Defines everything related to chords
Includes definition of preset chords

*/

#include <string.h>
#include "chord/chord.h"

// Preset Chords //

struct mah_chord_base const MAH_MAJOR_TRIAD = {
    "Major Triad", 3, (struct mah_interval[]) {{3, MAH_MAJOR},{3, MAH_MINOR}}
};

struct mah_chord_base const MAH_MINOR_TRIAD = {
    "Minor Triad", 3, (struct mah_interval[]) {{3, MAH_MINOR},{3, MAH_MAJOR}}
};

struct mah_chord_base const MAH_AUGMENTED_TRIAD = {
    "Augmented Triad", 3, (struct mah_interval[]) {{3, MAH_MAJOR},{3, MAH_MAJOR}}
};

struct mah_chord_base const MAH_DIMINISHED_TRIAD = {
    "Diminished Triad", 3, (struct mah_interval[]) {{3, MAH_MINOR},{3, MAH_MINOR}}
};

struct mah_chord_base const MAH_DIMINISHED_7 = {
    "Diminished 7th", 4, (struct mah_interval[]) {{3, MAH_MINOR},{3, MAH_MINOR},{3, MAH_MINOR}}
};

struct mah_chord_base const MAH_HALF_DIMINISHED_7 = {
    "Half-Diminished 7th", 4, (struct mah_interval[]) {{3, MAH_MINOR},{3, MAH_MINOR},{3, MAH_MAJOR}}
};

struct mah_chord_base const MAH_MINOR_7 = {
    "Minor 7th", 4, (struct mah_interval[]) {{3, MAH_MINOR},{3, MAH_MAJOR},{3, MAH_MINOR}}
};

struct mah_chord_base const MAH_MAJOR_7 = {
    "Major 7th", 4, (struct mah_interval[]) {{3, MAH_MAJOR},{3, MAH_MINOR},{3, MAH_MAJOR}}
};

struct mah_chord_base const MAH_DOMINANT_7 = {
    "Dominant 7th", 4, (struct mah_interval[]) {{3, MAH_MAJOR},{3, MAH_MINOR},{3, MAH_MINOR}}
};

// Functions //

struct mah_chord
mah_get_chord(struct mah_note const root, struct mah_chord_base const* type, struct mah_note* restrict base, struct mah_note* restrict notes, enum mah_error* err)
{
    notes[0] = base[0] = root;
    
    for (int i = 1; i < type->size; i++) {
        enum mah_error inter_err = MAH_ERROR_NONE;
        notes[i] = base[i] = mah_get_inter(notes[i - 1], type->steps[i - 1], &inter_err); // - 1 b/c interval starts at note before
        if (inter_err != MAH_ERROR_NONE) {
            RETURN_EMPTY_STRUCT_ERR(mah_chord, inter_err);
        }
    }
    
    return (struct mah_chord) {
        .size  = type->size,
        .inv   = 0,
        .base  = base,
        .notes = notes
    };
}

void
mah_invert_chord(struct mah_chord* chord, int const inv, enum mah_error* err)
{
    if (inv >= chord->size || inv < 0) {
        SET_ERR(MAH_ERROR_INVALID_INVERSION);
        return;
    }
    memcpy(chord->notes, chord->base, sizeof(struct mah_note) * chord->size); // reset to base before inversion
    
    for (size_t j = 0; j < inv; j++) {
        struct mah_note temp = chord->notes[0]; // save root since only root needs to "loop" to last place
        temp.pitch += 1;
        for (size_t i = 0; i < chord->size - 1; i++) {
            chord->notes[i] = chord->notes[i + 1];
        }
        chord->notes[chord->size - 1] = temp;
    }
    
    chord->inv = inv;
    return;
}

void
mah_return_chord(struct mah_note const notes[], int const num, struct mah_chord_result_list* list, struct mah_chord_check* custom, enum mah_error* err)
{
    static struct mah_chord_base const* chord_pos[] = {
        &MAH_MAJOR_TRIAD,
        &MAH_MINOR_TRIAD,
        &MAH_AUGMENTED_TRIAD,
        &MAH_DIMINISHED_TRIAD,
        &MAH_DIMINISHED_7,
        &MAH_DOMINANT_7
    };
    struct mah_chord_check* chord_list = custom ? custom : &(struct mah_chord_check) {
        .pos   = chord_pos,
        .notes = (struct mah_note[4]) {0},
        .base  = (struct mah_note[4]) {0},
        .size  = sizeof(chord_pos) / sizeof(*chord_pos)
    };
    
    bool* semi = chord_list->semi;
    for (int s = 0; s < chord_list->size; s++) {
        if (chord_list->pos[s]->size < num) {
            continue;
        }
        
        enum mah_error chord_err = MAH_ERROR_NONE;
        struct mah_chord base = mah_get_chord(
            (struct mah_note) {
                .tone = MAH_C, .acci = MAH_NATURAL
            }, chord_list->pos[s], chord_list->base, chord_list->notes, &chord_err
        );
        if (chord_err != MAH_ERROR_NONE) {
            SET_ERR(chord_err);
            return;
        }

        fill_semi_table(semi, base.notes, base.size);
        for (int d = 0; d < SIZE_CHROMATIC; d++) { // match every scale by semitone
            if (has_shifted_matches(notes, num, semi, d)) {
                ADD_MATCHING_RESULT(MAH_ERROR_OVERFLOW_CHORD_RETURN, mah_chord_result, chord_list->pos[s]);
            }
        }
        
    }
}

