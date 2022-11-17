#define ASSERT_C(act, exp) ASSERT(compChord(act, exp) && (ERR == MAHLER_ERROR_NONE), exp)

#define ASSERT_CRL(func, crl, exp) {\
    struct ChordResultList CHORD_LIST = crl;\
    func;\
    ASSERT(compChordResultList(CHORD_LIST, exp) && (ERR == MAHLER_ERROR_NONE), exp);\
} while(0)
    
#define ASSERT_CP(func, chd, exp) {\
    struct Chord CHORD = chd;\
    func;\
    ASSERT(compChord(CHORD, exp) && (ERR == MAHLER_ERROR_NONE), exp);\
} while(0)

#define CHORD(size, inv, base, notes) (struct Chord) {size, inv, base, notes}
#define CHORD_B(size, steps) &(struct ChordBase) {"", size, steps}

#define CHD_RES(note, base) (struct ChordResult) {note, base}
#define CHD_RES_L(size) (struct ChordResult[size]) {0}
#define CHD_RES_C(...) (struct ChordResult[]) {__VA_ARGS__}
#define CHD_RES_LIST(max, size, results) (struct ChordResultList) {max, size, results}

void testGetChord();
void testInvertChord();
void testReturnChord();

bool compChord(struct Chord chordA, struct Chord chordB);
bool compChordResultList(struct ChordResultList listA, struct ChordResultList listB);
bool compChordResult(struct ChordResult resultA, struct ChordResult resultB);
bool compChordResults(struct ChordResult* resultsA, struct ChordResult* resultsB, size_t sizeA, size_t sizeB);
bool compChordBase(struct ChordBase const* baseA, struct ChordBase const* baseB);

void
testGetChord()
{
   
    ASSERT_E(getChord(NOTE(E, 2, 9), CHORD_B(3, INTER_L(INTER(3, MAJOR), INTER(5, MAJOR))), NOTE_N(3, 0), NOTE_N(3, 0), &ERR), ERROR_INVALID_QUAL);
    ASSERT_C(getChord(NOTE(F, -1, 4), &MAHLER_DOMINANT_7, NOTE_N(4, 0), NOTE_N(4, 0), &ERR), CHORD(
        4, 0,
        NOTE_L(NOTE(F, -1, 4), NOTE(A, -1, 4), NOTE(C, -1, 5), NOTE(E, -2, 5)),
        NOTE_L(NOTE(F, -1, 4), NOTE(A, -1, 4), NOTE(C, -1, 5), NOTE(E, -2, 5))
    ));
    ASSERT_C(getChord(NOTE(D, 0, 9), &MAHLER_MAJOR_TRIAD, NOTE_N(3, 0), NOTE_N(3, 0), &ERR), CHORD(
        3, 0,
        NOTE_L(NOTE(D, 0, 9), NOTE(F, 1, 9), NOTE(A, 0, 9)),
        NOTE_L(NOTE(D, 0, 9), NOTE(F, 1, 9), NOTE(A, 0, 9))
    ));
    ASSERT_C(getChord(NOTE(C, 3, 1), CHORD_B(2, INTER_L(INTER(1, PERFECT))), NOTE_N(2, 0), NOTE_N(2, 0), &ERR), CHORD(
        2, 0,
        NOTE_L(NOTE(C, 3, 1), NOTE(C, 3, 1)),
        NOTE_L(NOTE(C, 3, 1), NOTE(C, 3, 1))
    ));
    
}

void
testInvertChord()
{
    ASSERT_E(invertChord(&CHORD(
        4, 0,
        NOTE_L(NOTE(E, 1, 1), NOTE(G, -1, 1), NOTE(B, 0, 1), NOTE(D, 1, 2)),
        NOTE_L(NOTE(E, 1, 1), NOTE(G, -1, 1), NOTE(B, 0, 1), NOTE(D, 1, 2))
    ), -1, &ERR), ERROR_INVALID_INVERSION);
    ASSERT_E(invertChord(&CHORD(
        2, 0,
        NOTE_L(NOTE(A, 2, 1), NOTE(G, 2, 1)),
        NOTE_L(NOTE(A, 2, 1), NOTE(G, 2, 1))
    ), 3, &ERR), ERROR_INVALID_INVERSION);
    ASSERT_CP(invertChord(&CHORD, 0, &ERR), CHORD(
        3, 0,
        NOTE_L(NOTE(D, 3, 1), NOTE(F, 1, 1), NOTE(A, 2, 1)),
        NOTE_L(NOTE(D, 3, 1), NOTE(F, 1, 1), NOTE(A, 2, 1))
    ), CHORD(
        3, 0,
        NOTE_L(NOTE(D, 3, 1), NOTE(F, 1, 1), NOTE(A, 2, 1)),
        NOTE_L(NOTE(D, 3, 1), NOTE(F, 1, 1), NOTE(A, 2, 1))
    ));
    ASSERT_CP(invertChord(&CHORD, 2, &ERR), CHORD(
        3, 0,
        NOTE_L(NOTE(B, -1, 2), NOTE(D, 1, 2), NOTE(G, 3, 3)),
        NOTE_L(NOTE(B, -1, 2), NOTE(D, 1, 2), NOTE(G, 3, 3))
    ), CHORD(
        3, 2,
        NOTE_L(NOTE(B, -1, 2), NOTE(D, 1, 2), NOTE(G, 3, 3)),
        NOTE_L(NOTE(G, 3, 3), NOTE(B, -1, 3), NOTE(D, 1, 3))
    ));
    ASSERT_CP(invertChord(&CHORD, 0, &ERR), CHORD(
        3, 1,
        NOTE_L(NOTE(A, -1, 0), NOTE(C, 1, 1), NOTE(E, 3, 1)),
        NOTE_L(NOTE(C, 1, 1), NOTE(E, 3, 1), NOTE(A, -1, 1))
    ), CHORD(
        3, 0,
        NOTE_L(NOTE(A, -1, 0), NOTE(C, 1, 1), NOTE(E, 3, 1)),
        NOTE_L(NOTE(A, -1, 0), NOTE(C, 1, 1), NOTE(E, 3, 1))
    ));
    
}

void
testReturnChord()
{
    ASSERT_E(returnChord(NOTE_L(NOTE(A, 0, 0)), 1, &CHD_RES_LIST(5, 0, CHD_RES_L(5)), NULL, false, &ERR), ERROR_OVERFLOW_CHORD_RETURN);
    ASSERT_CRL(returnChord(NOTE_L(NOTE(A, 0, 0), NOTE(C, 1, 1)), 2, &CHORD_LIST, NULL, false, &ERR),
        CHD_RES_LIST(5, 0, CHD_RES_L(5)),
        CHD_RES_LIST(5, 5, CHD_RES_C(
            CHD_RES(NOTE(A, 0, 0), &MAHLER_MAJOR_TRIAD),
            CHD_RES(NOTE(F, 1, 0), &MAHLER_MINOR_TRIAD),
            CHD_RES(NOTE(F, 0, 0), &MAHLER_AUGMENTED_TRIAD),
            CHD_RES(NOTE(A, 0, 0), &MAHLER_AUGMENTED_TRIAD),
            CHD_RES(NOTE(A, 0, 0), &MAHLER_DOMINANT_7)
        ))
    );
    ASSERT_CRL(returnChord(NOTE_L(NOTE(A, 0, 0), NOTE(G, 1, 1)), 2, &CHORD_LIST, NULL, false, &ERR),
        CHD_RES_LIST(2, 0, CHD_RES_L(2)),
        CHD_RES_LIST(2, 0, CHD_RES_C(
            0
        ))
    );
    ASSERT_CRL(returnChord(NOTE_L(NOTE(A, 0, 0), NOTE(F, 0, 0), NOTE(C, 0, 0), NOTE(E, -1, 1)), 4, &CHORD_LIST, NULL, false, &ERR),
        CHD_RES_LIST(2, 0, CHD_RES_L(2)),
        CHD_RES_LIST(2, 1, CHD_RES_C(
            CHD_RES(NOTE(F, 0, 0), &MAHLER_DOMINANT_7)
        ))
    );
    ASSERT_CRL(returnChord(NOTE_L(NOTE(D, 1, 0), NOTE(G, 1, 0), NOTE(B, 0, 1)), 3, &CHORD_LIST, NULL, true, &ERR),
        CHD_RES_LIST(2, 0, CHD_RES_L(2)),
        CHD_RES_LIST(2, 2, CHD_RES_C(
            CHD_RES(NOTE(G, 1, 0), &MAHLER_MINOR_TRIAD),
            CHD_RES(NOTE(A, -1, 0), &MAHLER_MINOR_TRIAD)
        ))
    );
    ASSERT_CRL(returnChord(NOTE_L(NOTE(E, -1, 1), NOTE(B, -1, 1)), 2, &CHORD_LIST, NULL, true, &ERR),
        CHD_RES_LIST(10, 0, CHD_RES_L(10)),
        CHD_RES_LIST(10, 6, CHD_RES_C(
            CHD_RES(NOTE(E, -1, 0), &MAHLER_MAJOR_TRIAD),
            CHD_RES(NOTE(D, 1, 0), &MAHLER_MAJOR_TRIAD),
            CHD_RES(NOTE(E, -1, 0), &MAHLER_MINOR_TRIAD),
            CHD_RES(NOTE(D, 1, 0), &MAHLER_MINOR_TRIAD),
            CHD_RES(NOTE(E, -1, 0), &MAHLER_DOMINANT_7),
            CHD_RES(NOTE(D, 1, 0), &MAHLER_DOMINANT_7)
        ))
    );
    
}

bool
compChord(struct Chord chordA, struct Chord chordB) {
    return chordA.size == chordB.size &&
           chordA.inv == chordB.inv &&
           compNotes(chordA.notes, chordB.notes, chordA.size, chordB.size) &&
           compNotes(chordA.base, chordB.base, chordA.size, chordB.size);
           
}

bool
compChordBase(struct ChordBase const* baseA, struct ChordBase const* baseB)
{
    return !strcmp(baseA->name, baseB->name) &&
           baseA->size == baseB->size &&
           compInters(baseA->steps, baseB->steps, baseA->size, baseB->size);
}

bool
compChordResult(struct ChordResult resultA, struct ChordResult resultB)
{
    return compNote(resultA.key, resultB.key) &&
           compChordBase(resultA.chord, resultB.chord);
}

bool
compChordResultList(struct ChordResultList listA, struct ChordResultList listB)
{
    return listA.max == listB.max &&
           listA.size == listB.size &&
           compChordResults(listA.results, listB.results, listA.size, listB.size);
}

bool
compChordResults(struct ChordResult* resultsA, struct ChordResult* resultsB, size_t sizeA, size_t sizeB)
{
    if (sizeA != sizeB) {
        return false;
    }

    for (size_t i = 0; i < sizeA; i++) {
        if (!compChordResult(resultsA[i], resultsB[i])) {
            return false;
        }
    }
    return true;
}