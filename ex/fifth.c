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

void gen_circle_fifths(struct mah_note buf[], int iter);

int
main(void)
{
    
    struct mah_note buf[GEN_SIZE];
    gen_circle_fifths(buf, GEN_SIZE);
    
    char disp[MAH_DISP_LEN];
    for (int i = 0; i < GEN_SIZE; i++) {
        struct mah_key_sig key = mah_get_key_sig(buf[i], MAH_MAJOR_KEY);
        printf("%s -> %d\n", mah_write_note(buf[i], disp, MAH_DISP_LEN, NULL), key.alter);
    }
}

void
gen_circle_fifths(struct mah_note buf[], int iter)
{
    struct mah_note prev = (struct mah_note) {MAH_C, MAH_NATURAL, 0};
    buf[0] = prev;
    
    for (int i = 1; i < iter; i++) {
        buf[i] = prev = mah_get_inter(prev, (struct mah_interval) {5, MAH_PERFECT}, NULL);
    }
    
}