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
    struct Note scNotes[8];
    struct Scale scale = getScale(
        (struct Note) {MAHLER_C, MAHLER_NONE, 0}, &MAHLER_MAJOR_SCALE, scNotes, MAHLER_ASCEND, NULL
    );

    char disp[MAHLER_DISP_LEN];
    for (size_t i = 0; i < scale.size; i++) {
        
        struct Note tempNotes[8];
        struct Scale temp = getScale(
            scale.notes[i], &MAHLER_MAJOR_SCALE, tempNotes, MAHLER_ASCEND, NULL
        );
        
        for (size_t j = 0; j < scale.size; j++) {
            printf("%-3s ", printNote(temp.notes[j], disp, MAHLER_DISP_LEN, NULL));
        }
        putchar('\n');
    }
}