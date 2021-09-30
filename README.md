<h1 align="center">mahler.c</h1>

<p align="center">A simple and easy-to-use library for Western music theory in pure C99</p>

## Features

* Small & easy to compile
* Interval, Chord, Scale, and Key Signature functions
* No Memory Allocation happening under the hood
* Supports Theoretical Keys (ie Fb+)
* No Accidental Limit (ie G 20th sharp)
* Enharmonically Correct (ie minor 6th of D is Bb, not A#)

## Who's Mahler?

Gustav Mahler is one of my favourite composers; if you like the emotions of Wagner, and the ideas of Stravinsky, Mahler is the perfect middle ground! All his works have a flair of modernism that gives his emotional works a unique touch. You should definitely check him out, especially his Symphony No.5 in C# Minor, The Song of the Earth, and Symphony No.6 in A Minor.

## Example

Here's an example that creates the C4 Blues Scale, ascending:

```C
struct Note notes[7];
struct Scale scale = getScale(
    (struct Note) {MAHLER_C, MAHLER_NONE, 4}, &MAHLER_BLUES_SCALE, notes, MAHLER_ASCEND
);
```

And if you want to print it:

```C
char print[MAHLER_DISP_LEN];
for (int i = 0; i < scale.size; i++) {
    puts(printNote(scale.notes[i], print, MAHLER_DISP_LEN));
}
```

## Documentation
### ⬛ Table of Contents ⬛
  * [Enumerators & Macros](#enum)
  * [Predefined](#pre)
  * [Structures](#struct)
  * [Interval Functions](#interval)
  * [Chord Functions](#chord)
  * [Scale Functions](#scale)
  * [Key Signature Functions](#key)
  * [Misc Functions](#misc)
  * [Error Handling](#error)

<a name="enum"/>

### 🟥 Enumerators & Macros 🟥
```C
#define MAHLER_DISP_LEN 8
```
This is the default print size you can use for ```printNote()```. The rationale is note (1) + max acci (<= 4) + number (<= 99) + null terminating (1).
```C
enum MahlerNote {
    MAHLER_C, MAHLER_D, MAHLER_E, MAHLER_F, MAHLER_G, MAHLER_A, MAHLER_B
};
```
```note``` member ```struct Note```, and represents the "root" note (no accidentals).

```C
enum MahlerQuality {
    MAHLER_MINOR = -1, MAHLER_MAJOR = 0, MAHLER_AUGMENTED = 1, MAHLER_DIMINISHED = -2, MAHLER_PERFECT = 3
};
```
```quality``` member of ```struct Interval```. Self-explanatory.

```C
enum MahlerAcci {
    MAHLER_DBFLAT = -2, MAHLER_FLAT = -1, MAHLER_NONE = 0, MAHLER_SHARP = 1, MAHLER_DBSHARP = 2
};
```
```acci``` member of ```struct Note```. Also self-explanatory. All functions support an infinite range of accidentals (ie, 30th sharp). Only thing holding you back is the size of ```enum Accidental```.
```C
enum MahlerScaleType {
    MAHLER_ASCEND, MAHLER_DESCEND, MAHLER_FULL
};
```
```mode``` parameter of ```getScale()```. Determines whether it is ascending, descending, or full (both). Scales include 8th degree, and in ```SCALE_FULL``` it is doubled.
```C
enum MahlerKeyType {
    MAHLER_MAJOR_KEY, MAHLER_MINOR_KEY
};
```
```type``` parameter of ```getKeySig()``` and ```returnKeySig()```. Determines type of key signature.
```C
enum MahlerChordType {
     MAHLER_TRIAD = 3, MAHLER_SEVENTH_CHORD = 4, MAHLER_NINTH_CHORD = 5, MAHLER_ELEVENTH_CHORD = 6
};
```
```size``` parameter of ```getKeyChord```. Determines size of returned chord.
```C
enum MahlerDegree {
    MAHLER_TONIC = 1, MAHLER_SUPERTONIC = 2, MAHLER_MEDIANT = 3, MAHLER_SUBDOMINANT = 4,
    MAHLER_DOMINANT = 5, MAHLER_SUBMEDIANT = 6, MAHLER_LEADING_TONE = 7, MAHLER_SUBTONIC = 8
};

enum MahlerNumeral {
    MAHLER_I = 1, MAHLER_II = 2, MAHLER_III = 3, MAHLER_IV = 4,
    MAHLER_V = 5, MAHLER_VI = 6, MAHLER_VII = 7, MAHLER_VIII = 8
};

enum MahlerMode {
    MAHLER_IONIAN = 1, MAHLER_DORIAN = 2, MAHLER_PHRYGIAN = 3, MAHLER_LYDIAN = 4,
    MAHLER_MIXOLYDIAN = 5, MAHLER_AEOLIAN = 6, MAHLER_LOCRIAN = 7
};
```
These are generals enums that can be used where convenient in place of numbers. For example, one could use them as the ```index``` parameter of ```getKeyChord```.
```C
enum MahlerError {
    MAHLER_ERROR_NONE,
    MAHLER_ERROR_INVALID_QUAL, MAHLER_ERROR_INVALID_INTER, MAHLER_ERROR_INVALID_INVERSION, MAHLER_ERROR_INVALID_PRINT_NOTE,
    MAHLER_ERROR_OVERFLOW_PRINT_NOTE, MAHLER_ERROR_OVERFLOW_SCALE_RETURN, MAHLER_ERROR_OVERFLOW_CHORD_RETURN
};
```
For reference only. See Error Handling section down below.

<a name="pre"/>

### 🟦 Predefined 🟦
#### Predefined Scales
```C
extern struct ScaleBase const MAHLER_MAJOR_SCALE;          // Major Scale
extern struct ScaleBase const MAHLER_NATURAL_MIN_SCALE;    // Natural Minor Scale
extern struct ScaleBase const MAHLER_HARMONIC_MIN_SCALE;   // Harmonic Minor Scale
extern struct ScaleBase const MAHLER_MELODIC_MIN_SCALE;    // Melodic Minor Scale
extern struct ScaleBase const MAHLER_PENTATONIC_MAJ_SCALE; // Major Pentatonic Scale
extern struct ScaleBase const MAHLER_PENTATONIC_MIN_SCALE; // Minor Pentatonic Scale
extern struct ScaleBase const MAHLER_BLUES_SCALE;          // Blues Scale (hexatonic)
extern struct ScaleBase const MAHLER_OCTATONIC_HALF_SCALE; // Octatonic Scale (starting with half tone)
extern struct ScaleBase const MAHLER_OCTATONIC_WHOLE_SCALE;// Octatonic Scale (starting with whole tone)
```
#### Predefined Chords
```C
extern struct ChordBase const MAHLER_MAJOR_TRIAD;          // Major Triad
extern struct ChordBase const MAHLER_MINOR_TRIAD;          // Minor Triad
extern struct ChordBase const MAHLER_AUGMENTED_TRIAD;      // Augmented Triad
extern struct ChordBase const MAHLER_DIMINISHED_TRIAD;     // Diminished Triad
extern struct ChordBase const MAHLER_DIMINISHED_7;         // Diminished 7th
extern struct ChordBase const MAHLER_HALF_DIMINISHED_7;    // Half Diminished 7th
extern struct ChordBase const MAHLER_MINOR_7;              // Minor 7th
extern struct ChordBase const MAHLER_MAJOR_7;              // Major 7th
extern struct ChordBase const MAHLER_DOMINANT_7;           // Dominant 7th
```
#### Predefined Chord List
```C
{
    &MAHLER_MAJOR_TRIAD,
    &MAHLER_MINOR_TRIAD,
    &MAHLER_AUGMENTED_TRIAD,
    &MAHLER_DIMINISHED_TRIAD,
    &MAHLER_DIMINISHED_7,
    &MAHLER_DOMINANT_7
};
```
#### Predefined Scale List
```C
{
    &MAHLER_MAJOR_SCALE,
    &MAHLER_NATURAL_MIN_SCALE,
    &MAHLER_HARMONIC_MIN_SCALE,
    &MAHLER_MELODIC_MIN_SCALE
};
```
<a name="struct"/>

### 🟧 Structures 🟧
```C
struct Note {
    enum MahlerNote note;
    int             acci;
    int             pitch;
};
```
The building block of music theory. ```pitch``` is the octave the note resides in.

```C
struct Interval {
    int                inter;
    enum MahlerQuality quality;
};
```
Self explanatory.
```C
struct Chord {
    int const                size;
    int                      inversion;
    struct Note const* const base;
    struct Note* const       notes;
};
```
You must provide two arrays of ```struct Note``` : ```base``` is for the root inversion chord (ie ```G7 is G B D F```) and ```notes``` is for the current inversion (ie ```B D F G```) specified in ```inversion```.
```C
struct Scale {
    int const                  size;
    enum MahlerScaleType const type;
    struct Note* const         notes;
};
```
Self-explanatory.
```C
struct KeySig {
    enum MahlerKeyType const type;
    int const                alter; // alteration
    int const                size;
    struct Note const        key;
    struct Note              notes[7];
};
```
```alter``` is the sum of the accidentals in the key (ie, G+ is 1 and G- is -2). ```size``` is the number of accidentals in the key (ie, G+ is 1 and G- is 2). ```key``` is the key note. The pitches in ```notes``` are NOT guaranteed to be 0.
```C
struct ChordResult {
    struct Note             key;
    struct ChordBase const* chord;
};

struct ScaleResult {
    struct Note             key;
    struct ScaleBase const* scale;
};
```
Used for results of ```returnChord``` and ```returnScale```, respectively.
```C
struct ChordBase {
    char const*            name;
    int const              size;
    struct Interval const* steps;
};

struct ScaleBase {
    char const*            name;
    int const              size;
    struct Interval const* steps;
};
```
These are the "types" of Chords/Scales to be used. A number of common types have been pre-defined, but you make make your own if you wish (see [Predefined](#pre)). ```steps``` defines the intervals between *each note* (ie, ```G -> B -> D``` is a major 3rd, then a minor 3rd). ```size``` for ```struct ScaleBase``` includes the octave (ie, a major scale is size 8)
```C
struct ChordList {
    struct ChordBase const** chordPos;
    size_t const             chordNum;
    struct Note* restrict    chordBase;
    struct Note* restrict    chordNotes;
};

struct ScaleList {
    struct ScaleBase const** scalePos;
    size_t const             scaleNum;
    struct Note*             scaleArray;
};
```
These are used for custom lists when checking for chords/scales in ```returnScale()``` and ```returnChord()```. The ```*Pos``` contain pointers to an array of bases, and ```*Num``` must contain the number of pointers inside. The arrays of ```struct Note``` must be big enough to hold the largest chord/scale. In terms of ```returnScale()```, it must be big enough to hold the largest scale in ```SCALE_ASCEND``` mode.

<a name="interval"/>

### 🟨 Interval Functions 🟨
```C
struct Note getInter(struct Note note, struct Interval interval);
```
```getInter``` accepts all intervals (both simple and compound). Returns the destination note of ```interval``` starting from ```note```. If the given interval is an invalid quality (ie, non-perfect intervals with perfect quality, or perfect intervals with major or minor quality), then the last error is set to ```MAHLER_ERROR_INVALID_QUAL```.
```C
struct Interval returnInter(struct Note notea, struct Note noteb);
```
```returnInter``` does the opposite. Given two notes, it will return the interval between them. If the resulting interval is invalid (wrong quality), then the last error is set to ```MAHLER_ERROR_INVALID_INTER```.

#### Example
```C
struct Note note = {MAHLER_B, MAHLER_DBFLAT, 4};
struct Interval inter = {4, MAHLER_AUGMENTED};

printf("Augmented 4th of B double flat is %s",
    printNote(getInter(note, inter), (char[MAHLER_DISP_LEN]) {0}, MAHLER_DISP_LEN)
);
```
#### Result
```
Augmented 4th of B double flat is Eb5.
```

<a name="chord"/>

### 🟩 Chord Functions 🟩
```C
struct Chord getChord(struct Note root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes);
```
Returns a ```struct Chord``` with root ```root``` and type ```type```. You must provide two arrays of ```struct Note``` : ```base``` is for the root inversion chord (ie ```G7 is G B D F```) and ```notes``` is for the current inversion (ie ```B D F G```) specified in ```inversion```.
```C
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResult list[], size_t listMax, struct ChordList const* custom, bool enharmonic);
```
This function populates ```list``` with the potential chords containing each note in ```notes``` . ```noteNum``` is the number of entries in ```notes```, while ```listMax - 1``` is the maximum number of entries to write to. The reason for -1 is because the last ```struct ChordBase``` is set empty as a looping sentinel. The ```pitch``` of each ```struct ChordResult``` note is 0. Defining ```custom``` will check for chords specified in ```struct ChordList```. Set to ```NULL``` is you would like to use the predefined chord list (see [Predefined](#pre)). ```useEnharmonic``` determines whether enharmonic equivalents are used (ie, Bb+ triad is also A#+ triad). If there are more possible chords than ```listMax - 1```, the last error is set to ```MAHLER_ERROR_OVERFLOW_CHORD_RETURN```. This function tests for chords up to one accidental (ie, flat, natural, and sharp).
```C
void invertChord(struct Chord* chord, int inversion);
```
This function inverts the ```notes``` member of ```chord``` to the ```inversion```th inversion. ```base``` is left unaltered. An inversion of 0 is considered the root inversion. Any invalid inversions will set the last error to ```MAHLER_ERROR_INVALID_INVERSION```.
```C
struct Chord getKeyChord(struct KeySig const* key, size_t index, size_t size, struct Note* restrict base, struct Note* restrict notes);
```
Returns a ```struct Chord``` starting on ```index```th note of ```key``` (ie, index 3 of D+ would be F#) of size ```size``` (ie, 3 would return a triad). ```base``` and ```notes``` are identical to ```getChord```. See general enums of [Enumerators & Macros](#pre).
#### Example
```C
struct Note base[4];
struct Note notes[4];
struct Chord chord = getChord(
    (struct Note) {MAHLER_D, MAHLER_NONE, 3},
    &MAHLER_DOMINANT_7,
    base, notes
);

for (size_t i = 0; i < chord.size; i++) {
    invertChord(&chord, i);
    
    char disp[MAHLER_DISP_LEN];
    for (size_t j = 0; j < chord.size; j++) {
        printf("%s ", printNote(chord.notes[j], disp, MAHLER_DISP_LEN));
    }
    putchar('\n');
}
```
#### Result
```
D3 F#3 A3 C4
F#3 A3 C4 D4
A3 C4 D4 F#4
C4 D4 F#4 A4
```

### 🟦 Scale Functions 🟦

<a name="scale"/>

```C
struct Scale getScale(struct Note start, const struct ScaleBase* type, struct Note notes[], enum ScaleType mode);
```
Returns a ```type``` scale starting on ```start```. ```notes``` contains the notes of the scale, hence the size must be >= the size member of ```type```. As well, a ```mode``` of ```MAHLER_FULL``` doubles the size requirement (ie, if it was 8, ```MAHLER_FULL``` would be 16).
```C
void returnScale(struct Note const notes[], size_t noteNum, struct ScaleResult list[], size_t listMax, struct ScaleList const* custom, bool enharmonic);
```
Identical to ```returnChord()```, but for scales.
```C
struct Scale getKeyScale(struct KeySig const* key, size_t index, enum MahlerScaleType mode, struct ScaleBase const* type, struct Note* notes);
```
Returns a ```struct Scale``` starting on ```index```th note of ```key``` (ie, index 3 of D+ would be F#). In other words, it returns the ```index```th mode of a given key (ie, index 2 of C+ would be C dorian). See general enums of [Enumerators & Macros](#pre).
#### Example
```C
struct ScaleResult list[5];
returnScale(
    (struct Note[]) {
        {MAHLER_A, MAHLER_FLAT, 0},
        {MAHLER_C, MAHLER_FLAT, 0},
        {MAHLER_G, MAHLER_NONE, 0}
    },
    3, list, sizeof(list) / sizeof(*list), NULL, true
);

char disp[MAHLER_DISP_LEN];
for (size_t i = 0; list[i].scale; i++) {
    printf("%s %s\n",
        printNote(list[i].key, disp, MAHLER_DISP_LEN), list[i].scale->name
    );
}
```
#### Result
```
Ab0 Harmonic Minor
G#0 Harmonic Minor
Ab0 Melodic Minor
G#0 Melodic Minor
```

<a name="key"/>

### 🟧 Key Signature Functions 🟧
```C
struct KeySig getKeySig(struct Note key, enum MahlerKeyType type);
```
Returns a ```struct KeySig```. All Key Signature functions support theoritical keys (ie D#+).
```C
struct KeySig returnKeySig(char const* str, enum MahlerKeyType type);
```
Returns a ```struct KeySig``` based on ```str```, which contains the accidentals stringified. For example, ```"###"``` with ```MAHLER_MAJOR_KEY``` would return A+. Accepted characters are ```#``` (sharp), ```x``` (double sharp), and ```b``` (flat). Any other characters are ignored. Equivalent expressions are accepted (ie ```"x#"``` -> G+).
```C
struct KeySig getKeyRelative(struct KeySig const* key);
```
Returns the relative major/minor of the given key.
```C
int queryAcci(struct KeySig const* key, enum MahlerNote note);
```
Returns the accidental of the given ```note``` based on ```key```. Note that this is not a ```struct Note``` but the "base" note of ```enum MahlerNote```.
#### Example
```C
struct KeySig key = getKeySig(
    (struct Note) {MAHLER_G, MAHLER_NONE, 0},
    MAHLER_MINOR_KEY
);
    
char disp[MAHLER_DISP_LEN];
for (size_t i = 0; i < key.size; i++) {
    printf("%s ", printNote(key.notes[i], disp, MAHLER_DISP_LEN));
}
```
#### Result
```
Bb0 Eb0
```

<a name="misc"/>

### 🟪 Misc Functions 🟪
```C
char* printNote(struct Note const note, char buf[], size_t size);
```
This returns the buffer with ```note``` in text up to 4 accidentals (ie, ````bbbb -> ####````). If ```acci``` exceeds that range or the ```note``` member is invalid, the last error is set to ```MAHLER_ERROR_INVALID_PRINT_NOTE```. If the given buffer is not large enough, the last error is set to ```MAHLER_ERROR_OVERFLOW_PRINT_NOTE```.
```C
bool isEnharmonic(struct Note notea, struct Note noteb);
```
This returns ```true``` if enharmonic, ```false``` if not. Identical notes are considered enharmonic.
#### Example
```C
struct Note noteA = {MAHLER_C, MAHLER_FLAT, 4};
struct Note noteB = {MAHLER_B, MAHLER_NONE, 3};

if (isEnharmonic (noteA, noteB)) {
    puts("Enharmonic!");
}
```
#### Result
```
Enharmonic!
```

<a name="error"/>

### 🟫 Error Handling 🟫
If a function encounters one of the defined errors, it will return an zeroed struct if applicable, or an empty string in the case of ```printNote()```. You can then query the error through
```
char* getMahlerError(void);
```
which returns a string containing details of the last error. Read each function blurb for their specific errors.

## To Do
- [x] Add Key Signature Functions
- [x] Add More Preset Scales, add option to include own in list in ```return``` functions
- [ ] Cadences?...
