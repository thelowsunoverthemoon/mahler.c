#ifndef __MAHLER_MISC_H__
#define __MAHLER_MISC_H__

#include "err/err.h"
#include "note/note.h"

char* printNote(struct Note const note, char buf[], size_t size, enum MahlerError* err);
bool isEnharmonic(struct Note const noteA, struct Note const noteB);

#endif