# Key Signature Addon

# Documentation still under construction
The Key Signature addon contains a key signature struct KeySig, 3 enumerators, and 6 key signature related functions. All functions support any theoritical keys (ie G# major). However, keep in mind that the print functions have a maximum of 4 accidentals (bbbb -> ####), so while the acci value will be correct, it may not print correctly. To use this addon, define the

    __MT_KEYSIG_H__

when compiling.

## DOCUMENTATION

### Enumerators

```C
enum KeySigChordType {TYPE_TRIAD = 3, TYPE_SEVENTH_CHORD = 4};
```
This is for the *type* parameter of getKeyChord.

* TYPE_TRIAD returns a triad
* TYPE_SEVENTH_CHORD returns a 7th chord

```C
enum DispKeySig {ACCIDENTAL_ONLY, KEYSIG_ONLY, KEYSIG_AND_ACCIDENTAL};
```
This specifies the display format for printKeySig in the parameter *type*.

* ACCIDENTAL_ONLY only displays the accidentals of the KeySig
* KEYSIG_ONLY only displays the base note along with it's tonality
* KEYSIG_AND_ACCIDENTAL displays both the accidentals and the base note + tonality

```C
enum KeySigType {MAJOR_KEY, MINOR_KEY};
```
This is for *type* parameter of getKeySig and the *type* member of KeySig. Pretty self-explanatory.

```C
enum ScaleDegree {TONIC = 1, SUPERTONIC = 2, MEDIANT = 3, SUBDOMINANT = 4, DOMINANT = 5, SUBMEDIANT = 6, SUBTONIC = 7};
```
This can be used in the *num* parameter of getKeyChord, as an alternative to numbers.

### Structs

```C
typedef struct {
    enum KeyType type;
    int accinum;
    int accitype;
    Note* accilist;
    Note* note;
} KeySig;
```
The KeySig struct represents a key signature. *type* represents the tonality of the key (enum KeySigType). *accinum* specifies the number of accidentals in the key, *accitype* is a value of FLAT or SHARP, depending on the key, *accilist* is a pointer to the array of accidentals, in order (ie D+ is F# C#), and finally *note* is a pointer to the base note.

### Print Function

This function prints a KeySig, similar to the other key signature functions. However, this also has the *type* parameter to specify the format.

```C
void printKeySig(char* prefix, KeySig key, char* suffix, enum DispKeySig type);
```
#### EXAMPLE
```C
Note accilist[2];
KeySig key = getKeySig((Note) {B, FLAT, 4}, MAJOR_KEY, accilist);
printKeySig("This key is ", key, "", KEYSIG_AND_ACCIDENTAL);

This key is Bb+ : Bb Eb
```

### Get Key Signature Functions

There are 2 functions that return KeySigs. getKeySig returns a KeySig based on a note, a *type* of enum KeySigType, and an array to hold the number of accidentals in the key signature. You can determine this number using the getKeyAcci function below. The pitch of *note* is automatically set to 0. On the other hand, the getKeyRelative function returns a KeySig that is the relative major or minor of the given KeySig *key* (ie A minor --> C major). You must provide an array to hold the number of accidentals, and also the address of a Note variable (to hold the *note* member of KeySig). 

```C
KeySig getKeySig(Note note, enum KeySigType type, Note accilist[]);
KeySig getKeyRelative(KeySig key, Note accilist[], Note* note);
```
#### EXAMPLE
```C
Note majorlist[3];
KeySig major = getKeySig((Note) {A, NONE, 3}, MAJOR_KEY, majorlist);
printKeySig("", major, "", KEYSIG_ONLY);
Note minorbase;
Note minorlist[3];
printKeySig("Relative minor : ", getKeyRelative(major, minorlist, &minorbase), "", KEYSIG_ONLY);

A+
Relative minor : F#-
```
