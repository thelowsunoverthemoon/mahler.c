#define ASSERT_I(act, exp) ASSERT(compInterval(act, exp) && (ERR == MAHLER_ERROR_NONE), exp)

#define INTER(inter, qual) (struct Interval) {inter, MAHLER_ ## qual}
#define INTER_L(...) (struct Interval[]) {__VA_ARGS__}

void testReturnInter();
void testGetInter();

bool compInterval(struct Interval interA, struct Interval interB);
bool compInters(struct Interval const* intersA, struct Interval const* intersB, size_t sizeA, size_t sizeB);

void
testGetInter()
{
    ASSERT_E(getInter(NOTE(E, 5, 1), INTER(0, AUGMENTED), &ERR), ERROR_INVALID_RANGE);
    ASSERT_E(getInter(NOTE(B, 2, 10), INTER(-6, DIMINISHED), &ERR), ERROR_INVALID_RANGE);
    ASSERT_E(getInter(NOTE(D, 1, 3), INTER(4, MAJOR), &ERR), ERROR_INVALID_QUAL);
    ASSERT_E(getInter(NOTE(F, 3, 12), INTER(11, MAJOR), &ERR), ERROR_INVALID_QUAL);
    ASSERT_E(getInter(NOTE(A, -2, 5), INTER(3, PERFECT), &ERR), ERROR_INVALID_QUAL);
    ASSERT_E(getInter(NOTE(C, -1, 7), INTER(10, PERFECT), &ERR), ERROR_INVALID_QUAL);
    
    ASSERT_N(getInter(NOTE(F, 2, 3), INTER(6, AUGMENTED), &ERR), NOTE(D, 3, 4));
    ASSERT_N(getInter(NOTE(B, 0, 2), INTER(5, AUGMENTED), &ERR), NOTE(F, 2, 3));
    ASSERT_N(getInter(NOTE(E, -1, 0), INTER(12, AUGMENTED), &ERR), NOTE(B, 0, 1));
    ASSERT_N(getInter(NOTE(G, -3, 4), INTER(24, AUGMENTED), &ERR), NOTE(B, -2, 7));
    
    ASSERT_N(getInter(NOTE(G, 0, 3), INTER(1, DIMINISHED), &ERR), NOTE(G, -1, 3));
    ASSERT_N(getInter(NOTE(A, -5, 2), INTER(3, DIMINISHED), &ERR), NOTE(C, -6, 3));
    ASSERT_N(getInter(NOTE(C, -1, 0), INTER(11, DIMINISHED), &ERR), NOTE(F, -2, 1));
    ASSERT_N(getInter(NOTE(C, -3, 4), INTER(13, DIMINISHED), &ERR), NOTE(A, -5, 5));
    
    ASSERT_N(getInter(NOTE(E, 0, 3), INTER(7, MAJOR), &ERR), NOTE(D, 1, 4));
    ASSERT_N(getInter(NOTE(C, -1, 0), INTER(14, MAJOR), &ERR), NOTE(B, -1, 1));
    
    ASSERT_N(getInter(NOTE(D, -5, 2), INTER(2, MINOR), &ERR), NOTE(E, -6, 2));
    ASSERT_N(getInter(NOTE(C, -3, 4), INTER(13, MINOR), &ERR), NOTE(A, -4, 5));
    
    ASSERT_N(getInter(NOTE(G, 1, 3), INTER(1, PERFECT), &ERR), NOTE(G, 1, 3));
    ASSERT_N(getInter(NOTE(A, 0, 2), INTER(4, PERFECT), &ERR), NOTE(D, 0, 3));
    ASSERT_N(getInter(NOTE(F, -4, 0), INTER(5, PERFECT), &ERR), NOTE(C, -4, 1));
    ASSERT_N(getInter(NOTE(E, 5, 4), INTER(8, PERFECT), &ERR), NOTE(E, 5, 5));
    ASSERT_N(getInter(NOTE(B, 7, 4), INTER(12, PERFECT), &ERR), NOTE(F, 8, 6));

}

void
testReturnInter()
{
    ASSERT_E(returnInter(NOTE(C, 2, 3), NOTE(G, 4, 4), &ERR), ERROR_INVALID_QUAL);
    ASSERT_E(returnInter(NOTE(B, 0, 2), NOTE(F, -3, 3), &ERR), ERROR_INVALID_QUAL);
    ASSERT_E(returnInter(NOTE(E, -1, 0), NOTE(B, 0, -1), &ERR), ERROR_INVALID_RANGE);
    ASSERT_E(returnInter(NOTE(A, 0, 0), NOTE(F, 0, 0), &ERR), ERROR_INVALID_RANGE);
    
    ASSERT_I(returnInter(NOTE(F, 2, 3), NOTE(D, 3, 4), &ERR), INTER(6, AUGMENTED));
    ASSERT_I(returnInter(NOTE(B, 0, 2), NOTE(F, 2, 3), &ERR), INTER(5, AUGMENTED));
    ASSERT_I(returnInter(NOTE(E, -1, 0), NOTE(B, 0, 1), &ERR), INTER(12, AUGMENTED));
    ASSERT_I(returnInter(NOTE(G, -3, 4), NOTE(B, -2, 7), &ERR), INTER(24, AUGMENTED));

    ASSERT_I(returnInter(NOTE(G, 0, 3), NOTE(G, -1, 3), &ERR), INTER(1, DIMINISHED));
    ASSERT_I(returnInter(NOTE(A, -5, 2), NOTE(C, -6, 3), &ERR), INTER(3, DIMINISHED));
    ASSERT_I(returnInter(NOTE(C, -1, 0), NOTE(F, -2, 1), &ERR), INTER(11, DIMINISHED) );
    ASSERT_I(returnInter(NOTE(C, -3, 4), NOTE(A, -5, 5), &ERR), INTER(13, DIMINISHED));
    
    ASSERT_I(returnInter(NOTE(E, 0, 3), NOTE(D, 1, 4), &ERR), INTER(7, MAJOR));
    ASSERT_I(returnInter(NOTE(C, -1, 0), NOTE(B, -1, 1), &ERR), INTER(14, MAJOR));
    
    ASSERT_I(returnInter(NOTE(D, -5, 2), NOTE(E, -6, 2), &ERR), INTER(2, MINOR));
    ASSERT_I(returnInter(NOTE(C, -3, 4), NOTE(A, -4, 5), &ERR), INTER(13, MINOR));
    
    ASSERT_I(returnInter(NOTE(G, 1, 3), NOTE(G, 1, 3), &ERR), INTER(1, PERFECT));
    ASSERT_I(returnInter(NOTE(A, 0, 2), NOTE(D, 0, 3), &ERR), INTER(4, PERFECT));
    ASSERT_I(returnInter(NOTE(F, -4, 0), NOTE(C, -4, 1), &ERR), INTER(5, PERFECT));
    ASSERT_I(returnInter(NOTE(E, 5, 4), NOTE(E, 5, 5), &ERR), INTER(8, PERFECT));
    ASSERT_I(returnInter(NOTE(B, 7, 4), NOTE(F, 8, 6), &ERR), INTER(12, PERFECT));
}

bool
compInterval(struct Interval interA, struct Interval interB)
{
    return interA.inter == interB.inter &&
           interA.qual == interB.qual;
}

bool
compInters(struct Interval const* intersA, struct Interval const* intersB, size_t sizeA, size_t sizeB) {
    if (sizeA != sizeB) {
        return false;
    }

    for (size_t i = 0; i < sizeA; i++) {
        if (!compInterval(intersA[i], intersB[i])) {
            return false;
        }
    }
    return true;
}