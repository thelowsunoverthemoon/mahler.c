#include <stdio.h>
#include "mahler.h"

// Transpose Melody up Perfect 5th //

#define BUF_SIZE 10

void transpose(struct Note* mel, struct Note* buf, int size, struct Interval inter);

int
main(void)
{
    struct Note mel[] = {
        {MAHLER_C, MAHLER_NONE, 3},
        {MAHLER_D, MAHLER_FLAT, 3},
        {MAHLER_F, MAHLER_SHARP, 4},
    };
    
    struct Note buf[BUF_SIZE];
    int melLen = sizeof(mel) / sizeof(*mel);
    transpose(mel, buf, melLen, (struct Interval) {
        5, MAHLER_PERFECT
    });
    
    char disp[MAHLER_DISP_LEN];
    for (size_t i = 0; i < melLen; i++) {
        printf("%s ", printNote(buf[i], disp, MAHLER_DISP_LEN));
    }
}

void
transpose(struct Note* mel, struct Note* buf, int size, struct Interval inter)
{
    for (int i = 0; i < size; i++) {
        buf[i] = getInter(mel[i], inter);
    }
}
