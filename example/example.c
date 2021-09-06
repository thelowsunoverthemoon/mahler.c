#include <stdio.h>
#include <stdlib.h>
#include "mahler.h"

int
main(void) {
    
    struct Note scNotes[8];
    struct Scale scale = getScale((struct Note) {MAHLER_C, MAHLER_NONE, 0}, &MAHLER_MAJOR_SCALE, scNotes, MAHLER_ASCEND);

    struct Note tempNotes[8];
    for (size_t i = 0; i < scale.size; i++) {
        struct Scale temp = getScale(scale.notes[i], &MAHLER_MAJOR_SCALE, tempNotes, MAHLER_ASCEND);
        
        char disp[MAHLER_DISP_LEN];
        for (size_t j = 0; j < scale.size; j++) {
            printf("%s ", printNote(temp.notes[j], disp, MAHLER_DISP_LEN));
        }
        putchar('\n');
    }
}
