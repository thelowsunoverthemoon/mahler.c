/*

| err.c |
Defines error handling in mahler.c
Defines the error macros used in functions

*/

#include "err/err.h"

// Functions //

char const*
mah_get_error(enum mah_error const err)
{
    switch (err) {
        case MAH_ERROR_NONE:
            return "None";
        case MAH_ERROR_INVALID_QUAL:
            return "Invalid Quality for Interval";
        case MAH_ERROR_INVALID_RANGE:
            return "Invalid Range for Interval";
        case MAH_ERROR_INVALID_INTER:
            return "Notes produce Invalid Interval";
        case MAH_ERROR_INVALID_INVERSION:
            return "Invalid Inversion for Chord";
        case MAH_ERROR_INVALID_PRINT_NOTE:
            return "Out of Range Note for Print Note";
        case MAH_ERROR_INVALID_PRINT_QUAL:
            return "Out of Range Acci for Print Note";
        case MAH_ERROR_OVERFLOW_PRINT_NOTE:
            return "Note Text is too Large";
        case MAH_ERROR_OVERFLOW_SCALE_RETURN:
            return "Too many Scale Return Results";
        case MAH_ERROR_OVERFLOW_CHORD_RETURN:
            return "Too many Chord Return Results";
        default:
            return "Unknown Error";
    }
}