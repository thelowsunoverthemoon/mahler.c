#include <stdio.h>
#include "musictheory.h"

int main(void) {
    Note scbase[8];
    Scale cmajor = getScale((Note) {C, NONE, 4}, &NATURALMINSCALE, scbase, DESCEND);
    printf("Major Scale and Chord of each note of the C4 minor scale, descending\n\n");

    Note scdisp[8];
    Note chdbase[3];
    Note chdnotes[3];
    for (int i = 0; i < cmajor.size; i++) {
        printNote("Major Scale of ", cmajor.notes[i], "\n");
        printScale("", getScale(cmajor.notes[i], &MAJORSCALE, scdisp, ASCEND), "\n");
        printNote("Major Triad of ", cmajor.notes[i], "\n");
        printChord("", getChord(cmajor.notes[i], &MAJORTRIAD, chdbase, chdnotes), "\n\n");
    }

}
