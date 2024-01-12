/*

| scale.c |
Defines everything related to scales
Includes definition of preset scales

*/

#include "scale/scale.h"
#include "shared/shared.h"
#include <stdbool.h>

// Preset Scales //

struct mah_scale_base const MAH_MAJOR_SCALE = {
    "Major",
    8,
    (struct mah_interval[]) {
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
    },
};

struct mah_scale_base const MAH_NATURAL_MIN_SCALE = {
    "Natural Minor",
    8,
    (struct mah_interval[]) {
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
    },
};

struct mah_scale_base const MAH_HARMONIC_MIN_SCALE = {
    "Harmonic Minor",
    8,
    (struct mah_interval[]) {
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_AUGMENTED },
        { 2, MAH_MINOR },
    },
};

struct mah_scale_base const MAH_MELODIC_MIN_SCALE = {
    "Melodic Minor",
    8,
    (struct mah_interval[]) {
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
    },
};

struct mah_scale_base const MAH_PENTATONIC_MAJ_SCALE = {
    "Pentatonic Major",
    6,
    (struct mah_interval[]) {
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 3, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 3, MAH_MINOR },
    },
};

struct mah_scale_base const MAH_PENTATONIC_MIN_SCALE = {
    "Pentatonic Minor",
    6,
    (struct mah_interval[]) {
        { 3, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 3, MAH_MINOR },
        { 2, MAH_MAJOR },
    },
};

struct mah_scale_base const MAH_BLUES_SCALE = {
    "Blues",
    7,
    (struct mah_interval[]) {
        { 3, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 1, MAH_AUGMENTED },
        { 2, MAH_MINOR },
        { 3, MAH_MINOR },
        { 2, MAH_MAJOR },
    },
};

struct mah_scale_base const MAH_WHOLE_TONE_SCALE = {
    "Whole Tone",
    7,
    (struct mah_interval[]) {
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MAJOR },
        { 3, MAH_DIMINISHED },
    },
};

struct mah_scale_base const MAH_OCTATONIC_HALF_SCALE = {
    "Octatonic Half",
    9,
    (struct mah_interval[]) {
        { 1, MAH_AUGMENTED },
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_MAJOR },
    },
};

struct mah_scale_base const MAH_OCTATONIC_WHOLE_SCALE = {
    "Octatonic Whole",
    9,
    (struct mah_interval[]) {
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 1, MAH_AUGMENTED },
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
        { 2, MAH_MAJOR },
        { 2, MAH_MINOR },
    },
};

// Functions //

struct mah_scale
mah_get_scale(
    struct mah_note const start, struct mah_scale_base const* type, struct mah_note notes[],
    enum mah_scale_type const mode, enum mah_error* err
)
{
    int size     = type->size;
    int ret_size = size;
    switch (mode)
    {
    case MAH_DESCEND:
        notes[size - 1] = start;
        break;
    case MAH_ASCEND:
        notes[0] = start;
        break;
    case MAH_FULL:
        ret_size *= 2;
        notes[0] = notes[size * 2 - 1] = start; // set both ends to root
        break;
    }
    for (int i = 1; i < size; i++)
    {
        enum mah_error scale_err = MAH_ERROR_NONE;
        switch (mode)
        {
        case MAH_DESCEND:
            notes[size - i - 1] = mah_get_inter(notes[size - i], type->steps[i - 1], &scale_err);
            break;
        case MAH_ASCEND:
            notes[i] = mah_get_inter(notes[i - 1], type->steps[i - 1], &scale_err);
            break;
        case MAH_FULL:
            notes[i] = notes[size * 2 - i - 1] =
                mah_get_inter(notes[i - 1], type->steps[i - 1], &scale_err); // set both since symmetrical
            break;
        }
        if (scale_err != MAH_ERROR_NONE)
        {
            RETURN_EMPTY_STRUCT_ERR(mah_scale, scale_err);
        }
    }

    return (struct mah_scale) {
        .size  = ret_size,
        .type  = mode,
        .notes = notes,
    };
}

void
mah_return_scale(
    struct mah_note const notes[], int const num, struct mah_scale_result_list* list, struct mah_scale_check* custom,
    enum mah_error* err
)
{
    static struct mah_scale_base const* scale_pos[] = {
        &MAH_MAJOR_SCALE,
        &MAH_NATURAL_MIN_SCALE,
        &MAH_HARMONIC_MIN_SCALE,
        &MAH_MELODIC_MIN_SCALE,
    };
    struct mah_scale_check* scale_list = custom ? custom
                                                : &(struct mah_scale_check) {
                                                      .pos   = scale_pos,
                                                      .notes = (struct mah_note[8]) { 0 },
                                                      .size  = sizeof(scale_pos) / sizeof(*scale_pos),
                                                  };

    bool* semi = scale_list->semi;
    for (int s = 0; s < scale_list->size; s++)
    {
        if (scale_list->pos[s]->size - 1 < num)
        { // - 1 b/c size includes end note
            continue;
        }

        enum MahlerError scale_err = MAH_ERROR_NONE;
        struct mah_scale base      = mah_get_scale(
            (struct mah_note) {
                     .tone = MAH_C,
                     .acci = MAH_NATURAL,
            },
            scale_list->pos[s], scale_list->notes, MAH_ASCEND, &scale_err
        );
        if (scale_err != MAH_ERROR_NONE)
        {
            SET_ERR(scale_err);
            return;
        }

        fill_semi_table(semi, base.notes, base.size - 1);
        for (int d = 0; d < SIZE_CHROMATIC; d++)
        { // match every scale by semitone
            if (has_shifted_matches(notes, num, semi, d))
            {
                ADD_MATCHING_RESULT(MAH_ERROR_OVERFLOW_SCALE_RETURN, mah_scale_result, scale_list->pos[s]);
            }
        }
    }
}