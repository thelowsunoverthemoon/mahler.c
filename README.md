# musictheory.c
A simple library for music theory in pure C, with Interval, Chord, and Scale functions. I tried to keep this as minimal as possible, so there is no malloc or anything happening under the hood - the user of the library is in full control. All return values are enharmonically correct (ie, minor 6th of D is B flat, not A sharp). Below is the documentation. Function examples are in code blocks labelled EXAMPLE, with the code first, and the result on the next line.

## DOCUMENTATION

### Enumerators
There are a couple enumerators for simplicity's sake.

```C
enum ScaleType {ASCEND, DESCEND, FULL};
```
This is for the *mode* parameter of getScale.

* ASCEND is to only return a Scale ascending
* DESCEND is to only return the Scale descending
* FULL is to return the Scale both ascending and descending

```C
enum NoteFormula {FREQUENCY, WAVELENGTH};
```
This is for the *type* parameter of getFreqOrWave.

* WAVELENGTH returns the wavelength of the Note
* FREQUENCY returns the frequency fo the Note

```C
enum PitchStandard {BAROQUE = 415, STANDARD = 440, CLASSICAL = 430};
```
These are pre-defined standards for getFreqOrWave. You can pass them in parameter *standard*, or just put your own number.

```C
enum NoteOrder {C, D, E, F, G, A, B};
```
This is for the *root* parameter of getInter, or the *note* member of the Note struct. Pretty self-explanatory.

```C
enum Quality {MINOR = -1, MAJOR = 0, AUGMENTED = 1, DIMINISHED = -2, PERFECT = 3};
```
This is for the *quality* parameter of getInter, or the *quality* member of the Interval struct. Also pretty self-explanatory.

```C
enum Accidental {DBFLAT = -2, FLAT = -1, NONE = 0, SHARP = 1, DBSHARP = 2};
```
Finally, this is for the *acci* parameter of getInter, or the *acci* member of the Note struct. Like above, pretty self-explanatory.

### Structs
```C
typedef struct {
    enum NoteOrder note;
    enum Accidental acci;
    int pitch;
} Note;
```
This is the basic Note struct. *note* represents the root note (ie A). acci represents the accidental of said note (ie FLAT). Pitch is the octave the note occupies (ie 4).

```C
typedef struct {
    int inter;
    enum Quality quality;
} Interval;
```

This is the Interval struct. *inter* is the number value of the interval (ie 5). *quality* is the quality of the interval (ie AUGMENTED),

```C
typedef struct {
    int size;
    int inversion;
    Note* base;
    Note* notes;
} Chord;
```

This is the Chord struct. *size* is the number of the notes in the Chord. *inversion* is the current inversion of the chord in *notes*. *base* is the root position of the chord. *notes* is the chord according to it's *inversion*. 

```C
typedef struct {
    int size;
    int type;
    Note* notes;
} Scale;
```

This is the Scale struct. *size* is the number of notes in the Scale. *type* is whether ASCEND, DESCEND, or FULL was used. *notes* is the scale.

```C
typedef struct {
    int size;
    Interval* steps;
} ChordBase;

typedef struct {
    int length;
    Interval* steps;
} ScaleBase;
```
These last two are for the getChord and getScale functions. They are "types" of Chords to be passed to the functions. A number of common scales and chords have been pre-defined (a list is in musictheory.h), but you can add your own and pass it in if you wish.

### Print Functions

These functions print a Note, a Chord, or a Scale respectively. There are also options for text to be displayed before and after.

```C
void printNote(char* prefix, Note note, char* suffix);
void printChord(char* prefix, Note note, char* suffix);
void printScale(char* prefix, Note note, char* suffix);
```
#### EXAMPLE
```C
printNote("This note is ", (Note) {G, SHARP, 4}, "");

This note is G#4
```

### Misc Note Functions

These are just two miscallenous functions that may be handy. isEnharmonic checks if two Notes are enharmonic, and returns 1 if true, and 0 if false.
getFreqOrWave returns the frequency (in Hertz) or wavelength (in meters) of a given Note. The parameter *standard* sets the pitch standard of A4 (there's also the PitchStandard enum defined for basic pitches), while *type*
specifies whether you want the frequency or wavelength.

```C
int isEnharmonic(Note notea, Note noteb);
double getFreqOrWave(Note note, int standard, enum NoteFormula type);
```
#### EXAMPLE
```C
Note notea = {D, DBSHARP, 4};
Note noteb = {E, NONE, 4};
if (isEnharmonic(notea, noteb)) {
    printf("The frequency is %f", getFreqOrWave(notea, STANDARD, FREQUENCY));
}

The frequency is 329.627704
```

### Interval Functions

There are two interval functions that are identical, except in parameter format. Both return the destination Note of the given interval. getInterStruct requires the Note
struct and the Interval struct, while getInter requires their members directly as parameters. The last parameter, *type*, is for the type of interval needed. (modeInter is a function pointer)

* SIMPLE is for intervals between 1-8

* COMPOUND is for intervals between 9-15

```C
Note getInter(enum NoteOrder root, enum Accidental acci, int pitch, int inter, enum Quality quality, modeInter type);
Note getInterStruct(Note note, Interval interval, modeInter type);
```
#### EXAMPLE
```C
Note note = {B, DBFLAT, 4};
Interval inter = {4, AUGMENTED};
printNote("Augmented 4th of B double flat is ", getInterStruct(note, inter, SIMPLE), ".");

Augmented 4th of B double flat is Eb5.
```

### Chord Functions

The chord functions are pretty self-explanatory. getChord returns a Chord struct starting from base *note*. The *type* is specified from const ChordBase* (see Structs), and you must
pass 2 arrays that can fit the number of chord notes (you can also read the size member of the ChordBase struct). One is for the base member of the Chord, while the other is for the notes member of the Chord. invertChord inverts the given Chord (1st inversion, 2nd inversion, ect), and also returns it. Only the Chord member notes[] is inverted;
the base is kept unchanged in case you still need the original.

```C
Chord getChord(Note note, const ChordBase* type, Note base[], Note notes[]);
Chord invertChord(Chord* chord, int inversion);
```
#### EXAMPLE
```C
Note chdbase[4];
Note chdnotes[4];
Chord chord = getChord((Note) {D, NONE, 3}, &DOMINANT7, chdbase, chdnotes);
for (int i = 0; i < chord.size; i++) {
    printChord("", invertChord(&chord, i), "\n");
}

D3 F#3 A3 C4
F#3 A3 C4 D4
A3 C4 D4 F#4
C4 D4 F#4 A4
```

### Scale Functions

There's only one scale function : getScale. It returns a Scale struct based on the base *note* and const ScaleBase* *type* (see Structs). You must pass in an array that can fit all the notes
in the scale according to *mode* (ie, major scale (8 notes) has mode FULL (ascending and descending), so the array must fit 16). *mode* determines the format of the returned Scale. See the ScaleType enum above.

```C
Scale getScale (Note start, const ScaleBase* type, Note notes[], enum ScaleType mode);
```
#### EXAMPLE
```C
Note note = {F, SHARP, 4};
Note scale[8];
printScale("Ascending : ", getScale(note, &NATURALMINSCALE, scale, ASCEND), "\n");
printScale("Descending : ", getScale(note, &NATURALMINSCALE, scale, DESCEND), "");

Ascending : F#4 G#4 A4 B4 C#5 D5 E5 F#5
Descending : F#5 E5 D5 C#5 B4 A4 G#4 F#4
```
