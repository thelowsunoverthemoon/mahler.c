#ifndef __MAH_SCL_H__
#define __MAH_SCL_H__

#include "err/err.h"
#include "note/note.h"
#include "inter/inter.h"
#include "shared/shared.h"

// Enums //

enum mah_scale_type {
    MAH_ASCEND, MAH_DESCEND, MAH_FULL
};

// Structures //

typedef struct mah_scale {
    int                 size;
    enum mah_scale_type type;
    struct mah_note*    notes;
} mah_scale;

typedef struct mah_scale_base {
    char const*          name;
    int                  size;
    struct mah_interval* steps;
} mah_scale_base;

typedef struct mah_scale_result {
    struct mah_note              key;
    struct mah_scale_base const* scale;
} mah_scale_result;

typedef struct mah_scale_result_list {
    int                      max;
    int                      size;
    struct mah_scale_result* results;
} mah_scale_result_list;

typedef struct mah_scale_check {
    struct mah_scale_base const** pos;
    int                           size;
    struct mah_note*              notes;
    bool                          semi[SIZE_CHROMATIC];
} mah_scale_check;

// Predefined Scales //

extern struct mah_scale_base const MAH_MAJOR_SCALE;           // Major Scale
extern struct mah_scale_base const MAH_NATURAL_MIN_SCALE;     // Natural Minor Scale
extern struct mah_scale_base const MAH_HARMONIC_MIN_SCALE;    // Harmonic Minor Scale
extern struct mah_scale_base const MAH_MELODIC_MIN_SCALE;     // Melodic Minor Scale
extern struct mah_scale_base const MAH_PENTATONIC_MAJ_SCALE;  // Major Pentatonic Scale
extern struct mah_scale_base const MAH_PENTATONIC_MIN_SCALE;  // Minor Pentatonic Scale
extern struct mah_scale_base const MAH_BLUES_SCALE;           // Blues Scale (hexatonic)
extern struct mah_scale_base const MAH_WHOLE_TONE_SCALE;      // Whole Tone Scale
extern struct mah_scale_base const MAH_OCTATONIC_HALF_SCALE;  // Octatonic Scale (starting with half tone)
extern struct mah_scale_base const MAH_OCTATONIC_WHOLE_SCALE; // Octatonic Scale (starting with whole tone)

// Functions //

struct mah_scale mah_get_scale(struct mah_note start, struct mah_scale_base const* type, struct mah_note notes[], enum mah_scale_type mode, enum mah_error* err);
void mah_return_scale(struct mah_note const notes[], int num, struct mah_scale_result_list* list, struct mah_scale_check* custom, enum mah_error* err);

#endif