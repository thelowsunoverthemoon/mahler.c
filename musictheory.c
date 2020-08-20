#include <stdio.h>
#include "musictheory.h"

#define NOTETOCHROMA(A) { \
    if (A > 2) {          \
        A = 2 * A;        \
    } else {              \
        A = 2 * A + 1;    \
    }                     \
}

static Note chromatic[] = {
    {C, NONE, 0},
    {C, SHARP, 0},
    {D, NONE, 0},
    {D, SHARP, 0},
    {E, NONE, 0},
    {F, NONE, 0},
    {F, SHARP, 0},
    {G, NONE, 0},
    {G, SHARP, 0},
    {A, NONE, 0},
    {A, SHARP, 0},
    {B, NONE, 0},
    {C, NONE, 0},
    {C, SHARP, 0}
};

const ScaleBase MAJORSCALE = {
    8, (Interval[]) {{2, MAJOR},{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MAJOR},{2, MINOR}}
};

const ScaleBase NATURALMINSCALE = {
    8, (Interval[]) {{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR}}
};

const ScaleBase HARMONICMINSCALE = {
    8, (Interval[]) {{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MINOR},{2, AUGMENTED},{2, MINOR}}
};

const ScaleBase MELODICMINSCALE = {
    8, (Interval[]) {{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MAJOR},{2, MAJOR},{2, MINOR}}
};

const ScaleBase PENTATONICMINSCALE = {
    6, (Interval[]) {{2, MAJOR},{2, MAJOR},{3, MINOR},{2, MAJOR},{3, MINOR}}
};

const ScaleBase PENTATONICMAJSCALE = {
    6, (Interval[]) {{3, MINOR},{2, MAJOR},{2, MAJOR},{3, MINOR},{2, MAJOR}}
};

const ChordBase MAJORTRIAD = {
    3, (Interval[]) {{3, MAJOR},{3, MINOR}}
};

const ChordBase MINORTRIAD = {
    3, (Interval[]) {{3, MINOR},{3, MAJOR}}
};

const ChordBase AUGMENTEDTRIAD = {
    3, (Interval[]) {{3, MAJOR},{3, MAJOR}}
};

const ChordBase DIMINISHEDTRIAD = {
    3, (Interval[]) {{3, MINOR},{3, MINOR}}
};

const ChordBase DIMINISHED7 = {
    4, (Interval[]) {{3, MINOR},{3, MINOR},{3, MINOR}}
};

const ChordBase HALFDIMINISHED7 = {
    4, (Interval[]) {{3, MINOR},{3, MINOR},{3, MAJOR}}
};

const ChordBase MINOR7 = {
    4, (Interval[]) {{3, MINOR},{3, MAJOR},{3, MINOR}}
};

const ChordBase MAJOR7 = {
    4, (Interval[]) {{3, MAJOR},{3, MINOR},{3, MAJOR}}
};

const ChordBase DOMINANT7 = {
    4, (Interval[]) {{3, MAJOR},{3, MINOR},{3, MINOR}}
};

static int modeCompoundInter(int* pitch, int* inter);
static int modeSimpleInter(int* pitch, int* inter);
static void modeAscend(Note notes[], const ScaleBase* type, Note* current, int index);
static void modeDescend(Note notes[], const ScaleBase* type, Note* current, int index);
static void modeFull(Note notes[], const ScaleBase* type, Note* current, int index);

static int interSteps[] = {0, 2, 4, 5, 7, 9, 11, 12};
static char* dispNote[] = {"C", "D", "E", "F", "G", "A", "B"};
static char* dispAccidental[] = {"bbbb", "bbb", "bb", "b", "", "#", "##", "###", "####"};
static modeScale modeScaleArray[] = {modeAscend, modeDescend, modeFull};


void printNote(char* prefix, Note note, char* suffix) {
    if (!(note.note == -1)) {
        printf("%s%s%s%d%s", prefix, dispNote[note.note], dispAccidental[note.acci + 4], note.pitch, suffix);
    }
}

void printChord(char* prefix, Chord chord, char* suffix) {
    printf("%s", prefix);
    for (int i = 0; i < chord.size; i++) {
        printNote("", chord.notes[i], " ");
    }
    printf("%s", suffix);
}

void printScale(char* prefix, Scale scale, char* suffix) {
    printf("%s", prefix);
    for (int i = 0; i < scale.size; i++) {
        printNote("", scale.notes[i], " ");
    }
    printf("%s", suffix);
}

double getFreqOrWave(Note note, int standard, enum NoteFormula type) {
    int root = note.note;
    NOTETOCHROMA(root);
    root += note.acci;
    int iter = (12 * (note.pitch - 1) + root) - 46;
    double base;
    if (iter > 0) {
        base = 1.059463;
    } else {
        iter *= -1;
        base = 1/1.059463;
    }
    double raw = 1;
    for (int i = 0; i < iter; i++) {
        raw *= base;
    }
    return type == FREQUENCY ? raw * standard : 345/(raw * standard);
}

int isEnharmonic(Note notea, Note noteb) {
    int roota = notea.note;
    NOTETOCHROMA(roota);
    roota += notea.acci;
    if (roota < 1) {
        roota += 11;
    } else if (roota > 12) {
        roota -= 11;
    }
    roota += notea.pitch;
    int rootb = noteb.note;
    NOTETOCHROMA(rootb);
    rootb += noteb.acci;
    if (rootb < 1) {
        rootb += 11;
    } else if (rootb > 12) {
        rootb -= 11;
    }
    rootb += noteb.pitch;
    return roota == rootb ? 1 : 0;
}

Chord invertChord(Chord* chord, int inversion) {
    if (inversion > chord->size - 1 || inversion < 0) {
        return *chord;
    }
    for (int i = 0; i < chord->size; i++) {
        chord->notes[i] = chord->base[i];
    }
    Note temp;
    for (int j = 0; j < inversion; j++) {
        temp = chord->notes[0];
        temp.pitch += 1;
        for (int i = 0; i < chord->size - 1; i++) {
            chord->notes[i] = chord->notes[i + 1];
        }
        chord->notes[chord->size - 1] = temp;
    }
    chord->inversion = inversion;
    return *chord;
}

Chord getChord(Note note, const ChordBase* type, Note base[], Note notes[]) {
    Chord chord; // need error checking for array!!!
    Note current = note;
    notes[0] = note;
    base[0] = note;
    for (int i = 1; i < type->size; i++) {
        notes[i] = getInterStruct(current, type->steps[i - 1], SIMPLE);
        base[i] = notes[i];
        current = notes[i];
    }
    chord.notes = notes;
    chord.base = base;
    chord.inversion = 0;
    chord.size = type->size;
    return chord;
}

Scale getScale(Note start, const ScaleBase* type, Note notes[], enum ScaleType mode) {
    int length = type->length;
    Note current = start;
    switch(mode) {
        case DESCEND:
            notes[type->length - 1] = start;
            break;
        case ASCEND:
            notes[0] = start;
            break;
        case FULL:
            length *= 2;
            notes[0] = start;
            notes[type->length * 2 - 1] = start;
            break;
    }
    for (int i = 1; i < type->length; i++) {
        modeScaleArray[mode](notes, type, &current, i);
    }
    Scale scale = {length, mode, notes};
    return scale;
}

void modeAscend(Note notes[], const ScaleBase* type, Note* current, int index) {
    notes[index] = getInterStruct(*current, type->steps[index - 1], SIMPLE);
    *current = notes[index];
}

void modeDescend(Note notes[], const ScaleBase* type, Note* current, int index) {
    notes[type->length - index - 1] = getInterStruct(*current, type->steps[index - 1], SIMPLE);
    *current = notes[type->length - index - 1];
}

void modeFull(Note notes[], const ScaleBase* type, Note* current, int index) {
    notes[index] = getInterStruct(*current, type->steps[index - 1], SIMPLE);
    notes[type->length * 2 - index - 1] = notes[index];
    *current = notes[index];
}

static int modeCompoundInter(int* pitch, int* inter) {
    if (*inter > 15 || *inter < 9 || *pitch < 0) {
        return 1;
    }
    *pitch += 1;
    *inter -= 7;
    return 0;
}

static int modeSimpleInter(int* pitch, int* inter) {
    return (*inter > 8 || *inter < 1 || *pitch < 0) ? 1 : 0;
}

Note getInter(enum NoteOrder root, enum Accidental acci, int pitch, int inter, enum Quality quality, modeInter type) {
    Note dest = {-1, 0, 0};
    if (type(&pitch, &inter) == 1) {
        return dest;
    }
    int end = 0;
    if (inter == 1 || inter == 4 || inter == 5 || inter == 8) {
        switch (quality) {
            case AUGMENTED:
                end = 0;
                break;
            case DIMINISHED:
                end = 1;
                break;
            case PERFECT:
                end = -3;
                break;
            default:
                return dest;
        }
    } else {
        if (quality == PERFECT) {
            return dest;
        }
    }
    int enharmonic = root + inter - 1;
    if (enharmonic > 6) {
        enharmonic -= 7;
        dest.pitch = pitch + 1;
    } else {
        dest.pitch = pitch;
    }
    NOTETOCHROMA(root);
    end += root + interSteps[inter - 1] + acci + quality - 1;
    if (end > 13) {
        end -= 12;
    } else if (end < 0) {
        end += 12;
    }
    if (chromatic[end].note == enharmonic) {
        dest.note = chromatic[end].note;
        dest.acci = chromatic[end].acci;
    } else {
        dest.note = enharmonic;
        NOTETOCHROMA(enharmonic);
        dest.acci = end - (enharmonic - 1);
        if (dest.acci > 4) {
            dest.acci -= 12;
        }
    }
    return dest;
}

Note getInterStruct(Note note, Interval interval, modeInter type) {
    Note dest = {-1, 0, 0};
    int inter = interval.inter, pitch = note.pitch, root = note.note, quality = interval.quality;
    if (type(&pitch, &inter) == 1) {
        return dest;
    }
    int end = 0;
    if (inter == 1 || inter == 4 || inter == 5 || inter == 8) {
        switch (quality) {
            case AUGMENTED:
                end = 0;
                break;
            case DIMINISHED:
                end = 1;
                break;
            case PERFECT:
                end = -3;
                break;
            default:
                return dest;
        }
    } else {
        if (quality == PERFECT) {
            return dest;
        }
    }
    int enharmonic = root + inter - 1;
    if (enharmonic > 6) {
        enharmonic -= 7;
        dest.pitch = pitch + 1;
    } else {
        dest.pitch = pitch;
    }
    NOTETOCHROMA(root);
    end += root + interSteps[inter - 1] + note.acci + quality - 1;
    if (end > 13) {
        end -= 12;
    } else if (end < 0) {
        end += 12;
    }
    if (chromatic[end].note == enharmonic) {
        dest.note = chromatic[end].note;
        dest.acci = chromatic[end].acci;
    } else {
        dest.note = enharmonic;
        NOTETOCHROMA(enharmonic);
        dest.acci = end - (enharmonic - 1);
        if (dest.acci > 4) {
            dest.acci -= 12;
        }
    }
    return dest;
}
