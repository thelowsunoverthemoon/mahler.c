#ifndef __MAH_SHARED_H__
#define __MAH_SHARED_H__

#include <stdbool.h>
#include "err/err.h"
#include "note/note.h"

// Macros //

#define SIZE_CHROMATIC 12 // size of chromatic scale

// Adds enharmonic Result entries to list for return functions
#define ADD_MATCHING_RESULT(err, type, match) {\
    struct mah_note ret = from_semitone(d);\
    RETURN_IF_OVERFLOW_ERR(err);\
    list->results[list->size++] = (struct type) {ret, match};\
    if (ret.acci != MAH_NATURAL) {\
        RETURN_IF_OVERFLOW_ERR(err);\
        list->results[list->size++] = (struct type) {get_enharmonic(ret), match};\
    }\
} while(0)

// Sets error if not null
#define SET_ERR(set) do {\
    if (err != NULL) {\
        *err = set;\
    }\
} while(0)

// Sets error if not null then returns empty structure
#define RETURN_EMPTY_STRUCT_ERR(type, set) do {\
    SET_ERR(set);\
    return (struct type) {0};\
} while(0)

// Sets error if not null and list will overflow
#define RETURN_IF_OVERFLOW_ERR(err) {\
    if (list->size == list->max) {\
        SET_ERR(err);\
        return;\
    }\
} while(0)
    
// Functions //

int constrain_semitone(int semi);
int to_semitone(int tone);
struct mah_note from_semitone(int semi);
int to_semitone_adj(struct mah_note note);
struct mah_note get_enharmonic(struct mah_note note);
void fill_semi_table(bool* semi, struct mah_note* notes, int size);
bool has_shifted_matches(struct mah_note const notes[], int num, bool* semi, int shift);

#endif