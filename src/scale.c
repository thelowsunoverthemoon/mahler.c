/*

| key.c |
defines everything related to scales
includes definition of preset scales

*/

// Enums //

enum MahlerScaleType {
    MAHLER_ASCEND, MAHLER_DESCEND, MAHLER_FULL
};

// Structures //

struct Scale {
    int                  size;
    enum MahlerScaleType type;
    struct Note*         notes;
};

struct ScaleBase {
    char const*      name;
    int              size;
    struct Interval* steps;
};

struct ScaleResult {
    struct Note             key;
    struct ScaleBase const* scale;
};

struct ScaleResultList {
    size_t              max;
    size_t              size;
    struct ScaleResult* results;
};

struct ScaleCheck {
    struct ScaleBase const** pos;
    size_t                   size;
    struct Note*             notes;
};

// Preset Scales //

struct ScaleBase const MAHLER_MAJOR_SCALE = {
    "Major", 8, (struct Interval[]) {{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{2, MAHLER_MINOR},{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{2, MAHLER_MINOR}}
};

struct ScaleBase const MAHLER_NATURAL_MIN_SCALE = {
    "Natural Minor", 8, (struct Interval[]) {{2, MAHLER_MAJOR},{2, MAHLER_MINOR},{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{2, MAHLER_MINOR},{2, MAHLER_MAJOR},{2, MAHLER_MAJOR}}
};

struct ScaleBase const MAHLER_HARMONIC_MIN_SCALE = {
    "Harmonic Minor", 8, (struct Interval[]) {{2, MAHLER_MAJOR},{2, MAHLER_MINOR},{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{2, MAHLER_MINOR},{2, MAHLER_AUGMENTED},{2, MAHLER_MINOR}}
};

struct ScaleBase const MAHLER_MELODIC_MIN_SCALE = {
    "Melodic Minor", 8, (struct Interval[]) {{2, MAHLER_MAJOR},{2, MAHLER_MINOR},{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{2, MAHLER_MINOR}}
};

struct ScaleBase const MAHLER_PENTATONIC_MAJ_SCALE = {
    "Pentatonic Major", 6, (struct Interval[]) {{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{3, MAHLER_MINOR},{2, MAHLER_MAJOR},{3, MAHLER_MINOR}}
};

struct ScaleBase const MAHLER_PENTATONIC_MIN_SCALE = {
    "Pentatonic Minor", 6, (struct Interval[]) {{3, MAHLER_MINOR},{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{3, MAHLER_MINOR},{2, MAHLER_MAJOR}}
};

struct ScaleBase const MAHLER_BLUES_SCALE = {
    "Blues", 7, (struct Interval[]) {{3, MAHLER_MINOR}, {2, MAHLER_MAJOR}, {1, MAHLER_AUGMENTED}, {2, MAHLER_MINOR}, {3, MAHLER_MINOR}, {2, MAHLER_MAJOR}}
};

struct ScaleBase const MAHLER_WHOLE_TONE_SCALE = {
    "Whole Tone", 7, (struct Interval[]) {{2, MAHLER_MAJOR}, {2, MAHLER_MAJOR}, {2, MAHLER_MAJOR}, {2, MAHLER_MAJOR}, {3, MAHLER_DIMINISHED}}
};

struct ScaleBase const MAHLER_OCTATONIC_HALF_SCALE = {
    "Octatonic Half", 9, (struct Interval[]) {{1, MAHLER_AUGMENTED}, {2, MAHLER_MAJOR}, {2, MAHLER_MINOR}, {2, MAHLER_MAJOR}, {2, MAHLER_MINOR}, {2, MAHLER_MAJOR}, {2, MAHLER_MINOR}, {2, MAHLER_MAJOR}}
};

struct ScaleBase const MAHLER_OCTATONIC_WHOLE_SCALE = {
    "Octatonic Whole", 9, (struct Interval[]) {{2, MAHLER_MAJOR}, {2, MAHLER_MINOR}, {2, MAHLER_MAJOR}, {1, MAHLER_AUGMENTED}, {2, MAHLER_MAJOR}, {2, MAHLER_MINOR}, {2, MAHLER_MAJOR}, {2, MAHLER_MINOR}}
};

// Functions //

struct Scale getScale(struct Note const start, struct ScaleBase const* type, struct Note notes[], enum MahlerScaleType mode, enum MahlerError* err);
void returnScale(struct Note const notes[], size_t noteNum, struct ScaleResultList* list, struct ScaleCheck* custom, bool enharmonic, enum MahlerError* err);

struct Scale
getScale(struct Note const start, struct ScaleBase const* type, struct Note notes[], enum MahlerScaleType const mode, enum MahlerError* err)
{
    int size = type->size;
    int retSize = size;
    switch(mode) {
        case MAHLER_DESCEND:
            notes[size - 1] = start;
            break;
        case MAHLER_ASCEND:
            notes[0] = start;
            break;
        case MAHLER_FULL:
            retSize *= 2;
            notes[0] = notes[size * 2 - 1] = start;
            break;
    }
    
    for (size_t i = 1; i < size; i++) {
        enum MahlerError scaleErr = MAHLER_ERROR_NONE;
        switch(mode) {
            case MAHLER_DESCEND:
                notes[size - i - 1] = getInter(notes[size - i], type->steps[i - 1], &scaleErr);
                break;
            case MAHLER_ASCEND:
                notes[i] = getInter(notes[i - 1], type->steps[i - 1], &scaleErr);
                break;
            case MAHLER_FULL:
                notes[i] = notes[size * 2 - i - 1] = getInter(notes[i - 1], type->steps[i - 1], &scaleErr);
                break;
        }
        if (scaleErr != MAHLER_ERROR_NONE) {
            RETURN_ERROR(Scale, scaleErr);
        }
    }
    
    return (struct Scale) {
        .size  = retSize,
        .type  = mode,
        .notes = notes
    };
}

void
returnScale(struct Note const notes[], size_t noteNum, struct ScaleResultList* list, struct ScaleCheck* custom, bool enharmonic, enum MahlerError* err)
{
    static struct ScaleBase const* scalePos[] = {
        &MAHLER_MAJOR_SCALE,
        &MAHLER_NATURAL_MIN_SCALE,
        &MAHLER_HARMONIC_MIN_SCALE,
        &MAHLER_MELODIC_MIN_SCALE
    };
    struct ScaleCheck* scaleList = custom ? custom : &(struct ScaleCheck) {
        .pos   = scalePos,
        .notes = (struct Note[8]) {0},
        .size   = SCALE_DEFAULT_NUM
    };

    for (size_t i = 0; i < scaleList->size; i++) { 
        if (scaleList->pos[i]->size - 1 < noteNum) { // - 1 b/c size includes end note
            continue;
        }
        for (enum MahlerNote n = MAHLER_C; n <= MAHLER_B; n++) {
            for (enum MahlerAcci a = MAHLER_FLAT; a <= MAHLER_SHARP; a++) {
                struct Note test = (struct Note) {
                    .note = n, .acci = a, .pitch = 0
                };
                
                enum MahlerError scaleErr = MAHLER_ERROR_NONE;
                struct Scale scale = getScale(
                    test, scaleList->pos[i], scaleList->notes, MAHLER_ASCEND, &scaleErr
                );
                if (scaleErr != MAHLER_ERROR_NONE) {
                    SET_ERROR(scaleErr);
                    return;
                }
                
                CHECK_MATCH(struct ScaleResult, scale.notes, scale.size - 1, scaleList->pos[i], MAHLER_ERROR_OVERFLOW_SCALE_RETURN);
                
            }
        }
    }
}