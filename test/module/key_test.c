#define ASSERT_K(act, exp) ASSERT(compKeySig(act, exp) && (ERR == MAHLER_ERROR_NONE), exp)

#define KEY(type, alter, size, key, notes) (struct KeySig) {type, alter, size, key, notes}

#define MAJOR_KEY MAHLER_MAJOR_KEY
#define MINOR_KEY MAHLER_MINOR_KEY

void testGetKeySig();
void testReturnKeySig();
void testGetKeyRelative();
void testQueryAcci();

bool compKeySig(struct KeySig keyA, struct KeySig keyB);

void
testGetKeySig()
{

    ASSERT_K(getKeySig(NOTE(A, 0, 0), MAJOR_KEY), KEY(MAJOR_KEY, 3, 3, NOTE(A, 0, 0), NOTE_K(
        NOTE(F, 1, 0),
        NOTE(C, 1, 0),
        NOTE(G, 1, 0)
    )));
    ASSERT_K(getKeySig(NOTE(D, -1, 5), MAJOR_KEY), KEY(MAJOR_KEY, -5, 5, NOTE(D, -1, 0), NOTE_K(
        NOTE(B, -1, 0),
        NOTE(E, -1, 0),
        NOTE(A, -1, 0),
        NOTE(D, -1, 0),
        NOTE(G, -1, 0)
    )));
    
    ASSERT_K(getKeySig(NOTE(F, 0, 2), MAJOR_KEY), KEY(MAJOR_KEY, -1, 1, NOTE(F, 0, 0), NOTE_K(
        NOTE(B, -1, 0)
    )));
    ASSERT_K(getKeySig(NOTE(B, 0, 3), MINOR_KEY), KEY(MINOR_KEY, 2, 2, NOTE(B, 0, 0), NOTE_K(
        NOTE(F, 1, 0),
        NOTE(C, 1, 0)
    )));
    ASSERT_K(getKeySig(NOTE(G, 1, 4), MINOR_KEY), KEY(MINOR_KEY, 5, 5, NOTE(G, 1, 0), NOTE_K(
        NOTE(F, 1, 0),
        NOTE(C, 1, 0),
        NOTE(G, 1, 0),
        NOTE(D, 1, 0),
        NOTE(A, 1, 0)
    )));
    ASSERT_K(getKeySig(NOTE(C, 2, 2), MAJOR_KEY), KEY(MAJOR_KEY, 14, 7, NOTE(C, 2, 0), NOTE_K(
        NOTE(F, 2, 0),
        NOTE(C, 2, 0),
        NOTE(G, 2, 0),
        NOTE(D, 2, 0),
        NOTE(A, 2, 0),
        NOTE(E, 2, 0),
        NOTE(B, 2, 0)
    )));
    ASSERT_K(getKeySig(NOTE(F, -3, 9), MINOR_KEY), KEY(MINOR_KEY, -25, 7, NOTE(F, -3, 0), NOTE_K(
        NOTE(B, -4, 0),
        NOTE(E, -4, 0),
        NOTE(A, -4, 0),
        NOTE(D, -4, 0),
        NOTE(G, -3, 0),
        NOTE(C, -3, 0),
        NOTE(F, -3, 0)
    )));
}

void
testReturnKeySig()
{
    ASSERT_K(returnKeySig(0, MAJOR_KEY), KEY(MAJOR_KEY, 0, 0, NOTE(C, 0, 0), NOTE_K(
        0
    )));
    ASSERT_K(returnKeySig(3, MAJOR_KEY), KEY(MAJOR_KEY, 3, 3, NOTE(A, 0, 0), NOTE_K(
        NOTE(F, 1, 0),
        NOTE(C, 1, 0),
        NOTE(G, 1, 0)
    )));
    ASSERT_K(returnKeySig(-5, MAJOR_KEY), KEY(MAJOR_KEY, -5, 5, NOTE(D, -1, 0), NOTE_K(
        NOTE(B, -1, 0),
        NOTE(E, -1, 0),
        NOTE(A, -1, 0),
        NOTE(D, -1, 0),
        NOTE(G, -1, 0)
    )));
    
    ASSERT_K(returnKeySig(-1, MAJOR_KEY), KEY(MAJOR_KEY, -1, 1, NOTE(F, 0, 0), NOTE_K(
        NOTE(B, -1, 0)
    )));
    ASSERT_K(returnKeySig(2, MINOR_KEY), KEY(MINOR_KEY, 2, 2, NOTE(B, 0, 0), NOTE_K(
        NOTE(F, 1, 0),
        NOTE(C, 1, 0)
    )));
    ASSERT_K(returnKeySig(5, MINOR_KEY), KEY(MINOR_KEY, 5, 5, NOTE(G, 1, 0), NOTE_K(
        NOTE(F, 1, 0),
        NOTE(C, 1, 0),
        NOTE(G, 1, 0),
        NOTE(D, 1, 0),
        NOTE(A, 1, 0)
    )));
    ASSERT_K(returnKeySig(14, MAJOR_KEY), KEY(MAJOR_KEY, 14, 7, NOTE(C, 2, 0), NOTE_K(
        NOTE(F, 2, 0),
        NOTE(C, 2, 0),
        NOTE(G, 2, 0),
        NOTE(D, 2, 0),
        NOTE(A, 2, 0),
        NOTE(E, 2, 0),
        NOTE(B, 2, 0)
    )));
    ASSERT_K(returnKeySig(-25, MINOR_KEY), KEY(MINOR_KEY, -25, 7, NOTE(F, -3, 0), NOTE_K(
        NOTE(B, -4, 0),
        NOTE(E, -4, 0),
        NOTE(A, -4, 0),
        NOTE(D, -4, 0),
        NOTE(G, -3, 0),
        NOTE(C, -3, 0),
        NOTE(F, -3, 0)
    )));
}

void
testGetKeyRelative()
{
    ASSERT_K(getKeyRelative(&KEY(MAJOR_KEY, -5, 5, NOTE(D, -1, 5), NOTE_K(
        NOTE(B, -1, 0),
        NOTE(E, -1, 0),
        NOTE(A, -1, 0),
        NOTE(D, -1, 0),
        NOTE(G, -1, 0)
    ))), KEY(MINOR_KEY, -5, 5, NOTE(B, -1, 0), NOTE_K(
        NOTE(B, -1, 0),
        NOTE(E, -1, 0),
        NOTE(A, -1, 0),
        NOTE(D, -1, 0),
        NOTE(G, -1, 0)
    )));
    ASSERT_K(getKeyRelative(&KEY(MINOR_KEY, -1, 1, NOTE(D, 0, 2), NOTE_K(
        NOTE(B, -1, 0)
    ))), KEY(MAJOR_KEY, -1, 1, NOTE(F, 0, 0), NOTE_K(
        NOTE(B, -1, 0)
    )));
    ASSERT_K(getKeyRelative(&KEY(MAJOR_KEY, 14, 7, NOTE(C, 2, 2), NOTE_K(
        NOTE(F, 2, 0),
        NOTE(C, 2, 0),
        NOTE(G, 2, 0),
        NOTE(D, 2, 0),
        NOTE(A, 2, 0),
        NOTE(E, 2, 0),
        NOTE(B, 2, 0)
    ))), KEY(MINOR_KEY, 14, 7, NOTE(A, 2, 0), NOTE_K(
        NOTE(F, 2, 0),
        NOTE(C, 2, 0),
        NOTE(G, 2, 0),
        NOTE(D, 2, 0),
        NOTE(A, 2, 0),
        NOTE(E, 2, 0),
        NOTE(B, 2, 0)
    )));
    ASSERT_K(getKeyRelative(&KEY(MINOR_KEY, -25, 7, NOTE(F, -3, 9), NOTE_K(
        NOTE(B, -4, 0),
        NOTE(E, -4, 0),
        NOTE(A, -4, 0),
        NOTE(D, -4, 0),
        NOTE(G, -3, 0),
        NOTE(C, -3, 0),
        NOTE(F, -3, 0)
    ))), KEY(MAJOR_KEY, -25, 7, NOTE(A, -4, 0), NOTE_K(
        NOTE(B, -4, 0),
        NOTE(E, -4, 0),
        NOTE(A, -4, 0),
        NOTE(D, -4, 0),
        NOTE(G, -3, 0),
        NOTE(C, -3, 0),
        NOTE(F, -3, 0)
    )));
}

void
testQueryAcci()
{
    ASSERT_D(queryAcci(&KEY(MAJOR_KEY, 0, 0, NOTE(C, 0, 0), NOTE_K(
        0
    )), MAHLER_F), 0);
    ASSERT_D(queryAcci(&KEY(MINOR_KEY, 0, 0, NOTE(B, -1, 0), NOTE_K(
        NOTE(B, -1, 0),
        NOTE(E, -1, 0),
        NOTE(A, -1, 0),
        NOTE(D, -1, 0),
        NOTE(G, -1, 0)
    )), MAHLER_D), -1);
    ASSERT_D(queryAcci(&KEY(MAJOR_KEY, 0, 0, NOTE(D, 1, 0), NOTE_K(
        NOTE(F, 2, 0),
        NOTE(C, 2, 0),
        NOTE(G, 1, 0),
        NOTE(D, 1, 0),
        NOTE(A, 1, 0),
        NOTE(E, 1, 0),
        NOTE(B, 1, 0)
    )), MAHLER_G), 1);
}

bool
compKeySig(struct KeySig keyA, struct KeySig keyB)
{
    return keyA.type == keyB.type &&
           keyA.alter == keyB.alter &&
           keyA.size == keyB.size &&
           compNote(keyA.key, keyB.key) &&
           compNotes(keyA.notes, keyB.notes, keyA.size, keyB.size);
}