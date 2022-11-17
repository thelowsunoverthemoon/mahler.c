#ifndef __MAHLER_KEY_H__
#define __MAHLER_KEY_H__

#include "note/note.h"

// Enums //

enum MahlerKeyType {
    MAHLER_MAJOR_KEY, MAHLER_MINOR_KEY
};

// Structures //

struct KeySig {
    enum MahlerKeyType type;
    int                alter; // alteration
    int                size;
    struct Note        key;
    struct Note        notes[7];
};

// Functions //

struct KeySig getKeySig(struct Note key, enum MahlerKeyType type);
struct KeySig returnKeySig(int alter, enum MahlerKeyType type);
struct KeySig getKeyRelative(struct KeySig const* key);
int queryAcci(struct KeySig const* key, enum MahlerNote note);

static struct KeySig makeKeySig(struct Note key, int alter, enum MahlerKeyType type);

#endif