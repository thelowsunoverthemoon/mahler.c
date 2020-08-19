# musictheory.c
A simple library for music theory in pure C, with Interval, Chord, and Scale functions. I tried to keep this as simple as possible, so there is no malloc or anything happening under the hood. The user of the library is in full control. All return values are enharmonically correct (ie, minor 6th of D is B flat, not A sharp) Below is the documentation. Examples are in code blocks, with the code first, and the result on the next line.

## DOCUMENTATION

### Enumerators

### Structs

### Print Functions

These functions print a Note, a Chord, or a Scale respectively. There are also options for text to be displayed before and after.

```C
void printNote(char* prefix, Note note, char* suffix);
void printChord(char* prefix, Note note, char* suffix);
void printScale(char* prefix, Note note, char* suffix);
```
#### EXAMPLE
```C
printNote("This note is ", (Note) {B, FLAT, 4}, "");

This note is Bb4
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
    printf("%f", getFreqOrWave(notea, STANDARD, FREQUENCY));
}

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
printNote(getInterStruct(
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
```

### Scale Functions

There's only one scale function : getScale. It returns a Scale struct based on the base *note* and const ScaleBase* *type* (see Structs). You must pass in an array that can fit all the notes
in the scale according to *mode* (ie, major scale (8 notes) has mode FULL (ascending and descending), so the array must fit 16). *mode* determines the format of the returned Scale. See the ScaleType enum above.

```C
Scale getScale (Note start, const ScaleBase* type, Note notes[], enum ScaleType mode);
```
#### EXAMPLE
```C
```
