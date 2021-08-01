<h1 align="center">musictheory.c</h1>

<p align="center">A simple and easy-to-use library for Western music theory in pure C99</p>

## Features

* Small & Simple - just 10 functions!
* Interval, Chord, and Scale functions
* No Memory Allocation happening under the hood
* No Accidental Limit (ie, G 20th sharp)
* Enharmonically Correct (ie, minor 6th of D is Bb, not A#)

## To-Do List

* Rename enum NoteOrder with ```NOTE``` prefix
* Add Key Signature functions
* More convenience functions (fromSemitone, more advanced scale/chord functions)

## Example

Here's an example that creates the C4 Blues Scale, ascending:

```C
struct Note notes[7];
struct Scale scale = getScale(
    (struct Note) {C, NONE, 4}, &BLUES_SCALE, notes, SCALE_ASCEND
);
```

And if you want to print it:

```C
char print[MT_DISP_LEN];
for (int i = 0; i < scale.size; i++) {
    puts(printNote(scale.notes[i], print, MT_DISP_LEN));
}
```

## Documentation
### ⬛ Table of Contents ⬛
  * [Enumerators & Macros](#enum)
  * [Structures](#struct)
  * [Interval Functions](#interval)
  * [Chord Functions](#chord)
  * [Scale Functions](#scale)
  * [Misc Functions](#misc)
  * [Error Handling](#error)

<a name="enum"/>

### 🟥 Enumerators & Macros 🟥
```C
#define MT_DISP_LEN 8
```
This is the default print size you can use for ```printNote()```. The rationale is note (1) + max acci (<= 4) + number (<= 99) + null terminating (1)
```C
enum NoteOrder {
    C, D, E, F, G, A, B
};
```
```note``` member ```struct Note```, and represents the "root" note (no accidentals).

```C
enum Quality {
    MINOR = -1, MAJOR = 0, AUGMENTED = 1, DIMINISHED = -2, PERFECT = 3
};
```
```quality``` member of ```struct Interval```. Self-explanatory.

```C
enum Accidental {
    DBFLAT = -2, FLAT = -1, NONE = 0, SHARP = 1, DBSHARP = 2
};
```
```acci``` member of ```struct Note```. Also self-explanatory. All functions support an infinite range of accidentals (ie, 30th sharp). Only thing holding you back is the size of ```enum Accidental```.
```C
enum ScaleType {
    SCALE_ASCEND, SCALE_DESCEND, SCALE_FULL
};
```
```mode``` parameter of getScale. Determines whether it is ascending, descending, or full (both). Scales include 8th degree, and in ```SCALE_FULL``` it is doubled.

```C
enum MT_Error {
    MT_ERROR_NONE,
    MT_ERROR_INVALID_QUAL, MT_ERROR_INVALID_INTER, MT_ERROR_INVALID_INVERSION, MT_ERROR_INVALID_PRINT_NOTE,
    MT_ERROR_OVERFLOW_PRINT_NOTE, MT_ERROR_OVERFLOW_SCALE_RETURN, MT_ERROR_OVERFLOW_CHORD_RETURN
};
```
For reference only. See Error Handling section down below.

<a name="struct"/>

### 🟧 Structures 🟧
```C
struct Note {
    enum NoteOrder  note;
    enum Accidental acci;
    int             pitch;
};
```
The building block of music theory. ```pitch``` is the octave the note resides in.

```C
struct Interval {
    int          inter;
    enum Quality quality;
};
```
Self explanatory.
```C
struct Chord {
    int const                         size;
    int                               inversion;
    struct Note const* const restrict base;
    struct Note* const restrict       notes;
};
```
You must provide two arrays of ```struct Note``` : ```base``` is for the root inversion chord (ie ```G7 is G B D F```) and ```notes``` is for the current inversion (ie ```B D F G```) specified in ```inversion```.
```C
struct Scale {
    int const            size;
    enum ScaleType const type;
    struct Note* const   notes;
};
```
Self-explanatory.
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
These are the "types" of Chords/Scales to be used. A number of common types have been pre-defined, but you make make your own if you wish. ```steps``` defines the intervals between *each note* (ie, ```G -> B -> D``` is a major 3rd, then a minor 3rd). ```size``` for ```struct ScaleBase``` includes the octave (ie, a major scale is size 8)

<a name="interval"/>

### 🟨 Interval Functions 🟨
```C
struct Note getInter(struct Note note, struct Interval interval);
```
```getInter``` accepts all intervals (both simple and compound). Returns the destination note of ```interval``` starting from ```note```. If the given interval is an invalid quality (ie, non-perfect intervals with perfect quality, or perfect intervals with major or minor quality), then the last error is set to ```MT_ERROR_INVALID_QUAL```.
```C
struct Interval returnInter(struct Note notea, struct Note noteb);
```
```returnInter``` does the opposite. Given two notes, it will return the interval between them. If the resulting interval is invalid (wrong quality), then the last error is set to ```MT_ERROR_INVALID_INTER```.

#### Example
```C
struct Note note = {B, DBFLAT, 4};
struct Interval inter = {4, AUGMENTED};

printf("Augmented 4th of B double flat is %s",
    printNote(getInter(note, inter), (char[MT_DISP_LEN]) {0}, MT_DISP_LEN)
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
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResult list[], size_t listMax, bool useEnharmonic);
```
This function populates ```list``` with the potential chords containing each note in ```notes``` . ```noteNum``` is the number of entries in ```notes```, while ```listMax - 1``` is the maximum number of entries to write to. The reason for -1 is because the last ```struct ChordBase``` is set empty as a looping sentinel. The ```pitch``` of each ```struct ChordResult``` note is 0. ```useEnharmonic``` determines whether enharmonic equivalents are used (ie, Bb+ triad is also A#+ triad). If there are more possible chords than ```listMax - 1```, the last error is set to ```MT_ERROR_OVERFLOW_CHORD_RETURN```. This function tests for major, minor, augmented, and diminished triads, as well as dominant and diminished 7ths up to one accidental (ie, flat, natural, and sharp).
```C
void invertChord(struct Chord* chord, int inversion);
```
This function inverts the ```notes``` member of ```chord``` to the ```inversion```th inversion. ```base``` is left unaltered. An inversion of 0 is considered the root inversion. Any invalid inversions will set the last error to ```MT_ERROR_INVALID_INVERSION```.

#### Example
```C
struct Note base[4];
struct Note notes[4];
struct Chord chord = getChord(
    (struct Note) {D, NONE, 3}, &DOMINANT_7, base, notes
);

for (size_t i = 0; i < chord.size; i++) {
    invertChord(&chord, i);
    
    char disp[MT_DISP_LEN];
    for (size_t j = 0; j < chord.size; j++) {
        printf("%s ", printNote(chord.notes[j], disp, MT_DISP_LEN));
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
Returns a ```type``` scale starting on ```start```. ```notes``` contains the notes of the scale, hence the size must be >= the size member of ```type```. As well, a ```mode``` of ```SCALE_FULL``` doubles the size requirement (ie, if it was 8, ```SCALE_FULL``` would be 16).
```C
void returnScale(struct Note const note[], size_t noteNum, struct ScaleResult list[], size_t listMax, bool useEnharmonic);
```
Identical to ```returnChord()```, but for scales. This function tests for major, natural minor, harmonic minor, and melodic minor scales up to one accidental (ie, flat, natural, and sharp).

#### Example
```C
struct ScaleResult list[5];
returnScale(
    (struct Note[]) {
        {A, FLAT, 0},
        {C, FLAT, 0},
        {G, NONE, 0}
    },
    3, list, sizeof(list) / sizeof(*list), true
);

size_t i = 0;
while (list[i].scale) {
    printf("%s %s\n",
        printNote(list[i].key, (char[MT_DISP_LEN]) {0}, MT_DISP_LEN), list[i].scale->name
    );
    i++;
}
```
#### Result
```
Ab0 Harmonic Minor
G#0 Harmonic Minor
Ab0 Melodic Minor
G#0 Melodic Minor
```

<a name="misc"/>

### 🟪 Misc Functions 🟪
```C
char* printNote(struct Note const note, char buf[], size_t size);
```
This returns the buffer with ```note``` in text up to 4 accidentals (ie, ````bbbb -> ####````). If ```acci``` exceeds that range or the ```note``` member is invalid, the last error is set to ```MT_ERROR_INVALID_PRINT_NOTE```. If the given buffer is not large enough, the last error is set to ```MT_ERROR_OVERFLOW_PRINT_NOTE```.
```C
bool isEnharmonic(struct Note notea, struct Note noteb);
```
This returns ```true``` if enharmonic, ```false``` if not. Identical notes are considered enharmonic.
#### Example
```C
struct Note noteA = {C, FLAT, 4};
struct Note noteB = {B, NONE, 3};

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
char* getMTError(void);
```
which returns a string containing details of the last error. Read each function blurb for their specific errors.
