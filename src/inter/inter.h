#ifndef __MAH_INTER_H__
#define __MAH_INTER_H__

#include "err/err.h"
#include "note/note.h"

// Enums //

typedef enum mah_quality
{
    MAH_MINOR      = -1,
    MAH_MAJOR      = 0,
    MAH_AUGMENTED  = 1,
    MAH_DIMINISHED = -2,
    MAH_PERFECT    = 3
} mah_quality;

// Structures //

typedef struct mah_interval
{
    int steps;
    enum mah_quality qual;
} mah_interval;

// Functions //

struct mah_note mah_get_inter(struct mah_note note, struct mah_interval interval, enum mah_error* err);
struct mah_interval mah_return_inter(struct mah_note note_a, struct mah_note note_b, enum mah_error* err);

#endif