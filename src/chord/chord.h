#ifndef __MAH_CHD_H__
#define __MAH_CHD_H__

#include "err/err.h"
#include "note/note.h"
#include "inter/inter.h"
#include "shared/shared.h"

// Structures //

typedef struct mah_chord {
    int                       size; 
    int                       inv;
    struct mah_note* restrict base;
    struct mah_note* restrict notes;
} mah_chord;

typedef struct mah_chord_base {
    char const*          name;
    int                  size;
    struct mah_interval* steps;
} mah_chord_base;

typedef struct mah_chord_result {
    struct mah_note              key;
    struct mah_chord_base const* chord;
}  mah_chord_result;

typedef struct mah_chord_result_list {
    int                      max;
    int                      size;
    struct mah_chord_result* results;
} mah_chord_result_list;

typedef struct mah_chord_check {
    struct mah_chord_base const** pos;
    int                           size;
    struct mah_note* restrict     base;
    struct mah_note* restrict     notes;
    bool                          semi[SIZE_CHROMATIC];
} mah_chord_check;

// Preset Chords //

extern struct mah_chord_base const MAH_MAJOR_TRIAD;       // Major Triad
extern struct mah_chord_base const MAH_MINOR_TRIAD;       // Minor Triad
extern struct mah_chord_base const MAH_AUGMENTED_TRIAD;   // Augmented Triad
extern struct mah_chord_base const MAH_DIMINISHED_TRIAD;  // Diminished Triad
extern struct mah_chord_base const MAH_DIMINISHED_7;      // Diminished 7th
extern struct mah_chord_base const MAH_HALF_DIMINISHED_7; // Half Diminished 7th
extern struct mah_chord_base const MAH_MINOR_7;           // Minor 7th
extern struct mah_chord_base const MAH_MAJOR_7;           // Major 7th
extern struct mah_chord_base const MAH_DOMINANT_7;        // Dominant 7th

// Functions //

struct mah_chord mah_get_chord(struct mah_note root, struct mah_chord_base const* type, struct mah_note* restrict base, struct mah_note* restrict notes, enum mah_error* err);
void mah_return_chord(struct mah_note const notes[], int note, struct mah_chord_result_list* list, struct mah_chord_check* custom, enum mah_error* err);
void mah_invert_chord(struct mah_chord* chord, int inv, enum mah_error* err);

#endif