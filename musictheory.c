#include <stdio.h>
#include "musictheory.h"

#define TWELTH_ROOT_2 1.059463      // Needed for frequency formula (frequency of A4 * 2^(1/12)^half steps from A4)
#define SOUND_SPEED 345             // Needed for wavelength formula
#define SIZE_NOTE 6                 // Size of note scale from 0 (C D E F G A B)              
#define SIZE_CHROMATIC 12           // Size of normal chromatic scale
#define SIZE_ACCI 4                 // Max accidentals (bbbb to ####)
#define SIZE_CHROMATIC_ADJUSTED 13  // For interval functions, size of chromatic[]
#define STEPS_A4 46                 // Steps to A4, starting from C0
#define ADJUST_OCTAVE 11            // For calculations in isEnharmonic
#define ADJUST_CHROMATIC 12         // For calculations in getInter functions
#define ADJUST_NOTE 7               // For calculations with note scale
#define SIMPLE_INTER_MAX 8          // Simple Interval maximum
#define SIMPLE_INTER_MIN 1          // Simple Interval minimum
#define COMPOUND_INTER_MAX 15       // Compound Interval maximum
#define COMPOUND_INTER_MIN 9        // Compoud Interval minimum

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

const ScaleBase MAJOR_SCALE = {
    8, (Interval[]) {{2, MAJOR},{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MAJOR},{2, MINOR}}
};

const ScaleBase NATURAL_MIN_SCALE = {
    8, (Interval[]) {{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR}}
};

const ScaleBase HARMONIC_MIN_SCALE = {
    8, (Interval[]) {{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MINOR},{2, AUGMENTED},{2, MINOR}}
};

const ScaleBase MELODIC_MIN_SCALE = {
    8, (Interval[]) {{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MAJOR},{2, MAJOR},{2, MINOR}}
};

const ScaleBase PENTATONIC_MIN_SCALE = {
    6, (Interval[]) {{2, MAJOR},{2, MAJOR},{3, MINOR},{2, MAJOR},{3, MINOR}}
};

const ScaleBase PENTATONIC_MAJ_SCALE = {
    6, (Interval[]) {{3, MINOR},{2, MAJOR},{2, MAJOR},{3, MINOR},{2, MAJOR}}
};

const ChordBase MAJOR_TRIAD = {
    3, (Interval[]) {{3, MAJOR},{3, MINOR}}
};

const ChordBase MINOR_TRIAD = {
    3, (Interval[]) {{3, MINOR},{3, MAJOR}}
};

const ChordBase AUGMENTED_TRIAD = {
    3, (Interval[]) {{3, MAJOR},{3, MAJOR}}
};

const ChordBase DIMINISHED_TRIAD = {
    3, (Interval[]) {{3, MINOR},{3, MINOR}}
};

const ChordBase DIMINISHED_7 = {
    4, (Interval[]) {{3, MINOR},{3, MINOR},{3, MINOR}}
};

const ChordBase HALF_DIMINISHED_7 = {
    4, (Interval[]) {{3, MINOR},{3, MINOR},{3, MAJOR}}
};

const ChordBase MINOR_7 = {
    4, (Interval[]) {{3, MINOR},{3, MAJOR},{3, MINOR}}
};

const ChordBase MAJOR_7 = {
    4, (Interval[]) {{3, MAJOR},{3, MINOR},{3, MAJOR}}
};

const ChordBase DOMINANT_7 = {
    4, (Interval[]) {{3, MAJOR},{3, MINOR},{3, MINOR}}
};

static int noteToChroma(int note);
static int modeCompoundInter(int* pitch, int* inter);
static int modeSimpleInter(int* pitch, int* inter);
static void modeAscend(Note notes[], const ScaleBase* type, Note* current, int index);
static void modeDescend(Note notes[], const ScaleBase* type, Note* current, int index);
static void modeFull(Note notes[], const ScaleBase* type, Note* current, int index);

static int interSteps[] = {0, 2, 4, 5, 7, 9, 11, 12};
static char* dispNote[] = {"C", "D", "E", "F", "G", "A", "B"};
static char* dispInter[] = {"Diminished", "Minor", "Major", "Augmented", "Perfect"};
static char* dispAccidental[] = {"bbbb", "bbb", "bb", "b", "", "#", "##", "###", "####"};
static modeScale modeScaleArray[] = {modeAscend, modeDescend, modeFull};


void printNote(char* prefix, Note note, char* suffix) {
    if (note.note != -1) {
        printf("%s%s%s%d%s", prefix, dispNote[note.note], dispAccidental[note.acci + 4], note.pitch, suffix);
    }
}

void printInter(char* prefix, Interval inter, char* suffix) {
    if (inter.inter != -1) {
        printf("%s%s %d%s", prefix, dispInter[inter.quality + 2], inter.inter, suffix);
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
    int root = noteToChroma(note.note);
    root += note.acci;
    int iter = (SIZE_CHROMATIC * (note.pitch - 1) + root) - STEPS_A4;
    double base;
    if (iter > 0) {
        base = TWELTH_ROOT_2;
    } else {
        iter *= -1;
        base = 1/TWELTH_ROOT_2;
    }
    double raw = 1;
    for (int i = 0; i < iter; i++) {
        raw *= base;
    }
    return type == FREQUENCY ? raw * standard : SOUND_SPEED/(raw * standard);
}

Interval returnInter(Note notea, Note noteb) {
    Interval dest = {-1, 0};
    if (notea.note == -1 || noteb.note == -1) {
        return dest;
    }
    int inter = (noteb.note + (noteb.pitch * ADJUST_NOTE)) - (notea.note + (notea.pitch * ADJUST_NOTE)) + 1;
    if (inter < 0 || inter > 15) {
        return dest;
    }
    int pitch, checkcomp;
    if (inter > 8) {
        checkcomp = 1;
        inter -= ADJUST_NOTE;
        pitch = noteb.pitch - 1;
    } else {
        pitch = noteb.pitch;
    }
    int quality = ((noteToChroma(noteb.note) + (pitch * SIZE_CHROMATIC) + noteb.acci) - (noteToChroma(notea.note) + (notea.pitch * SIZE_CHROMATIC) + notea.acci)) - interSteps[inter - 1];
    if (inter == 1 || inter == 4 || inter == 5 || inter == 8) {
        switch(quality) {
            case MAJOR:
                dest.quality = PERFECT;
                break;
            case MINOR:
                dest.quality = DIMINISHED;
                break;
            case AUGMENTED:
                dest.quality = AUGMENTED;
                break;
            default:
                return dest;
        }
    } else {
        if (quality < -2 || quality > 2) {
            return dest;
        } else {
            dest.quality = quality;
        }
    }
    if (checkcomp == 1) {
        dest.inter = inter + ADJUST_NOTE;
    } else {
        dest.inter = inter;
    }
    return dest;
}

int isEnharmonic(Note notea, Note noteb) {
    int roota = noteToChroma(notea.note);
    roota += notea.acci;
    if (roota < 1) {
        roota += ADJUST_OCTAVE;
    } else if (roota > SIZE_CHROMATIC) {
        roota -= ADJUST_OCTAVE;
    }
    roota += notea.pitch;
    int rootb = noteToChroma(noteb.note);
    rootb += noteb.acci;
    if (rootb < 1) {
        rootb += ADJUST_OCTAVE;
    } else if (rootb > SIZE_CHROMATIC) {
        rootb -= ADJUST_OCTAVE;
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
    Chord chord;
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

static int noteToChroma(int note) {
    return note > 2 ? note * 2 : note * 2 + 1;
}

static int modeCompoundInter(int* pitch, int* inter) {
    if (*inter > COMPOUND_INTER_MAX || *inter < COMPOUND_INTER_MIN || *pitch < 0) {
        return 1;
    }
    *pitch += 1;
    *inter -= ADJUST_NOTE;
    return 0;
}

static int modeSimpleInter(int* pitch, int* inter) {
    return (*inter > SIMPLE_INTER_MAX || *inter < SIMPLE_INTER_MIN || *pitch < 0) ? 1 : 0;
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
                end = -2;
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
    if (enharmonic > SIZE_NOTE) {
        enharmonic -= ADJUST_NOTE;
        dest.pitch = pitch + 1;
    } else {
        dest.pitch = pitch;
    }
    end += noteToChroma(root) + interSteps[inter - 1] + acci + quality - 1;
    if (end > SIZE_CHROMATIC_ADJUSTED) {
        end -= ADJUST_CHROMATIC;
    } else if (end < 0) {
        end += ADJUST_CHROMATIC;
    }
    if (chromatic[end].note == enharmonic) {
        dest.note = chromatic[end].note;
        dest.acci = chromatic[end].acci;
    } else {
        dest.note = enharmonic;
        dest.acci = end - (noteToChroma(enharmonic) - 1);
        if (dest.acci > SIZE_ACCI) {
            dest.acci -= ADJUST_CHROMATIC;
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
                end = -2;
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
    if (enharmonic > SIZE_NOTE) {
        enharmonic -= ADJUST_NOTE;
        dest.pitch = pitch + 1;
    } else {
        dest.pitch = pitch;
    }
    end += noteToChroma(root) + interSteps[inter - 1] + note.acci + quality - 1;
    if (end > SIZE_CHROMATIC_ADJUSTED) {
        end -= ADJUST_CHROMATIC;
    } else if (end < 0) {
        end += ADJUST_CHROMATIC;
    }
    if (chromatic[end].note == enharmonic) {
        dest.note = chromatic[end].note;
        dest.acci = chromatic[end].acci;
    } else {
        dest.note = enharmonic;
        dest.acci = end - (noteToChroma(enharmonic) - 1);
        if (dest.acci > SIZE_ACCI) {
            dest.acci -= ADJUST_CHROMATIC;
        }
    }
    return dest;
}
