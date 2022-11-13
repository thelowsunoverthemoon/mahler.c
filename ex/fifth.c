#include <stdio.h>
#include "mahler.h"

// Generate the Major Circle of 5ths for 12 iterations //
/*
    C0 -> 0
    G0 -> 1
    D1 -> 2
    A1 -> 3
    E2 -> 4
    B2 -> 5
    F#3 -> 6
    C#4 -> 7
    G#4 -> 8
    D#5 -> 9
    A#5 -> 10
    E#6 -> 11
*/

#define GEN_SIZE 12

void genCircle5ths(struct Note buf[], int iter);

int
main(void)
{
    
    struct Note buf[GEN_SIZE];
    genCircle5ths(buf, GEN_SIZE);
    
    char disp[MAHLER_DISP_LEN];
    for (size_t i = 0; i < GEN_SIZE; i++) {
        struct KeySig key = getKeySig(buf[i], MAHLER_MAJOR_KEY);
        printf("%s -> %d\n", printNote(buf[i], disp, MAHLER_DISP_LEN, NULL), key.alter);
    }
}

void
genCircle5ths(struct Note buf[], int iter)
{
    struct Note prev = (struct Note) {MAHLER_C, MAHLER_NONE, 0};
    buf[0] = prev;
    
    for (int i = 1; i < iter; i++) {
        buf[i] = prev = getInter(prev, (struct Interval) {5, MAHLER_PERFECT}, NULL);
    }
    
}