#include <stdio.h>
#include "mahler.h"

// Create a Major Scale Chart starting from C0 //
/*
    C0  D0  E0  F0  G0  A0  B0  C1
    D0  E0  F#0 G0  A0  B0  C#1 D1
    E0  F#0 G#0 A0  B0  C#1 D#1 E1
    F0  G0  A0  Bb0 C1  D1  E1  F1
    G0  A0  B0  C1  D1  E1  F#1 G1
    A0  B0  C#1 D1  E1  F#1 G#1 A1
    B0  C#1 D#1 E1  F#1 G#1 A#1 B1
    C1  D1  E1  F1  G1  A1  B1  C2
*/

int
main(void)
{
    struct mah_note sc_notes[8];
    struct mah_scale scale = mah_get_scale(
        (struct mah_note) {MAH_C, MAH_NATURAL, 0}, &MAH_MAJOR_SCALE, sc_notes, MAH_ASCEND, NULL
    );

    char disp[MAH_DISP_LEN];
    for (int i = 0; i < scale.size; i++) {
        
        struct mah_note temp_notes[8];
        struct mah_scale temp = mah_get_scale(
            scale.notes[i], &MAH_MAJOR_SCALE, temp_notes, MAH_ASCEND, NULL
        );
        
        for (size_t j = 0; j < scale.size; j++) {
            printf("%-3s ", mah_write_note(temp.notes[j], disp, MAH_DISP_LEN, NULL));
        }
        putchar('\n');
    }
}