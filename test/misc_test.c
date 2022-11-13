#define ASSERT_ST(act, exp) ASSERT(!strcmp(act, exp), exp)

#define ASSERT_BC(func, size, exp) {\
    char BUF[size] = {0};\
    func;\
    ASSERT_ST(BUF, exp);\
} while(0)

#define BUF_C(size) (char[size]) {0}

void testGetMahlerError();
void testIsEnharmonic();
void testPrintNote();

void
testIsEnharmonic()
{
    ASSERT_D(isEnharmonic(NOTE(C, 0, 5), NOTE(B, 1, 4)), true);
    ASSERT_D(isEnharmonic(NOTE(D, 1, 2), NOTE(D, 1, 2)), true);
    ASSERT_D(isEnharmonic(NOTE(F, -1, 3), NOTE(E, 0, 3)), true);
    ASSERT_D(isEnharmonic(NOTE(E, 2, 3), NOTE(A, -3, 3)), true);
    ASSERT_D(isEnharmonic(NOTE(G, 1, 7), NOTE(A, -1, 7)), true);
    ASSERT_D(isEnharmonic(NOTE(A, 4, 7), NOTE(B, 2, 7)), true);
    
    ASSERT_D(isEnharmonic(NOTE(G, 1, 3), NOTE(G, 1, 4)), false);
    ASSERT_D(isEnharmonic(NOTE(B, 1, 4), NOTE(C, 1, 5)), false);
    ASSERT_D(isEnharmonic(NOTE(C, 4, 2), NOTE(F, 0, 2)), false);
    ASSERT_D(isEnharmonic(NOTE(F, -1, 3), NOTE(E, 2, 3)), false);
    ASSERT_D(isEnharmonic(NOTE(A, -1, 3), NOTE(A, 0, 3)), false);
}

void
testPrintNote()
{

    ASSERT_BC(printNote(NOTE(F, 2, 3), BUF, 5, &ERR), 5, "F##3");
    
    ASSERT_BC(printNote(NOTE(B, 4, 0), BUF, 7, &ERR), 7, "B####0");
    ASSERT_BC(printNote(NOTE(G, -4, 1), BUF, 7, &ERR), 7, "Gbbbb1");
    ASSERT_BC(printNote(NOTE(C, 0, 9), BUF, 5, &ERR), 5, "C9");
    
    ASSERT_E(printNote(NOTE(D, 5, 12), BUF_C(4), 4, &ERR), ERROR_INVALID_PRINT_QUAL);
    ASSERT_E(printNote(NOTE(A, -5, 1), BUF_C(4), 4, &ERR), ERROR_INVALID_PRINT_QUAL);
    ASSERT_E(printNote(NOTE(B + 1, 1, 2), BUF_C(4), 4, &ERR), ERROR_INVALID_PRINT_NOTE);
    ASSERT_E(printNote(NOTE(C - 1, 1, 2), BUF_C(4), 4, &ERR), ERROR_INVALID_PRINT_NOTE);
    
    ASSERT_E(printNote(NOTE(F, 3, 2), BUF_C(2), 2, &ERR), ERROR_OVERFLOW_PRINT_NOTE);
    
}

void
testGetMahlerError()
{
    ASSERT_ST(getMahlerError(MAHLER_ERROR_NONE), "None");
    ASSERT_ST(getMahlerError(MAHLER_ERROR_INVALID_QUAL), "Invalid Quality for Interval");
    ASSERT_ST(getMahlerError(200), "Unknown Error");
}