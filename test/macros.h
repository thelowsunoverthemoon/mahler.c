/*

    This is a simple framework for testing mahler.c
    There are various ASSERT macros to check the types of functions.
    The _ suffix represents the type it checks. See table below
    
    D   Default (two integer values)
    ST  Null terminated String
    N   mah_note
    I   mah_interval
    C   mah_chord
    S   mah_scale
    K   mah_key_sig
    CP  Chord for functions that alter them via pointer (2nd param is base mah_chord)
    CRL mah_chord_result_list for functions that alter them via pointer (2nd param is base list)
    SRL mah_scale_result_list for functions that alter them via pointer (2nd param is scale list)
    BC  Null terminated String for functions that take char[] (2nd param is the buf size)
    E   mah_error. No need for MAH prefix when writing the error
    
    So for example, "ASSERT_N" checks if the two notes match
    There are also global variables to assist in testing that are used in functions
    
    CHORD      mah_chord. Used in ASSERT_CP. The value is the 2nd param
    CHORD_LIST mah_chord_result_listhordResultList. Used in ASSERT_CRL. The value is the 2nd param
    SCALE_LIST mah_scale_result_listcaleResultList. Used in ASSERT_SCL. The value is the 2nd param
    ERR        mah_error
    BUF        char[]. Used in ASSERT_BC. The size is the 2nd param
    TEST       Used to keep track of passed/failed tests. Not used in any ASSERTs

    Macros are included to save typing. 1st, 2nd, ... refer to the nth parameter
    
    BUF_C        Creates char buffer of size 1st 
    NOTE         Creates note of note 1st, accidental 2nd, pitch 3rd. No need for MAH prefix in 1st
    NOTE_K       Takes in any number of notes to put as notes parameter of mah_key_sig
    NOTE_N       Creates mah_note buffer of size 1st, with the contents in 2nd
    NOTE_L       Creates empty mah_note buffer with any number of notes
    INTER        Creates mah_interval with interval 1st and quality 2nd. No need for MAH prefix in 2nd
    INTER_L      Creates mah_interval buffer with any number of intervals
    CHORD        Creates mah_chord with size 1st, inversion 2nd, base 3rd, and notes 4th
    CHORD_B      Creates &mah_chord_base with empty name, size 1st, and steps 2nd
    SCALE        Creates mah_scale with size 1st, type 2nd, notes 3rd.
    SCALE_B      Creates &mah_scale_base with empty name, size 1st, and steps 2nd
    KEY          Creates mah_key_sig with type 1st, alter 2nd, size 3rd, key 4th, notes 5th
    CHD_RES      Creates mah_chord_result with note 1st, base 2nd
    CHD_RES_L    Creates empty mah_chord_result buffer with size 1st
    CHD_RES_C    Creates mah_chord_result buffer with any number of notes
    CHD_RES_LIST Creates mah_chord_result_list with size max 1st, size 2nd, result 3rd
    SCL_RES      Creates mah_scale_result with note 1st, base 2nd
    SCL_RES_L    Creates empty mah_scale_result buffer with size 1st
    SCL_RES_C    Creates mah_scale_result buffer with any number of notes
    SCL_RES_LIST Creates mah_scale_result_list with size max 1st, size 2nd, result 3rd
    MAJOR_KEY    Same as MAH_MAJOR_KEY
    MINOR_KEY    Same as MAH_MINOR_KEY
    ASCEND       Same as MAH_ASCEND
    DESCEND      Same as MAH_DESCEND
    FULL         Same as MAH_FULL

*/

// Notes //

#define ASSERT(expr, mes) assert(expr, __LINE__, __FILE__, "Expected " #mes)
#define ASSERT_D(act, exp) ASSERT(act == exp, exp)
#define ASSERT_N(act, exp) ASSERT(comp_note(act, exp) && (ERR == MAH_ERROR_NONE), exp)
#define ASSERT_E(func, err) do {\
    func;\
    ASSERT(ERR == MAH_ ## err, MAH_ ## err);\
} while (0)
    
#define NOTE(note, acci, pitch) (struct mah_note) {MAH_ ## note, acci, pitch}
#define NOTE_K(...) {__VA_ARGS__}
#define NOTE_N(size, ...) (struct mah_note[size]) {__VA_ARGS__}
#define NOTE_L(...) (struct mah_note[]) {__VA_ARGS__}

// Intervals //

#define ASSERT_I(act, exp) ASSERT(comp_interval(act, exp) && (ERR == MAH_ERROR_NONE), exp)

#define INTER(inter, qual) (struct mah_interval) {inter, MAH_ ## qual}
#define INTER_L(...) (struct mah_interval[]) {__VA_ARGS__}

// Keys //

#define ASSERT_K(act, exp) ASSERT(comp_key_sig(act, exp) && (ERR == MAH_ERROR_NONE), exp)

#define KEY(type, alter, size, key, notes) (struct mah_key_sig) {type, alter, size, key, notes}

#define MAJOR_KEY MAH_MAJOR_KEY
#define MINOR_KEY MAH_MINOR_KEY

// Misc //

#define ASSERT_ST(act, exp) ASSERT(!strcmp(act, exp), exp)

#define ASSERT_BC(func, size, exp) {\
    char BUF[size] = {0};\
    func;\
    ASSERT_ST(BUF, exp);\
} while(0)

#define BUF_C(size) (char[size]) {0}

// Chord //

#define ASSERT_C(act, exp) ASSERT(comp_chord(act, exp) && (ERR == MAH_ERROR_NONE), exp)

#define ASSERT_CRL(func, crl, exp) {\
    struct mah_chord_result_list CHORD_LIST = crl;\
    func;\
    ASSERT(comp_chord_result_list(CHORD_LIST, exp) && (ERR == MAH_ERROR_NONE), exp);\
} while(0)
    
#define ASSERT_CP(func, chd, exp) {\
    struct mah_chord CHORD = chd;\
    func;\
    ASSERT(comp_chord(CHORD, exp) && (ERR == MAH_ERROR_NONE), exp);\
} while(0)

#define CHORD(size, inv, base, notes) (struct mah_chord) {size, inv, base, notes}
#define CHORD_B(size, steps) &(struct mah_chord_base) {"", size, steps}

#define CHD_RES(note, base) (struct mah_chord_result) {note, base}
#define CHD_RES_L(size) (struct mah_chord_result[size]) {0}
#define CHD_RES_C(...) (struct mah_chord_result[]) {__VA_ARGS__}
#define CHD_RES_LIST(max, size, results) (struct mah_chord_result_list) {max, size, results}

// Scale //

#define ASSERT_S(act, exp) ASSERT(comp_scale(act, exp) && (ERR == MAH_ERROR_NONE), exp)

#define ASSERT_SRL(func, srl, exp) {\
    struct mah_scale_result_list SCALE_LIST = srl;\
    func;\
    ASSERT(comp_scale_result_list(SCALE_LIST, exp) && (ERR == MAH_ERROR_NONE), exp);\
} while(0)
    
#define SCALE(size, type, notes) (struct mah_scale) {size, type, notes}
#define SCALE_B(size, steps) &(struct mah_scale_base) {"", size, steps}

#define SCL_RES(note, base) (struct mah_scale_result) {note, base}
#define SCL_RES_L(size) (struct mah_scale_result[size]) {0}
#define SCL_RES_C(...) (struct mah_scale_result[]) {__VA_ARGS__}
#define SCL_RES_LIST(max, size, results) (struct mah_scale_result_list) {max, size, results}

#define ASCEND MAH_ASCEND
#define DESCEND MAH_DESCEND
#define FULL MAH_FULL