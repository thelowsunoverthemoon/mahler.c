#include <stdio.h>
#include "musictheory.h"

int
main(void) {
    struct Note scNotes[8];
    struct Scale scale = getScale((struct Note) {C, NONE, 0}, &MAJOR_SCALE, scNotes, SCALE_ASCEND);

    struct Note tempNotes[8];
    for (size_t i = 0; i < scale.size; i++) {
        struct Scale temp = getScale(scale.notes[i], &MAJOR_SCALE, tempNotes, SCALE_ASCEND);
        
        char disp[MT_DISP_LEN];
        for (size_t j = 0; j < scale.size; j++) {
            printf("%s ", printNote(temp.notes[j], disp, MT_DISP_LEN));
        }
        putchar('\n');
    }
}
