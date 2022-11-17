/*

| err.c |
defines error handling in mahler.c
defines the error macros used in functions

*/

#include "err.h"

// Functions //

char const*
getMahlerError(enum MahlerError err)
{
    switch (err) {
        case MAHLER_ERROR_NONE:
            return "None";
        case MAHLER_ERROR_INVALID_QUAL:
            return "Invalid Quality for Interval";
        case MAHLER_ERROR_INVALID_RANGE:
            return "Invalid Range for Interval";
        case MAHLER_ERROR_INVALID_INTER:
            return "Notes produce Invalid Interval";
        case MAHLER_ERROR_INVALID_INVERSION:
            return "Invalid Inversion for Chord";
        case MAHLER_ERROR_INVALID_PRINT_NOTE:
            return "Out of Range Note for Print Note";
        case MAHLER_ERROR_INVALID_PRINT_QUAL:
            return "Out of Range Acci for Print Note";
        case MAHLER_ERROR_OVERFLOW_PRINT_NOTE:
            return "Note Text is too Large";
        case MAHLER_ERROR_OVERFLOW_SCALE_RETURN:
            return "Too many Scale Return Results";
        case MAHLER_ERROR_OVERFLOW_CHORD_RETURN:
            return "Too many Chord Return Results";
        default:
            return "Unknown Error";
    }
}