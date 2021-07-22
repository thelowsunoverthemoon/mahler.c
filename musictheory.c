#include <stdio.h>
#include <string.h>
#include <stdbool.h>
         
#define SIZE_CHROMATIC 12       // Size of normal chromatic scale
#define ADJUST_NOTE 7           // Size of note scale (enum NoteOrder)
#define SIMPLE_INTER_MAX 8      // Simple Interval maximum

#define RETURN_ERROR(A, B) {\
    MT_LAST_ERROR = B;      \
    return (struct A) {0};  \
}

// Enumerators //

enum NoteOrder {
    C, D, E, F, G, A, B
};

enum Quality {
    MINOR = -1, MAJOR = 0, AUGMENTED = 1, DIMINISHED = -2, PERFECT = 3
};

enum Accidental {
    DBFLAT = -2, FLAT = -1, NONE = 0, SHARP = 1, DBSHARP = 2
};

enum ScaleType {
    SCALE_ASCEND, SCALE_DESCEND, SCALE_FULL
};

enum MT_Error { // change
    MT_ERROR_NONE,
    MT_ERROR_INVALID_QUAL, MT_ERROR_INVALID_INTER, MT_ERROR_INVALID_INVERSION, MT_ERROR_INVALID_PRINT_NOTE,
    MT_ERROR_OVERFLOW_PRINT_NOTE, MT_ERROR_OVERFLOW_SCALE_RETURN, MT_ERROR_OVERFLOW_CHORD_RETURN
};

// Structures //

struct Note {
    enum NoteOrder  note;
    enum Accidental acci;
    int             pitch;
};

struct Interval {
    int          inter;
    enum Quality quality;
};

struct Chord {
    int const                         size;
    int                               inversion;
    struct Note const* const restrict base;
    struct Note* const restrict       notes;
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

struct Scale {
    int const            size;
    enum ScaleType const type;
    struct Note* const   notes;
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

// Interval Functions //

struct Note getInter(struct Note note, struct Interval interval);
struct Interval returnInter(struct Note noteA, struct Note noteB);

// Chord Functions //

struct Chord getChord(struct Note root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes);
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResult list[], size_t listMax, bool useEnharmonic);
void invertChord(struct Chord* chord, int inversion);

// Scale Functions //

struct Scale getScale(struct Note start, const struct ScaleBase* type, struct Note notes[], enum ScaleType mode);
void returnScale(struct Note const notes[], size_t noteNum, struct ScaleResult list[], size_t listMax, bool useEnharmonic);

// Misc Functions //

char* getMTError(void);
char* printNote(struct Note const note, char buf[], size_t size);
bool isEnharmonic(struct Note notea, struct Note noteb);

// Helper Function //

static inline int toSemitone(int note);
static inline struct Note getEnharmonic(struct Note const note);
static inline bool matchNote(struct Note const notes[], size_t noteNum, struct Note const scale[], size_t scaleNum);

// Preset Chords & Scales //

struct ChordBase const MAJOR_TRIAD = {
    "Major Triad", 3, (struct Interval[]) {{3, MAJOR},{3, MINOR}}
};

struct ChordBase const MINOR_TRIAD = {
    "Minor Triad", 3, (struct Interval[]) {{3, MINOR},{3, MAJOR}}
};

struct ChordBase const AUGMENTED_TRIAD = {
    "Augmented Triad", 3, (struct Interval[]) {{3, MAJOR},{3, MAJOR}}
};

struct ChordBase const DIMINISHED_TRIAD = {
    "Diminished Triad", 3, (struct Interval[]) {{3, MINOR},{3, MINOR}}
};

struct ChordBase const DIMINISHED_7 = {
    "Diminished 7th", 4, (struct Interval[]) {{3, MINOR},{3, MINOR},{3, MINOR}}
};

struct ChordBase const HALF_DIMINISHED_7 = {
    "Half-Diminished 7th", 4, (struct Interval[]) {{3, MINOR},{3, MINOR},{3, MAJOR}}
};

struct ChordBase const MINOR_7 = {
    "Minor 7th", 4, (struct Interval[]) {{3, MINOR},{3, MAJOR},{3, MINOR}}
};

struct ChordBase const MAJOR_7 = {
    "Major 7th", 4, (struct Interval[]) {{3, MAJOR},{3, MINOR},{3, MAJOR}}
};

struct ChordBase const DOMINANT_7 = {
    "Dominant 7th", 4, (struct Interval[]) {{3, MAJOR},{3, MINOR},{3, MINOR}}
};

struct ScaleBase const MAJOR_SCALE = {
    "Major", 8, (struct Interval[]) {{2, MAJOR},{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MAJOR},{2, MINOR}}
};

struct ScaleBase const NATURAL_MIN_SCALE = {
    "Natural Minor", 8, (struct Interval[]) {{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR}}
};

struct ScaleBase const HARMONIC_MIN_SCALE = {
    "Harmonic Minor", 8, (struct Interval[]) {{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MINOR},{2, AUGMENTED},{2, MINOR}}
};

struct ScaleBase const MELODIC_MIN_SCALE = {
    "Melodic Minor", 8, (struct Interval[]) {{2, MAJOR},{2, MINOR},{2, MAJOR},{2, MAJOR},{2, MAJOR},{2, MAJOR},{2, MINOR}}
};

struct ScaleBase const PENTATONIC_MIN_SCALE = {
    "Pentatonic", 6, (struct Interval[]) {{2, MAJOR},{2, MAJOR},{3, MINOR},{2, MAJOR},{3, MINOR}}
};

struct ScaleBase const PENTATONIC_MAJ_SCALE = {
    "Pentatonic Major", 6, (struct Interval[]) {{3, MINOR},{2, MAJOR},{2, MAJOR},{3, MINOR},{2, MAJOR}}
};

struct ScaleBase const BLUES_SCALE = {
    "Blues", 7, (struct Interval[]) {{3, MINOR}, {2, MAJOR}, {1, AUGMENTED}, {2, MINOR}, {3, MINOR}, {2, MAJOR}}
};

// Internal Global Variables //

static enum MT_Error MT_LAST_ERROR = MT_ERROR_NONE;

static int const INTER_STEPS[] = {
    0, 2, 4, 5, 7, 9, 11, 12
};

static inline int
toSemitone(int note)
{
    return note * 2 + (note <= E);  // "if" accounts for F (we convert from C, D, E, to C, C#, D, ect)
}

char*
printNote(struct Note const note, char buf[], size_t size)
{
    static char const* const dispNote[] = {"C", "D", "E", "F", "G", "A", "B"};
    static char const* const dispAccidental[] = {"bbbb", "bbb", "bb", "b", "", "#", "##", "###", "####"};
    
    if (note.acci < -4 || note.acci > 4) { // dispAccidental limit
        MT_LAST_ERROR = MT_ERROR_INVALID_PRINT_NOTE;
        return "";
    }
    
    if (note.note < C || note.note > B) {
        MT_LAST_ERROR = MT_ERROR_INVALID_PRINT_NOTE;
        return "";
    }
    
    if (!(snprintf(buf, size, "%s%s%d", dispNote[note.note], dispAccidental[note.acci + 4], note.pitch) < size)) {
        MT_LAST_ERROR = MT_ERROR_OVERFLOW_PRINT_NOTE;
    }
    return buf;
}

void
invertChord(struct Chord* chord, int const inversion)
{
    if (inversion >= chord->size || inversion < 0) {
        MT_LAST_ERROR = MT_ERROR_INVALID_INVERSION;
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
        type->size,
        0,
        base,
        notes
    };
}

struct Interval
returnInter(struct Note const notea, struct Note const noteb)
{
	int checkComp = 0, pitch = noteb.pitch; // checkComp for compound interval
    int inter = (noteb.note + noteb.pitch * ADJUST_NOTE) - (notea.note + notea.pitch * ADJUST_NOTE) + 1;
    if (inter > SIMPLE_INTER_MAX) { // set checkComp to original interval, use for return
        checkComp = inter;
        while (inter > SIMPLE_INTER_MAX) {
            inter -= ADJUST_NOTE;
            pitch -= 1;
        }
    }
    
    int quality = (toSemitone(noteb.note) + noteb.acci + pitch * SIZE_CHROMATIC) - (toSemitone(notea.note) + notea.acci + notea.pitch * SIZE_CHROMATIC) - INTER_STEPS[inter - 1];
    if (quality < -2 || quality > 1) {
        RETURN_ERROR(Interval, MT_ERROR_INVALID_INTER);
    }
    if (inter == 1 || inter == 4 || inter == 5 || inter == 8) {
        if (quality == MAJOR) {
            quality = PERFECT;
        } else if (quality == MINOR) {
            quality = DIMINISHED;
        }
    }
    
    return (struct Interval) {
		checkComp ? checkComp :
		            inter,
		quality
	};
}

struct Note
getInter(struct Note const note, struct Interval const interval)
{
    int inter = interval.inter, pitch = note.pitch, root = note.note, quality = interval.quality;
    while (inter > SIMPLE_INTER_MAX) { // convert to simple interval
        pitch += 1;
        inter -= ADJUST_NOTE;
    }
    int end = 0;
    if (inter == 1 || inter == 4 || inter == 5 || inter == 8) { // if perfect interval
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
                RETURN_ERROR(Note, MT_ERROR_INVALID_QUAL);
        }
    } else {
        if (quality == PERFECT) {
            RETURN_ERROR(Note, MT_ERROR_INVALID_QUAL);
        }
    }
    
    int enharmonic = root + inter - 1; // finding correct "note" with enum
    bool above = false;
    if (enharmonic >= ADJUST_NOTE) {
        above = true;
        pitch += 1;
    }
    end += toSemitone(root) + INTER_STEPS[inter - 1] + note.acci + quality - 1; // INTER_STEPS is amount of steps for major interval
    int acci = above ? end - (toSemitone(enharmonic) - 3) - (enharmonic <= 9) : end - (toSemitone(enharmonic) - 1); // 7 -> 9 is C to E (see toSemitone)
    return (struct Note) {
        enharmonic - (above * ADJUST_NOTE),
        acci,
        pitch
    };
}

void
returnScale(struct Note const notes[], size_t noteNum, struct ScaleResult list[], size_t listMax, bool useEnharmonic)
{
    static struct ScaleBase const* scalePos[] = {
        &MAJOR_SCALE,
        &NATURAL_MIN_SCALE,
        &HARMONIC_MIN_SCALE,
        &MELODIC_MIN_SCALE
    };
    static struct Note scaleArray[8];
    static scalePosNum = sizeof(scalePos) / sizeof(struct ScaleBase*);

    size_t listNum = 0;
    for (size_t i = 0; i < scalePosNum; i++) { 
        if (scalePos[i]->size - 1 < noteNum) {
            continue;
        }
        for (enum NoteOrder n = C; n <= B; n++) {
            for (enum Accidental a = FLAT; a <= SHARP; a++) {
                size_t checkNote = noteNum;
                struct Scale scale = getScale(
                    (struct Note) {n, a, 0}, scalePos[i], scaleArray, SCALE_ASCEND
                );
                if (matchNote(notes, noteNum, scale.notes, scale.size)) {
                    if (listNum == listMax - 1) {
                        MT_LAST_ERROR = MT_ERROR_OVERFLOW_SCALE_RETURN;
                        goto exit;
                    }
                    list[listNum++] = (struct ScaleResult) {
                        (struct Note) {n, a, 0}, scalePos[i]
                    };
                    if (useEnharmonic && a != NONE) { // must be flat or sharp
                        if (listNum == listMax - 1) {
                            MT_LAST_ERROR = MT_ERROR_OVERFLOW_SCALE_RETURN;
                            goto exit;
                        }
                        list[listNum++] = (struct ScaleResult) {
                            getEnharmonic((struct Note) {n, a, 0}), scalePos[i]
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
returnChord(struct Note const notes[], size_t noteNum, struct ChordResult list[], size_t listMax, bool useEnharmonic)
{
    static struct ChordBase const* chordPos[] = {
        &MAJOR_TRIAD,
        &MINOR_TRIAD,
        &AUGMENTED_TRIAD,
        &DIMINISHED_TRIAD,
        &DIMINISHED_7,
        &DOMINANT_7
    };
    static struct Note chordBase[4];
    static struct Note chordNotes[4];
    static chordPosNum = sizeof(chordPos) / sizeof(struct ChordBase*);

    size_t listNum = 0;
    for (size_t i = 0; i < chordPosNum; i++) { 
        if (chordPos[i]->size - 1 < noteNum) {
            continue;
        }
        for (enum NoteOrder n = C; n <= B; n++) {
            for (enum Accidental a = FLAT; a <= SHARP; a++) {
                size_t checkNote = noteNum;
                struct Chord chord = getChord(
                    (struct Note) {n, a, 0}, chordPos[i], chordBase, chordNotes
                );
                if (matchNote(notes, noteNum, chord.notes, chord.size)) {
                    if (listNum == listMax - 1) {
                        MT_LAST_ERROR = MT_ERROR_OVERFLOW_CHORD_RETURN;
                        goto exit;
                    }
                    list[listNum++] = (struct ChordResult) {
                        (struct Note) {n, a, 0}, chordPos[i]
                    };
                    if (useEnharmonic && a != NONE) { // must be flat or sharp
                        if (listNum == listMax - 1) {
                            MT_LAST_ERROR = MT_ERROR_OVERFLOW_CHORD_RETURN;
                            goto exit;
                        }
                        list[listNum++] = (struct ChordResult) {
                            getEnharmonic((struct Note) {n, a, 0}), chordPos[i]
                        };
                    }
                }
            }
        }
    }
    
    exit:
    list[listNum] = (struct ChordResult) {0};
}

static inline struct Note
getEnharmonic(struct Note const note) // only works for sharps and flats
{
    enum NoteOrder tone = note.note;
    int enharmonic = tone + note.acci;
    if (enharmonic > B) {
        enharmonic = C;
    } else if (enharmonic < C) {
        enharmonic = B;
    }
    enum Accidental acci = note.acci;
    return (struct Note) {
        enharmonic,
        -acci * ((acci == SHARP && tone != B && tone != E) || (acci == FLAT && tone != C && tone != F)),
        0
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

struct Scale
getScale(struct Note const start, struct ScaleBase const* type, struct Note notes[], enum ScaleType const mode)
{
    int size = type->size;
    switch(mode) {
        case SCALE_DESCEND:
            notes[type->size - 1] = start;
            break;
        case SCALE_ASCEND:
            notes[0] = start;
            break;
        case SCALE_FULL:
            size *= 2;
            notes[0] = notes[type->size * 2 - 1] = start;
            break;
    }
    for (size_t i = 1; i < type->size; i++) {
        switch(mode) {
            case SCALE_DESCEND:
                notes[type->size - i - 1] = getInter(notes[type->size - i], type->steps[i - 1]);
                break;
            case SCALE_ASCEND:
                notes[i] = getInter(notes[i - 1], type->steps[i - 1]);
                break;
            case SCALE_FULL:
                notes[i] = notes[type->size * 2 - i - 1] = getInter(notes[i - 1], type->steps[i - 1]);
                break;
        }
    }
    return (struct Scale) {
		size,
		mode,
		notes
	};
}

bool
isEnharmonic(struct Note const noteA, struct Note const noteB)
{
    return (toSemitone(noteA.note) + noteA.acci + noteA.pitch * SIZE_CHROMATIC) == (toSemitone(noteB.note) + noteB.acci + noteB.pitch * SIZE_CHROMATIC);
}

char*
getMTError(void)
{
    switch (MT_LAST_ERROR) {
        case MT_ERROR_NONE:
            return "None";
        case MT_ERROR_INVALID_QUAL:
            return "Invalid Interval with Perfect Quality";
        case MT_ERROR_INVALID_INTER:
            return "Notes produce Invalid Interval";
        case MT_ERROR_INVALID_INVERSION:
            return "Invalid Inversion for Chord";
        case MT_ERROR_INVALID_PRINT_NOTE:
            return "Out of Range Accidental for Print Note";
        case MT_ERROR_OVERFLOW_PRINT_NOTE:
            return "Note Text is too Large";
        case MT_ERROR_OVERFLOW_SCALE_RETURN:
            return "Too many Scale Return Results";
        default:
            return "Unknown Error";
    }
}
