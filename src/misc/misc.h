#ifndef __MAH_MISC_H__
#define __MAH_MISC_H__

#include <stdbool.h>
#include "err/err.h"
#include "note/note.h"

char* mah_write_note(struct mah_note note, char buf[], size_t size, enum mah_error* err);
bool mah_is_enharmonic(struct mah_note note_a, struct mah_note note_b);

#endif