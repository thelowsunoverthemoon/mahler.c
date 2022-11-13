/*

| chord.c |
defines everything related to chords
includes definition of preset chords

*/

// Structures //

struct Chord {
    int                   size;
    int                   inv;
    struct Note* restrict base;
    struct Note* restrict notes;
};

struct ChordBase {
    char const*      name;
    int              size;
    struct Interval* steps;
};

struct ChordResult {
    struct Note             key;
    struct ChordBase const* chord;
};

struct ChordResultList {
    size_t              max;
    size_t              size;
    struct ChordResult* results;
};

struct ChordCheck {
    struct ChordBase const** pos;
    size_t                   size;
    struct Note* restrict    base;
    struct Note* restrict    notes;
};

// Preset Chords //

struct ChordBase const MAHLER_MAJOR_TRIAD = {
    "Major Triad", 3, (struct Interval[]) {{3, MAHLER_MAJOR},{3, MAHLER_MINOR}}
};

struct ChordBase const MAHLER_MINOR_TRIAD = {
    "Minor Triad", 3, (struct Interval[]) {{3, MAHLER_MINOR},{3, MAHLER_MAJOR}}
};

struct ChordBase const MAHLER_AUGMENTED_TRIAD = {
    "Augmented Triad", 3, (struct Interval[]) {{3, MAHLER_MAJOR},{3, MAHLER_MAJOR}}
};

struct ChordBase const MAHLER_DIMINISHED_TRIAD = {
    "Diminished Triad", 3, (struct Interval[]) {{3, MAHLER_MINOR},{3, MAHLER_MINOR}}
};

struct ChordBase const MAHLER_DIMINISHED_7 = {
    "Diminished 7th", 4, (struct Interval[]) {{3, MAHLER_MINOR},{3, MAHLER_MINOR},{3, MAHLER_MINOR}}
};

struct ChordBase const MAHLER_HALF_DIMINISHED_7 = {
    "Half-Diminished 7th", 4, (struct Interval[]) {{3, MAHLER_MINOR},{3, MAHLER_MINOR},{3, MAHLER_MAJOR}}
};

struct ChordBase const MAHLER_MINOR_7 = {
    "Minor 7th", 4, (struct Interval[]) {{3, MAHLER_MINOR},{3, MAHLER_MAJOR},{3, MAHLER_MINOR}}
};

struct ChordBase const MAHLER_MAJOR_7 = {
    "Major 7th", 4, (struct Interval[]) {{3, MAHLER_MAJOR},{3, MAHLER_MINOR},{3, MAHLER_MAJOR}}
};

struct ChordBase const MAHLER_DOMINANT_7 = {
    "Dominant 7th", 4, (struct Interval[]) {{3, MAHLER_MAJOR},{3, MAHLER_MINOR},{3, MAHLER_MINOR}}
};

// Functions //

struct Chord getChord(struct Note const root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes, enum MahlerError* err);
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResultList* list, struct ChordCheck* custom, bool enharmonic, enum MahlerError* err);
void invertChord(struct Chord* chord, int inv, enum MahlerError* err);

struct Chord
getChord(struct Note const root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes, enum MahlerError* err)
{
    notes[0] = root, base[0] = root;
    
    for (int i = 1; i < type->size; i++) {
        enum MahlerError interErr = MAHLER_ERROR_NONE;
        notes[i] = base[i] = getInter(notes[i - 1], type->steps[i - 1], &interErr);
        if (interErr != MAHLER_ERROR_NONE) {
            RETURN_ERROR(Chord, interErr);
        }
    }
    
    return (struct Chord) {
        .size  = type->size,
        .inv   = 0,
        .base  = base,
        .notes = notes
    };
}

void
invertChord(struct Chord* chord, int const inv, enum MahlerError* err)
{
    if (inv >= chord->size || inv < 0) {
        SET_ERROR(MAHLER_ERROR_INVALID_INVERSION);
        return;
    }
    memcpy(chord->notes, chord->base, sizeof(struct Note) * chord->size);
    
    for (size_t j = 0; j < inv; j++) {
        struct Note temp = chord->notes[0];
        temp.pitch += 1;
        for (size_t i = 0; i < chord->size - 1; i++) {
            chord->notes[i] = chord->notes[i + 1];
        }
        chord->notes[chord->size - 1] = temp;
    }
    
    chord->inv = inv;
    return;
}

void
returnChord(struct Note const notes[], size_t noteNum, struct ChordResultList* list, struct ChordCheck* custom, bool enharmonic, enum MahlerError* err)
{
    static struct ChordBase const* chordPos[] = {
        &MAHLER_MAJOR_TRIAD,
        &MAHLER_MINOR_TRIAD,
        &MAHLER_AUGMENTED_TRIAD,
        &MAHLER_DIMINISHED_TRIAD,
        &MAHLER_DIMINISHED_7,
        &MAHLER_DOMINANT_7
    };
    struct ChordCheck* chordList = custom ? custom : &(struct ChordCheck) {
        .pos   = chordPos,
        .notes = (struct Note[4]) {0},
        .base  = (struct Note[4]) {0},
        .size   = CHORD_DEFAULT_NUM
    };

    for (size_t i = 0; i < chordList->size; i++) { 
        if (chordList->pos[i]->size < noteNum) {
            continue;
        }
        for (enum MahlerNote n = MAHLER_C; n <= MAHLER_B; n++) {
            for (enum MahlerAcci a = MAHLER_FLAT; a <= MAHLER_SHARP; a++) {
                struct Note test = (struct Note) {
                    .note = n, .acci = a, .pitch = 0
                };
                
                enum MahlerError chordErr = MAHLER_ERROR_NONE;
                struct Chord chord = getChord(
                    test, chordList->pos[i], chordList->base, chordList->notes, &chordErr
                );
                if (chordErr != MAHLER_ERROR_NONE) {
                    SET_ERROR(chordErr);
                    return;
                }
                
                CHECK_MATCH(struct ChordResult, chord.notes, chord.size, chordList->pos[i], MAHLER_ERROR_OVERFLOW_CHORD_RETURN);
            }
        }
    }
}
