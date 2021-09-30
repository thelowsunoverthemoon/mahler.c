#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// To Do:
// nextNote()?
         
#define SIZE_CHROMATIC 12    // Size of normal chromatic scale
#define ADJUST_NOTE 7        // Size of note scale (enum MahlerNote)
#define SIMPLE_INTER_MAX 8   // Simple Interval maximum
#define KEYSIG_MAX 7         // Size of Circle of Fifths
#define KEYSIG_DIF 3         // Difference of Parallel Major and Minor (G+ = 1, 1 - 3 = -2 = G-)

#define RETURN_ERROR(A, B) {\
    MAHLER_LAST_ERROR = B;      \
    return (struct A) {0};  \
}

// Enumerators //

enum MahlerNote {
    MAHLER_C, MAHLER_D, MAHLER_E, MAHLER_F, MAHLER_G, MAHLER_A, MAHLER_B
};

enum MahlerQuality {
    MAHLER_MINOR = -1, MAHLER_MAJOR = 0, MAHLER_AUGMENTED = 1, MAHLER_DIMINISHED = -2, MAHLER_PERFECT = 3
};

enum MahlerAcci {
    MAHLER_DBFLAT = -2, MAHLER_FLAT = -1, MAHLER_NONE = 0, MAHLER_SHARP = 1, MAHLER_DBSHARP = 2
};

enum MahlerKeyType {
    MAHLER_MAJOR_KEY, MAHLER_MINOR_KEY
};

enum MahlerScaleType {
    MAHLER_ASCEND, MAHLER_DESCEND, MAHLER_FULL
};

enum MahlerError { // ERROR prefix to differentiate
    MAHLER_ERROR_NONE,
    MAHLER_ERROR_INVALID_QUAL, MAHLER_ERROR_INVALID_INTER, MAHLER_ERROR_INVALID_INVERSION, MAHLER_ERROR_INVALID_PRINT_NOTE,
    MAHLER_ERROR_OVERFLOW_PRINT_NOTE, MAHLER_ERROR_OVERFLOW_SCALE_RETURN, MAHLER_ERROR_OVERFLOW_CHORD_RETURN
};

// Structures //

struct Note {
    enum MahlerNote note;
    int             acci; // use int instead? Since only guaranteed enum to hold char, could have way higher flats
    int             pitch;
};

struct Interval {
    int                inter;
    enum MahlerQuality quality;
};

struct Chord {
    int const                size;
    int                      inversion;
    struct Note const* const base; // include restrict?...
    struct Note* const       notes;
};

struct ChordResult {
    struct Note             key;
    struct ChordBase const* chord;
};

struct ChordBase {
    char const*            name;
    int const              size;
    struct Interval const* steps;
};

struct ChordList {
    struct ChordBase const** chordPos;
    size_t const             chordNum;
    struct Note* restrict    chordBase;
    struct Note* restrict    chordNotes;
};

struct Scale {
    int const                  size;
    enum MahlerScaleType const type;
    struct Note* const         notes;
};

struct ScaleResult {
    struct Note             key;
    struct ScaleBase const* scale;
};

struct ScaleBase {
    char const*            name;
    int const              size;
    struct Interval const* steps;
};

struct ScaleList {
    struct ScaleBase const** scalePos;
    size_t const             scaleNum;
    struct Note*             scaleArray;
};

struct KeySig {
    enum MahlerKeyType const type;
    int const                alter; // alteration
    int const                size;
    struct Note const        key;
    struct Note              notes[7];
};

// Interval Functions //

struct Note getInter(struct Note note, struct Interval interval);
struct Interval returnInter(struct Note noteA, struct Note noteB);

// Chord Functions //

struct Chord getKeyChord(struct KeySig const* key, size_t index, size_t size, struct Note* restrict base, struct Note* restrict notes);
struct Chord getChord(struct Note root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes);
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResult list[], size_t listMax, struct ChordList const* custom, bool enharmonic);
void invertChord(struct Chord* chord, int inversion);

// Scale Functions //

struct Scale getKeyScale(struct KeySig const* key, size_t index, enum MahlerScaleType mode, struct ScaleBase const* type, struct Note* notes);
struct Scale getScale(struct Note start, const struct ScaleBase* type, struct Note notes[], enum MahlerScaleType mode);
void returnScale(struct Note const notes[], size_t noteNum, struct ScaleResult list[], size_t listMax, struct ScaleList const* custom, bool enharmonic);

// Key Signature Functions //

struct KeySig getKeySig(struct Note key, enum MahlerKeyType type);
struct KeySig returnKeySig(char const* str, enum MahlerKeyType type);
struct KeySig getKeyRelative(struct KeySig const* key);
int queryAcci(struct KeySig const* key, enum MahlerNote note);

// Misc Functions //

char* getMahlerError(void);
char* printNote(struct Note const note, char buf[], size_t size);
bool isEnharmonic(struct Note notea, struct Note noteb);

// Helper Function //

static inline enum MahlerNote nextNote(enum MahlerNote note, size_t skip);
static struct KeySig makeKeySig(struct Note key, int alter, enum MahlerKeyType type);
static inline int toSemitone(int note);
static inline struct Note getEnharmonic(struct Note const note);
static inline bool matchNote(struct Note const notes[], size_t noteNum, struct Note const scale[], size_t scaleNum);

// Preset Chords & Scales //

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

struct ScaleBase const MAHLER_PENTATONIC_MIN_SCALE = {
    "Pentatonic", 6, (struct Interval[]) {{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{3, MAHLER_MINOR},{2, MAHLER_MAJOR},{3, MAHLER_MINOR}}
};

struct ScaleBase const MAHLER_PENTATONIC_MAJ_SCALE = {
    "Pentatonic Major", 6, (struct Interval[]) {{3, MAHLER_MINOR},{2, MAHLER_MAJOR},{2, MAHLER_MAJOR},{3, MAHLER_MINOR},{2, MAHLER_MAJOR}}
};

struct ScaleBase const MAHLER_BLUES_SCALE = {
    "Blues", 7, (struct Interval[]) {{3, MAHLER_MINOR}, {2, MAHLER_MAJOR}, {1, MAHLER_AUGMENTED}, {2, MAHLER_MINOR}, {3, MAHLER_MINOR}, {2, MAHLER_MAJOR}}
};

struct ScaleBase const MAHLER_WHOLE_TONE_SCALE = {
    "Whole Tone", 7, (struct Interval[]) {{2, MAHLER_MAJOR}, {2, MAHLER_MAJOR}, {2, MAHLER_MAJOR}, {2, MAHLER_MAJOR}, {3, MAHLER_DIMINISHED}}
};

struct ScaleBase const MAHLER_OCTATONIC_HALF_SCALE = {
    "Octatonic", 9, (struct Interval[]) {{1, MAHLER_AUGMENTED}, {2, MAHLER_MAJOR}, {2, MAHLER_MINOR}, {2, MAHLER_MAJOR}, {2, MAHLER_MINOR}, {2, MAHLER_MAJOR}, {2, MAHLER_MINOR}, {2, MAHLER_MAJOR}}
};

struct ScaleBase const MAHLER_OCTATONIC_WHOLE_SCALE = {
    "Octatonic", 9, (struct Interval[]) {{2, MAHLER_MAJOR}, {2, MAHLER_MINOR}, {2, MAHLER_MAJOR}, {1, MAHLER_AUGMENTED}, {2, MAHLER_MAJOR}, {2, MAHLER_MINOR}, {2, MAHLER_MAJOR}, {2, MAHLER_MINOR}}
};

// Internal Global Variables //

static enum MahlerError MAHLER_LAST_ERROR = MAHLER_ERROR_NONE;

static int const INTER_STEPS[] = {
    0, 2, 4, 5, 7, 9, 11, 12
};

static inline enum MahlerNote
nextNote(enum MahlerNote note, size_t skip)
{
    return (note + skip) % ADJUST_NOTE;
}

static inline int
toSemitone(int const note)
{
    return note * 2 + (note <= MAHLER_E);  // "if" accounts for F (we convert from C, D, E, to C, C#, D, ect)
}

bool
isEnharmonic(struct Note const noteA, struct Note const noteB)
{
    return (toSemitone(noteA.note) + noteA.acci + noteA.pitch * SIZE_CHROMATIC) == (toSemitone(noteB.note) + noteB.acci + noteB.pitch * SIZE_CHROMATIC);
}

char*
printNote(struct Note const note, char buf[], size_t size)
{
    static char const* dispNote = "CDEFGAB";
    static char const* const dispAccidental[] = {"bbbb", "bbb", "bb", "b", "", "#", "##", "###", "####"};
    
    if (note.acci < -4 || note.acci > 4) { // dispAccidental limit
        MAHLER_LAST_ERROR = MAHLER_ERROR_INVALID_PRINT_NOTE;
        return "";
    }
    
    if (note.note < MAHLER_C || note.note > MAHLER_B) {
        MAHLER_LAST_ERROR = MAHLER_ERROR_INVALID_PRINT_NOTE;
        return "";
    }
    
    if (!(snprintf(buf, size, "%c%s%d", dispNote[note.note], dispAccidental[note.acci + 4], note.pitch) < size)) {
        MAHLER_LAST_ERROR = MAHLER_ERROR_OVERFLOW_PRINT_NOTE;
    }
    return buf;
}

void
invertChord(struct Chord* chord, int const inversion)
{
    if (inversion >= chord->size || inversion < 0) {
        MAHLER_LAST_ERROR = MAHLER_ERROR_INVALID_INVERSION;
        return;
    }
    memcpy(chord->notes, chord->base, sizeof(struct Note) * chord->size);
    
    struct Note temp;
    for (size_t j = 0; j < inversion; j++) {
        temp = chord->notes[0];
        temp.pitch += 1;
        for (size_t i = 0; i < chord->size - 1; i++) {
            chord->notes[i] = chord->notes[i + 1];
        }
        chord->notes[chord->size - 1] = temp;
    }
    
    chord->inversion = inversion;
    return;
}

struct Chord
getChord(struct Note const root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes)
{
    notes[0] = root, base[0] = root;
    for (int i = 1; i < type->size; i++) {
        notes[i] = base[i] = getInter(notes[i - 1], type->steps[i - 1]);
    }
    return (struct Chord) {
        .size      = type->size,
        .inversion = 0,
        .base      = base,
        .notes     = notes
    };
}

struct Interval
returnInter(struct Note const notea, struct Note const noteb)
{
    int inter = (noteb.note + noteb.pitch * ADJUST_NOTE) - (notea.note + notea.pitch * ADJUST_NOTE) + 1,
        pitch = inter > SIMPLE_INTER_MAX ? noteb.pitch - (inter / ADJUST_NOTE) : noteb.pitch,
        quality = (toSemitone(noteb.note) + noteb.acci + pitch * SIZE_CHROMATIC) - (toSemitone(notea.note) + notea.acci + notea.pitch * SIZE_CHROMATIC) - INTER_STEPS[inter % ADJUST_NOTE - 1];
 
    if (quality < -2 || quality > 1) {
        RETURN_ERROR(Interval, MAHLER_ERROR_INVALID_INTER);
    }
    if (inter == 1 || inter == 4 || inter == 5 || inter == 8) {
        if (quality == MAHLER_MAJOR) {
            quality = MAHLER_PERFECT;
        } else if (quality == MAHLER_MINOR) {
            quality = MAHLER_DIMINISHED;
        }
    }

    return (struct Interval) {
        .inter   = inter,
        .quality = quality
    };
}

struct Note
getInter(struct Note const note, struct Interval const interval)
{
    int inter = interval.inter % ADJUST_NOTE,
                pitch = note.pitch + (interval.inter / ADJUST_NOTE),
                root = note.note,
                quality = interval.quality;
    int end = 0;
    if (inter == 1 || inter == 4 || inter == 5 || inter == 8) { // if perfect interval
        switch (quality) {
            case MAHLER_AUGMENTED:
                end = 0;
                break;
            case MAHLER_DIMINISHED:
                end = 1;
                break;
            case MAHLER_PERFECT:
                end = -3;
                break;
            default:
                RETURN_ERROR(Note, MAHLER_ERROR_INVALID_QUAL);
        }
    } else {
        if (quality == MAHLER_PERFECT) {
            RETURN_ERROR(Note, MAHLER_ERROR_INVALID_QUAL);
        }
    }
    
    int enharmonic = root + inter - 1; // finding correct "note" with enum
    bool above = false;
    if (enharmonic >= ADJUST_NOTE) {
        above = true;
        pitch += 1;
    }
    end += toSemitone(root) + INTER_STEPS[inter - 1] + note.acci + quality - 1; // INTER_STEPS is amount of steps for major interval
    int acci = above ? end - (toSemitone(enharmonic) - 3) - (enharmonic <= 9) :
                       end - (toSemitone(enharmonic) - 1); // 7 -> 9 is C to E (see toSemitone)
    return (struct Note) {
        .note  = enharmonic - (above * ADJUST_NOTE),
        .acci  = acci,
        .pitch = pitch
    };
}

void
returnScale(struct Note const notes[], size_t noteNum, struct ScaleResult list[], size_t listMax, struct ScaleList const* custom, bool enharmonic)
{
    static struct ScaleBase const* scalePos[] = {
        &MAHLER_MAJOR_SCALE,
        &MAHLER_NATURAL_MIN_SCALE,
        &MAHLER_HARMONIC_MIN_SCALE,
        &MAHLER_MELODIC_MIN_SCALE
    };
    static struct Note scaleArray[8];
    static struct ScaleList const normal = {
        .scalePos   = scalePos,
        .scaleArray = scaleArray,
        .scaleNum   = sizeof(scalePos) / sizeof(struct ScaleBase*)
    };
    
    struct ScaleList const* scaleList = custom ? custom : &normal;
    size_t listNum = 0;
    for (size_t i = 0; i < scaleList->scaleNum; i++) { 
        if (scaleList->scalePos[i]->size - 1 < noteNum) {
            continue;
        }
        for (enum MahlerNote n = MAHLER_C; n <= MAHLER_B; n++) {
            for (enum MahlerAcci a = MAHLER_FLAT; a <= MAHLER_SHARP; a++) {
                size_t checkNote = noteNum;
                struct Scale scale = getScale(
                    (struct Note) {n, a, 0}, scaleList->scalePos[i], scaleList->scaleArray, MAHLER_ASCEND
                );
                if (matchNote(notes, noteNum, scale.notes, scale.size)) {
                    if (listNum == listMax - 1) {
                        MAHLER_LAST_ERROR = MAHLER_ERROR_OVERFLOW_SCALE_RETURN;
                        goto exit;
                    }
                    list[listNum++] = (struct ScaleResult) {
                        (struct Note) {n, a, 0}, scaleList->scalePos[i]
                    };
                    if (enharmonic && a != MAHLER_NONE) { // must be flat or sharp
                        if (listNum == listMax - 1) {
                            MAHLER_LAST_ERROR = MAHLER_ERROR_OVERFLOW_SCALE_RETURN;
                            goto exit;
                        }
                        list[listNum++] = (struct ScaleResult) {
                            getEnharmonic((struct Note) {n, a, 0}),scaleList->scalePos[i]
                        };
                    }
                }
            }
        }
    }
    
    exit:
    list[listNum] = (struct ScaleResult) {0};
}

void
returnChord(struct Note const notes[], size_t noteNum, struct ChordResult list[], size_t listMax, struct ChordList const* custom, bool enharmonic)
{
    static struct ChordBase const* chordPos[] = {
        &MAHLER_MAJOR_TRIAD,
        &MAHLER_MINOR_TRIAD,
        &MAHLER_AUGMENTED_TRIAD,
        &MAHLER_DIMINISHED_TRIAD,
        &MAHLER_DIMINISHED_7,
        &MAHLER_DOMINANT_7
    };
    static struct Note chordBase[4];
    static struct Note chordNotes[4];
    static struct ChordList const normal = {
        .chordPos   = chordPos,
        .chordNotes = chordNotes,
        .chordBase  = chordBase,
        .chordNum   = sizeof(chordPos) / sizeof(struct ChordBase*)
    };
    
    struct ChordList const* chordList = custom ? custom : &normal;
    size_t listNum = 0;
    for (size_t i = 0; i < chordList->chordNum; i++) { 
        if (chordList->chordPos[i]->size - 1 < noteNum) {
            continue;
        }
        for (enum MahlerNote n = MAHLER_C; n <= MAHLER_B; n++) {
            for (enum MahlerAcci a = MAHLER_FLAT; a <= MAHLER_SHARP; a++) {
                size_t checkNote = noteNum;
                struct Chord chord = getChord(
                    (struct Note) {n, a, 0}, chordList->chordPos[i], chordList->chordBase, chordList->chordNotes
                );
                if (matchNote(notes, noteNum, chord.notes, chord.size)) {
                    if (listNum == listMax - 1) {
                        MAHLER_LAST_ERROR = MAHLER_ERROR_OVERFLOW_CHORD_RETURN;
                        goto exit;
                    }
                    list[listNum++] = (struct ChordResult) {
                        (struct Note) {n, a, 0}, chordList->chordPos[i]
                    };
                    if (enharmonic && a != MAHLER_NONE) { // must be flat or sharp
                        if (listNum == listMax - 1) {
                            MAHLER_LAST_ERROR = MAHLER_ERROR_OVERFLOW_CHORD_RETURN;
                            goto exit;
                        }
                        list[listNum++] = (struct ChordResult) {
                            getEnharmonic((struct Note) {n, a, 0}), chordList->chordPos[i]
                        };
                    }
                }
            }
        }
    }
    
    exit:
    list[listNum] = (struct ChordResult) {0};
}

struct Scale
getScale(struct Note const start, struct ScaleBase const* type, struct Note notes[], enum MahlerScaleType const mode)
{
    int size = type->size;
    switch(mode) {
        case MAHLER_DESCEND:
            notes[size - 1] = start;
            break;
        case MAHLER_ASCEND:
            notes[0] = start;
            break;
        case MAHLER_FULL:
            size *= 2;
            notes[0] = notes[size * 2 - 1] = start;
            break;
    }
    for (size_t i = 1; i < size; i++) {
        switch(mode) {
            case MAHLER_DESCEND:
                notes[size - i - 1] = getInter(notes[size - i], type->steps[i - 1]);
                break;
            case MAHLER_ASCEND:
                notes[i] = getInter(notes[i - 1], type->steps[i - 1]);
                break;
            case MAHLER_FULL:
                notes[i] = notes[size * 2 - i - 1] = getInter(notes[i - 1], type->steps[i - 1]);
                break;
        }
    }
    return (struct Scale) {
        .size  = size,
        .type  = mode,
        .notes = notes
    };
}

struct Scale
getKeyScale(struct KeySig const* key, size_t index, enum MahlerScaleType mode, struct ScaleBase const* type, struct Note* notes)
{
    int size = type->size;
    enum MahlerNote next = nextNote(key->key.note, index - 1);
    struct Note note = (struct Note) {
        .note  = next,
        .acci  = queryAcci(key, next),
        .pitch = 0
    };
    switch(mode) {
        case MAHLER_DESCEND:
            notes[size - 1] = note;
            break;
        case MAHLER_ASCEND:
            notes[0] = note;
            break;
        case MAHLER_FULL:
            size *= 2;
            notes[0] = notes[size * 2 - 1] = note;
            break;
    }
    for (size_t i = 1; i < size; i++) {
        note.note = nextNote(note.note, type->steps[i - 1].inter - 1);
        note.acci = queryAcci(key, note.note);
        switch(mode) {
            case MAHLER_DESCEND:
                notes[size - i - 1] = note;
                break;
            case MAHLER_ASCEND:
                notes[i] = note;
                break;
            case MAHLER_FULL:
                notes[i] = notes[size * 2 - i - 1] = note;
                break;
        }
    }
    return (struct Scale) {
        .size  = size,
        .type  = mode,
        .notes = notes
    };
}

struct Chord
getKeyChord(struct KeySig const* key, size_t index, size_t size, struct Note* restrict base, struct Note* restrict notes)
{
    enum MahlerNote note = nextNote(key->key.note, index - 1);
    for (size_t i = 0; i < size; i++) {
        notes[i] = base[i] = (struct Note) {
            .note  = note,
            .acci  = queryAcci(key, note),
            .pitch = 0
        };
        note = nextNote(note, 2);
    }  
    return (struct Chord) {
        .size      = size,
        .inversion = 0,
        .base      = base,
        .notes     = notes     
    };
}

struct KeySig
getKeyRelative(struct KeySig const* key)
{
    struct Note note = getInter(
        key->key,
        key->type == MAHLER_MINOR_KEY ? (struct Interval) {3, MAHLER_MINOR} :
                                        (struct Interval) {6, MAHLER_MAJOR} 
    );
    return getKeySig(note, key->type == MAHLER_MINOR_KEY ? MAHLER_MAJOR_KEY : MAHLER_MINOR_KEY);
    
}

struct KeySig
returnKeySig(char const* str, enum MahlerKeyType const type)
{
    static enum MahlerNote const tblSharp[] = { // Major Key MahlerAcci Number + 1 (no -1)
        [1] = MAHLER_C,
        [3] = MAHLER_D,
        [5] = MAHLER_E,
        [0] = MAHLER_F,
        [2] = MAHLER_G,
        [4] = MAHLER_A,
        [6] = MAHLER_B,
    };
    static enum MahlerNote const tblFlat[] = { // For flats, no F or C
        [6] = MAHLER_D,
        [4] = MAHLER_E,
        [7] = MAHLER_G,
        [5] = MAHLER_A,
        [3] = MAHLER_B
    };
    int alter = 0;
    for ( ; *str; str++) {
        switch (*str) {
            case '#':
                alter += MAHLER_SHARP;
                break;
            case 'b':
                alter += MAHLER_FLAT;
                break;
            case 'x':
                alter += MAHLER_DBSHARP;
                break;
        }
    }
    static enum MahlerNote const* table = tblSharp;
    int note = alter;
    if ((note < 0 || note > 5) && note != -1) {
        note = alter % 7;
        if (note < 0 && note != -1) { // get absolute EXCEPT F (in tblSharp)
            table = tblFlat;
            note *= -1;
        }
    }
    struct Note key = (struct Note) {
        .note  = table[note + 1], 
        .acci  = (alter - table[note + 1]) / 7, // since sep by 7, want to get distance from .note by adjusting to 0.
        .pitch = 0
    };
    
    return makeKeySig(type == MAHLER_MAJOR ? key : getInter(key, (struct Interval) {6, MAHLER_MAJOR}), alter, type);
}

int
queryAcci(struct KeySig const* key, enum MahlerNote const note)
{
    static size_t const lkpSharp[] = { // lkp = lookup
        [MAHLER_F] = 0,
        [MAHLER_C] = 1,
        [MAHLER_G] = 2,
        [MAHLER_D] = 3,
        [MAHLER_A] = 4,
        [MAHLER_E] = 5,
        [MAHLER_B] = 6
    };
    static size_t const lkpFlat[] = {
        [MAHLER_B] = 0,
        [MAHLER_E] = 1,
        [MAHLER_A] = 2,
        [MAHLER_D] = 3,
        [MAHLER_G] = 4,
        [MAHLER_C] = 5,
        [MAHLER_F] = 6
    };
    return key->notes[key->alter < 0 ? lkpFlat[note] : lkpSharp[note]].acci;
}

struct KeySig
getKeySig(struct Note const key, enum MahlerKeyType const type)
{
    static size_t const table[] = {
        0, 2, 4, -1, 1, 3, 5, // Major Key MahlerAcci Number
    };
  
    return makeKeySig(key, table[key.note] + (KEYSIG_MAX * key.acci) - (type * KEYSIG_DIF), type); // +7 is sharp equiv, -7 is flat equiv, ect, minor equiv - 3
}

static struct KeySig
makeKeySig(struct Note const key, int const alter, enum MahlerKeyType const type)
{
    static enum MahlerNote const ordrSharp[] = {
        MAHLER_F, MAHLER_C, MAHLER_G, MAHLER_D, MAHLER_A, MAHLER_E, MAHLER_B
    };
    static enum MahlerNote const ordrFlat[] = {
        MAHLER_B, MAHLER_E, MAHLER_A, MAHLER_D, MAHLER_G, MAHLER_C, MAHLER_F
    };
    
    int noteSign = alter < 0 ? -1 : 1, noteAbs = alter * noteSign, acci = alter / KEYSIG_MAX;
    
    struct KeySig new = {
        .key   = key,
        .type  = type,
        .alter = alter,
        .size  = noteAbs > KEYSIG_MAX ? KEYSIG_MAX : noteAbs
    };
    enum MahlerNote const* ordr = alter < MAHLER_NONE ? ordrFlat : ordrSharp;
    for (size_t i = 0; i < KEYSIG_MAX; i++) { // fill in base first
        new.notes[i] = (struct Note) {
            ordr[i], acci, 0
        };
    }
    for (size_t c = 0 ; c < noteAbs % KEYSIG_MAX; c++) { // do leftovers
        new.notes[c].acci += noteSign;
    }
    
    return new;
}

static inline struct Note
getEnharmonic(struct Note const note) // only works for sharps and flats
{
    enum MahlerNote tone = note.note;
    int enharmonic = tone + note.acci;
    if (enharmonic > MAHLER_B) {
        enharmonic = MAHLER_C;
    } else if (enharmonic < MAHLER_C) {
        enharmonic = MAHLER_B;
    }
    int acci = note.acci;
    return (struct Note) {
        .note  = enharmonic,
        .acci  = -acci * ((acci == MAHLER_SHARP && tone != MAHLER_B && tone != MAHLER_E) || (acci == MAHLER_FLAT && tone != MAHLER_C && tone != MAHLER_F)),
        .pitch = 0
    };
}

static inline bool
matchNote(struct Note const notes[], size_t noteNum, struct Note const comp[], size_t compNum)
{
    size_t checkNote = noteNum;
    for (size_t s = 0; s < compNum - 1; s++) {
        for (size_t c = 0; c < noteNum; c++) {
            if (comp[s].note == notes[c].note && comp[s].acci == notes[c].acci) {
                if (!(--checkNote)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char*
getMahlerError(void)
{
    switch (MAHLER_LAST_ERROR) {
        case MAHLER_ERROR_NONE:
            return "None";
        case MAHLER_ERROR_INVALID_QUAL:
            return "Invalid Interval with Perfect MahlerQuality";
        case MAHLER_ERROR_INVALID_INTER:
            return "Notes produce Invalid Interval";
        case MAHLER_ERROR_INVALID_INVERSION:
            return "Invalid Inversion for Chord";
        case MAHLER_ERROR_INVALID_PRINT_NOTE:
            return "Out of Range MahlerAcci for Print Note";
        case MAHLER_ERROR_OVERFLOW_PRINT_NOTE:
            return "Note Text is too Large";
        case MAHLER_ERROR_OVERFLOW_SCALE_RETURN:
            return "Too many Scale Return Results";
        case MAHLER_ERROR_OVERFLOW_CHORD_RETURN:
            return "Too many Chord Return Results";
        default:
            return "Unknown Error";
    }
}
