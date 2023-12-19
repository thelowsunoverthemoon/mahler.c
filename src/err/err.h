#ifndef __MAH_ERR_H__
#define __MAH_ERR_H__

#include <stddef.h>

// Enums //

typedef enum mah_error {
    MAH_ERROR_NONE,
    MAH_ERROR_INVALID_QUAL, MAH_ERROR_INVALID_RANGE, MAH_ERROR_INVALID_INTER,
    MAH_ERROR_INVALID_INVERSION, MAH_ERROR_INVALID_PRINT_NOTE, MAH_ERROR_INVALID_PRINT_QUAL,
    MAH_ERROR_OVERFLOW_PRINT_NOTE, MAH_ERROR_OVERFLOW_SCALE_RETURN, MAH_ERROR_OVERFLOW_CHORD_RETURN
} mah_error;

// Functions //

char const* mah_get_error(enum mah_error err);

#endif