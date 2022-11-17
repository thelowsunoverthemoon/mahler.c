#ifndef __MAHLER_ERR_H__
#define __MAHLER_ERR_H__

#include <stdio.h>
#include <stdlib.h>

// Macros //

#define SET_ERR(set) do {\
    if (err != NULL) {\
        *err = set;\
    }\
} while(0)

#define RETURN_EMPTY_STRUCT_ERR(type, set) do {\
    SET_ERR(set);\
    return (struct type) {0};\
} while(0)

#define RETURN_IF_OVERFLOW_ERR(err) {\
    if (list->size == list->max) {\
        SET_ERR(err);\
        return;\
    }\
} while(0)
    
// Enums //

enum MahlerError {
    MAHLER_ERROR_NONE,
    MAHLER_ERROR_INVALID_QUAL, MAHLER_ERROR_INVALID_RANGE, MAHLER_ERROR_INVALID_INTER,
    MAHLER_ERROR_INVALID_INVERSION, MAHLER_ERROR_INVALID_PRINT_NOTE, MAHLER_ERROR_INVALID_PRINT_QUAL,
    MAHLER_ERROR_OVERFLOW_PRINT_NOTE, MAHLER_ERROR_OVERFLOW_SCALE_RETURN, MAHLER_ERROR_OVERFLOW_CHORD_RETURN
};

// Functions //

char const* getMahlerError(enum MahlerError err);

#endif