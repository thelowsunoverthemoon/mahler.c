#include <stdio.h>
#include <stdbool.h>
#include "mahler.h"

/*

    This is a simple framework for testing mahler.c
    There are various ASSERT macros to check the types of functions.
    The _ suffix represents the type it checks. See table below
    
    D   Default (two integer values)
    ST  Null terminated String
    N   Note
    I   Interval
    C   Chord
    S   Scale
    K   KeySig
    CP  Chord for functions that alter them via pointer (2nd param is base Chord)
    CRL ChordResultList for functions that alter them via pointer (2nd param is base list)
    SRL ScaleResultList for functions that alter them via pointer (2nd param is scale list)
    BC  Null terminated String for functions that take char[] (2nd param is the buf size)
    E   MahlerError. No need for MAHLER prefix when writing the error
    
    So for example, "ASSERT_N" checks if the two notes match
    There are also global variables to assist in testing that are used in functions
    
    CHORD      Chord. Used in ASSERT_CP. The value is the 2nd param
    CHORD_LIST ChordResultList. Used in ASSERT_CRL. The value is the 2nd param
    SCALE_LIST ScaleResultList. Used in ASSERT_SCL. The value is the 2nd param
    ERR        MahlerError
    BUF        char[]. Used in ASSERT_BC. The size is the 2nd param
    TEST       Used to keep track of passed/failed tests. Not used in any ASSERTs

    Macros are included to save typing. 1st, 2nd, ... refer to the nth parameter
    
    BUF_C        Creates char buffer of size 1st 
    NOTE         Creates note of note 1st, accidental 2nd, pitch 3rd. No need for MAHLER prefix in 1st
    NOTE_K       Takes in any number of notes to put as notes parameter of KeySig
    NOTE_N       Creates Note buffer of size 1st, with the contents in 2nd
    NOTE_L       Creates empty Note buffer with any number of notes
    INTER        Creates Interval with interval 1st and quality 2nd. No need for MAHLER prefix in 2nd
    INTER_L      Creates Interval buffer with any number of intervals
    CHORD        Creates Chord with size 1st, inversion 2nd, base 3rd, and notes 4th
    CHORD_B      Creates &ChordBase with empty name, size 1st, and steps 2nd
    SCALE        Creates Scale with size 1st, type 2nd, notes 3rd.
    SCALE_B      Creates &ScaleBase with empty name, size 1st, and steps 2nd
    KEY          Creates KeySig with type 1st, alter 2nd, size 3rd, key 4th, notes 5th
    CHD_RES      Creates chord result with note 1st, base 2nd
    CHD_RES_L    Creates empty ChordResult buffer with size 1st
    CHD_RES_C    Creates ChordResult buffer with any number of notes
    CHD_RES_LIST Creates ChordResultList with size max 1st, size 2nd, result 3rd
    SCL_RES      Creates chord result with note 1st, base 2nd
    SCL_RES_L    Creates empty ChordResult buffer with size 1st
    SCL_RES_C    Creates ChordResult buffer with any number of notes
    SCL_RES_LIST Creates ChordResultList with size max 1st, size 2nd, result 3rd
    MAJOR_KEY    Same as MAHLER_MAJOR_KEY
    MINOR_KEY    Same as MAHLER_MINOR_KEY
    ASCEND       Same as MAHLER_ASCEND
    DESCEND      Same as MAHLER_DESCEND
    FULL         Same as MAHLER_FULL

*/

#define ASSERT(expr, mes) assert(expr, __LINE__, __FILE__, "Expected " #mes)
#define ASSERT_D(act, exp) ASSERT(act == exp, exp)
#define ASSERT_N(act, exp) ASSERT(compNote(act, exp) && (ERR == MAHLER_ERROR_NONE), exp)

#define ASSERT_E(func, err) do {\
    func;\
    ASSERT(ERR == MAHLER_ ## err, MAHLER_ ## err);\
} while (0)
    
#define NOTE(note, acci, pitch) (struct Note) {MAHLER_ ## note, acci, pitch}
#define NOTE_K(...) {__VA_ARGS__}
#define NOTE_N(size, ...) (struct Note[size]) {__VA_ARGS__}
#define NOTE_L(...) (struct Note[]) {__VA_ARGS__}

struct Test {
    int testPass;
    int testTotal;
};

struct Test TEST = {0};
enum MahlerError ERR = MAHLER_ERROR_NONE;

void runAllTests();
void assert(int val, int line, char* file, char* expr);

bool compNote(struct Note noteA, struct Note noteB);
bool compNotes(struct Note const* notesA, struct Note const* notesB, size_t sizeA, size_t sizeB);

#include "inter_test.c"
#include "chord_test.c"
#include "scale_test.c"
#include "key_test.c"
#include "misc_test.c"

int
main(void)
{

    runAllTests();
    printf("%d / %d Tests Passed", TEST.testPass, TEST.testTotal);
    
}

void
runAllTests()
{

    testGetInter();
    testReturnInter();

    testGetChord();
    testInvertChord();
    testReturnChord();
    
    testGetScale();
    testReturnScale();
    
    testGetKeySig();
    testReturnKeySig();
    testGetKeyRelative();
    testQueryAcci();
    
    testIsEnharmonic();
    testPrintNote();
    testGetMahlerError();
    
}

void
assert(int val, int line, char* file, char* expr)
{
    TEST.testTotal++;
    if (val) {
        TEST.testPass++;
    } else {
        fprintf(stderr, "%s @ LINE %d | %s\n", file, line, expr);
    }
    ERR = MAHLER_ERROR_NONE;
}

bool
compNote(struct Note noteA, struct Note noteB)
{
    return noteA.note == noteB.note &&
           noteA.acci == noteB.acci &&
           noteA.pitch == noteB.pitch;
}

bool
compNotes(struct Note const* notesA, struct Note const* notesB, size_t sizeA, size_t sizeB) {
    if (sizeA != sizeB) {
        return false;
    }

    for (size_t i = 0; i < sizeA; i++) {
        if (!compNote(notesA[i], notesB[i])) {
            return false;
        }
    }
    return true;
}