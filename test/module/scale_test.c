#define ASSERT_S(act, exp) ASSERT(compScale(act, exp) && (ERR == MAHLER_ERROR_NONE), exp)

#define ASSERT_SRL(func, srl, exp) {\
    struct ScaleResultList SCALE_LIST = srl;\
    func;\
    ASSERT(compScaleResultList(SCALE_LIST, exp) && (ERR == MAHLER_ERROR_NONE), exp);\
} while(0)
    
#define SCALE(size, type, notes) (struct Scale) {size, type, notes}
#define SCALE_B(size, steps) &(struct ScaleBase) {"", size, steps}

#define SCL_RES(note, base) (struct ScaleResult) {note, base}
#define SCL_RES_L(size) (struct ScaleResult[size]) {0}
#define SCL_RES_C(...) (struct ScaleResult[]) {__VA_ARGS__}
#define SCL_RES_LIST(max, size, results) (struct ScaleResultList) {max, size, results}

#define ASCEND MAHLER_ASCEND
#define DESCEND MAHLER_DESCEND
#define FULL MAHLER_FULL

void testGetScale();
void testReturnScale();

bool compScale(struct Scale scaleA, struct Scale scaleB);
bool compScaleResultList(struct ScaleResultList listA, struct ScaleResultList listB);
bool compScaleResult(struct ScaleResult resultA, struct ScaleResult resultB);
bool compScaleResults(struct ScaleResult* resultsA, struct ScaleResult* resultsB, size_t sizeA, size_t sizeB);
bool compScaleBase(struct ScaleBase const* baseA, struct ScaleBase const* baseB);

void
testReturnScale()
{
    
    ASSERT_E(returnScale(NOTE_L(NOTE(A, 0, 0)), 1, &SCL_RES_LIST(5, 0, SCL_RES_L(5)), NULL, false, &ERR), ERROR_OVERFLOW_SCALE_RETURN);
    ASSERT_SRL(returnScale(NOTE_L(NOTE(A, 0, 1), NOTE(D, 0, 1), NOTE(C, 1, 1), NOTE(G, 1, 2)), 4, &SCALE_LIST, NULL, false, &ERR),
        SCL_RES_LIST(5, 0, SCL_RES_L(5)),
        SCL_RES_LIST(5, 3, SCL_RES_C(
            SCL_RES(NOTE(A, 0, 0), &MAHLER_MAJOR_SCALE),
            SCL_RES(NOTE(F, 1, 0), &MAHLER_NATURAL_MIN_SCALE),
            SCL_RES(NOTE(F, 1, 0), &MAHLER_HARMONIC_MIN_SCALE)
        ))
    );
    ASSERT_SRL(returnScale(NOTE_L(NOTE(F, 9, 1)), 1, &SCALE_LIST, NULL, false, &ERR),
        SCL_RES_LIST(2, 0, SCL_RES_L(2)),
        SCL_RES_LIST(2, 0, SCL_RES_C(
            0
        ))
    );
    ASSERT_SRL(returnScale(NOTE_L(NOTE(A, 0, 1), NOTE(C, 1, 1), NOTE(B, 0, 1), NOTE(D, 0, 2), NOTE(E, 0, 2), NOTE(F, 1, 2), NOTE(G, 1, 2)), 7, &SCALE_LIST, NULL, false, &ERR),
        SCL_RES_LIST(2, 0, SCL_RES_L(2)),
        SCL_RES_LIST(2, 2, SCL_RES_C(
            SCL_RES(NOTE(A, 0, 0), &MAHLER_MAJOR_SCALE),
            SCL_RES(NOTE(F, 1, 0), &MAHLER_NATURAL_MIN_SCALE)
        ))
    );
    ASSERT_SRL(returnScale(NOTE_L(NOTE(D, 1, 1), NOTE(A, 0, 2), NOTE(B, 0, 2), NOTE(G, 1, 2)), 4, &SCALE_LIST, NULL, true, &ERR),
        SCL_RES_LIST(6, 0, SCL_RES_L(6)),
        SCL_RES_LIST(6, 5, SCL_RES_C(
            SCL_RES(NOTE(E, 0, 0), &MAHLER_MAJOR_SCALE),
            SCL_RES(NOTE(C, 1, 0), &MAHLER_NATURAL_MIN_SCALE),
            SCL_RES(NOTE(D, -1, 0), &MAHLER_NATURAL_MIN_SCALE),
            SCL_RES(NOTE(F, 1, 0), &MAHLER_MELODIC_MIN_SCALE),
            SCL_RES(NOTE(G, -1, 0), &MAHLER_MELODIC_MIN_SCALE)
        ))
    );
   
}

void
testGetScale()
{
    ASSERT_E(getScale(NOTE(D, 3, 6), SCALE_B(2, INTER_L(INTER(3, PERFECT))), NOTE_N(6, 0), ASCEND, &ERR), ERROR_INVALID_QUAL);
    
    ASSERT_S(getScale(NOTE(C, 0, 4), &MAHLER_PENTATONIC_MAJ_SCALE, NOTE_N(6, 0), ASCEND, &ERR), 
        SCALE(6, ASCEND, NOTE_L(
            NOTE(C, 0, 4), NOTE(D, 0, 4), NOTE(E, 0, 4), NOTE(G, 0, 4), NOTE(A, 0, 4), NOTE(C, 0, 5)
        ))
    );
    
    ASSERT_S(getScale(NOTE(F, -2, 4), &MAHLER_BLUES_SCALE, NOTE_N(8, 0), DESCEND, &ERR), 
        SCALE(7, DESCEND, NOTE_L(
            NOTE(F, -2, 5), NOTE(E, -3, 5), NOTE(C, -2, 5), NOTE(B, -2, 4), NOTE(B, -3, 4), NOTE(A, -3, 4), NOTE(F, -2, 4)
        ))
    );
    ASSERT_S(getScale(NOTE(B, 3, 2), &MAHLER_NATURAL_MIN_SCALE, NOTE_N(16, 0), FULL, &ERR), 
        SCALE(16, FULL, NOTE_L(
            NOTE(B, 3, 2), NOTE(C, 4, 3), NOTE(D, 3, 3), NOTE(E, 3, 3), NOTE(F, 4, 3), NOTE(G, 3, 3), NOTE(A, 3, 3), NOTE(B, 3, 3),
            NOTE(B, 3, 3), NOTE(A, 3, 3), NOTE(G, 3, 3), NOTE(F, 4, 3), NOTE(E, 3, 3), NOTE(D, 3, 3), NOTE(C, 4, 3), NOTE(B, 3, 2)
        ))
    );
}

bool
compScale(struct Scale scaleA, struct Scale scaleB)
{
    return scaleA.size == scaleB.size &&
           scaleA.type == scaleB.type &&
           compNotes(scaleA.notes, scaleB.notes, scaleA.size, scaleB.size);
}

bool
compScaleBase(struct ScaleBase const* baseA, struct ScaleBase const* baseB)
{
    return !strcmp(baseA->name, baseB->name) &&
           baseA->size == baseB->size &&
           compInters(baseA->steps, baseB->steps, baseA->size, baseB->size);
}

bool
compScaleResult(struct ScaleResult resultA, struct ScaleResult resultB)
{
    return compNote(resultA.key, resultB.key) &&
           compScaleBase(resultA.scale, resultB.scale);
}

bool
compScaleResultList(struct ScaleResultList listA, struct ScaleResultList listB)
{
    return listA.max == listB.max &&
           listA.size == listB.size &&
           compScaleResults(listA.results, listB.results, listA.size, listB.size);
}

bool
compScaleResults(struct ScaleResult* resultsA, struct ScaleResult* resultsB, size_t sizeA, size_t sizeB)
{
    if (sizeA != sizeB) {
        return false;
    }

    for (size_t i = 0; i < sizeA; i++) {
        if (!compScaleResult(resultsA[i], resultsB[i])) {
            return false;
        }
    }
    return true;
}