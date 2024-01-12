#ifndef __MAH_KEY_H__
#define __MAH_KEY_H__

#include "note/note.h"

// Enums //

typedef enum mah_key_type
{
    MAH_MAJOR_KEY,
    MAH_MINOR_KEY
} mah_key_type;

// Structures //

typedef struct mah_key_sig
{
    enum mah_key_type type;
    int alter;
    int size;
    struct mah_note key;
    struct mah_note notes[7];
} mah_key_sig;

// Functions //

struct mah_key_sig mah_get_key_sig(struct mah_note key, enum mah_key_type type);
struct mah_key_sig mah_return_key_sig(int alter, enum mah_key_type type);
struct mah_key_sig mah_get_key_relative(struct mah_key_sig const* key);
int mah_query_acci(struct mah_key_sig const* key, enum mah_tone note);

#endif