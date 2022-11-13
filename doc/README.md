## Documentation

Click on each tab to see the respective documentation!

<details>
  <summary>🎷 <b>Enumerators & Macros</b> 🎷</summary>
  
#### MAHLER_CHORD_LIST_DEFAULT
```C
#define MAHLER_CHORD_LIST_DEFAULT NULL
```
Macro to use default chord list in ```returnChord```

#### MAHLER_SCALE_LIST_DEFAULT
```C
#define MAHLER_SCALE_LIST_DEFAULT NULL
```
Macro to use default chord list in ```returnScale```

#### MAHLER_DISP_LEN
```C
#define MAHLER_DISP_LEN 8
```
Macro for default print size you can use for ```printNote()```. The rationale is ```note (1) + max acci (<= 4) + number (<= 99) + null terminating (1)```

---

#### MahlerNote

```C
enum MahlerNote {
    MAHLER_C, MAHLER_D, MAHLER_E, MAHLER_F, MAHLER_G, MAHLER_A, MAHLER_B
};
```
```note``` member ```struct Note```, and represents the base note (no accidentals)

---

#### MahlerQuality

```C
enum MahlerQuality {
    MAHLER_MINOR = -1, MAHLER_MAJOR = 0, MAHLER_AUGMENTED = 1, MAHLER_DIMINISHED = -2, MAHLER_PERFECT = 3
};
```
```quality``` member of ```struct Interval```. Quality of interval

---

#### MahlerAcci

```C
enum MahlerAcci {
    MAHLER_DBFLAT = -2, MAHLER_FLAT = -1, MAHLER_NONE = 0, MAHLER_SHARP = 1, MAHLER_DBSHARP = 2
};
```
```acci``` member of ```struct Note```. Accidental of note

---

#### MahlerScaleType

```C
enum MahlerScaleType {
    MAHLER_ASCEND, MAHLER_DESCEND, MAHLER_FULL
};
```
```mode``` parameter of ```getScale()```. Determines whether it is ascending, descending, or full (both). Scales include 8th degree, and in ```SCALE_FULL``` it is doubled

---

#### MahlerKeyType

```C
enum MahlerKeyType {
    MAHLER_MAJOR_KEY, MAHLER_MINOR_KEY
};
```
```type``` parameter of ```getKeySig()``` and ```returnKeySig()```. Type of key signature

---

#### MahlerChordType

```C
enum MahlerChordType {
     MAHLER_TRIAD = 3, MAHLER_SEVENTH_CHORD = 4, MAHLER_NINTH_CHORD = 5, MAHLER_ELEVENTH_CHORD = 6
};
```
```size``` parameter of ```getKeyChord```. Size of returned chord

---

#### MahlerDegree, MahleNumeral, MahlerMode
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
Generals enums that can be used where convenient in place of numbers. For example, one could use them as the ```index``` parameter of ```getKeyChord```

---

#### MahlerError

```C
enum MahlerError {
    MAHLER_ERROR_NONE,
    MAHLER_ERROR_INVALID_QUAL, MAHLER_ERROR_INVALID_INTER, MAHLER_ERROR_INVALID_INVERSION, MAHLER_ERROR_INVALID_PRINT_NOTE,
    MAHLER_ERROR_OVERFLOW_PRINT_NOTE, MAHLER_ERROR_OVERFLOW_SCALE_RETURN, MAHLER_ERROR_OVERFLOW_CHORD_RETURN
};
```
For reference only. See [Error Handling](#err).

</details>

---

<details>
  <summary><b>🎸 Predefined 🎸</b></n></summary>

#### Predefined Scales

| Name  | Type | 
| ------------- | ------------- | 
| ```MAHLER_MAJOR_SCALE```|           Major Scale |
| ```MAHLER_NATURAL_MIN_SCALE```|     Natural Minor Scale |
| ```MAHLER_HARMONIC_MIN_SCALE```|    Harmonic Minor Scale |
| ```MAHLER_MELODIC_MIN_SCALE```|     Melodic Minor Scale |
| ```MAHLER_PENTATONIC_MAJ_SCALE```|  Major Pentatonic Scale |
| ```MAHLER_PENTATONIC_MIN_SCALE```|  Minor Pentatonic Scale |
| ```MAHLER_BLUES_SCALE```|          Blues Scale (hexatonic) |
| ```MAHLER_WHOLE_TONE_SCALE```|      Whole Tone Scale |
| ```MAHLER_OCTATONIC_HALF_SCALE```|  Octatonic Scale (starting with half tone) |
| ```MAHLER_OCTATONIC_WHOLE_SCALE```| Octatonic Scale (starting with whole tone) |

---

#### Predefined Chords

| Name  | Type | 
| ------------- | ------------- | 
| ```MAHLER_MAJOR_TRIAD```|           Major Triad |
| ```MAHLER_MINOR_TRIAD```|           Minor Triad |
| ```MAHLER_AUGMENTED_TRIAD```|       Augmented Triad |
| ```MAHLER_DIMINISHED_TRIAD```|      Diminished Triad |
| ```MAHLER_DIMINISHED_7```|          Diminished 7th |
| ```MAHLER_HALF_DIMINISHED_7```|     Half Diminished 7th |
| ```MAHLER_MINOR_7```|               Minor 7th |
| ```MAHLER_MAJOR_7```|             Major 7th |
| ```MAHLER_DOMINANT_7```|           Dominant 7th |


---

#### Predefined Chord List

Contains :
```C

MAHLER_MAJOR_TRIAD,
MAHLER_MINOR_TRIAD,
MAHLER_AUGMENTED_TRIAD,
MAHLER_DIMINISHED_TRIAD,
MAHLER_DIMINISHED_7,
MAHLER_DOMINANT_7

```

---

#### Predefined Scale List

Contains :
```C

MAHLER_MAJOR_SCALE,
MAHLER_NATURAL_MIN_SCALE,
MAHLER_HARMONIC_MIN_SCALE,
MAHLER_MELODIC_MIN_SCALE

```
</details>

---

<details>
  <summary><b>♩ Notes ♩</b></n></summary>
  
#### Note

```C
struct Note {
    enum MahlerNote note;
    int             acci;
    int             pitch;
};
```
A note in scientific pitch notation.

* **note** : base note
* **acci** : accidental (ie, G+ is 1 and G- is -2)
* **pitch** : octave the note resides in
</details>

---

<details>
  <summary><b>♫ Intervals ♫</b></n></summary>
  
#### Interval

```C
struct Interval {
    int                inter;
    enum MahlerQuality qual;
};
```
An interval.

* **inter** : interval length that must be >= than 1
* **quality** : interval quality

---

#### getInter

```C
struct Note getInter(struct Note const note, struct Interval const interval, enum MahlerError* err)
```
```getInter``` accepts all intervals (both simple and compound). Returns the destination note of ```interval``` starting from ```note```. If the given interval is an invalid quality (ie, non-perfect intervals with perfect quality, or perfect intervals with major or minor quality), then the ```err``` is set to ```MAHLER_ERROR_INVALID_QUAL```. If the length is not >= 1, it is set to ```MAHLER_ERROR_INVALID_RANGE```.

---

#### returnInter

```C
struct Interval returnInter(struct Note const noteA, struct Note const noteB, enum MahlerError* err)
```
```returnInter``` does the opposite. Given two notes, it will return the interval between them. If the resulting interval has an invalid quality, then the ```err``` is set to ```MAHLER_ERROR_INVALID_INTER```. If it is not >= 1, ```err``` is set to ```MAHLER_ERROR_INVALID_RANGE```.

</details>

---

<details>
  <summary><b>🎶 Chords 🎶</b></n></summary>
  
#### Chord
```C
struct Chord {
    int                   size;
    int                   inv;
    struct Note* restrict base;
    struct Note* restrict notes;
};
```
A chord.

* **size** : number of chord notes
* **inversion** : current inversion
* **base** : root inversion chord notes
* **notes** : current inversion chord notes specified in ```inversion``

---

#### ChordBase
```C
struct ChordBase {
    char const*      name;
    int              size;
    struct Interval* steps;
};
```
Types of chords to be used in chord functions. A number of common types have been pre-defined, but you make make your own if you wish (see Predefined).

* **name** : name of chord base
* **size** : size of chord
* **steps** : intervals between *each note* (ie, ```G -> B -> D``` is a major 3rd, then a minor 3rd)

---

#### ChordResult
```C
struct ChordResult {
    struct Note             key;
    struct ChordBase const* chord;
};
```
Entry of result from ```returnChord```

* **key** : chord base note
* **chord** : pointer to ChordBase from list

---

#### ChordResultList
```C
struct ChordResultList {
    size_t              max;
    size_t              size;
    struct ChordResult* results;
};
```
Passed to ```returnChord``` containing results.

* **max** : maximum size of ```results```
* **size** : number of entries in ```results```
* **results** : pointer to ChordResult array with matching chords

---

#### ChordCheck
```C
struct ChordCheck {
    struct ChordBase const** pos;
    size_t                   size;
    struct Note* restrict    base;
    struct Note* restrict    notes;
};
```
Passeed to ```returnChord``` with possible chord list

* **pos** : array of bases to check
* **size** : number of bases inside ```pos```
* **chordBase** : array of ```struct Note``` big enough to hold the largest chord. Cannot be the same as ```chordNotes```
* **chordNotes** : array of ```struct Note``` big enough to hold the largest chord. Cannot be the same as ```chordBase```

---

#### getChord

```C
struct Chord getChord(struct Note const root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes, enum MahlerError* err)
```
Returns a ```struct Chord``` with root ```root``` and type ```type```. You must provide two arrays of ```struct Note``` : ```base``` is for the root inversion chord (ie ```G7 is G B D F```) and ```notes``` is for the current inversion (ie ```B D F G```) specified in ```inv```. Returns error in ```err``` if the ```type``` contains invalid intervals.

---

#### returnChord

```C
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResultList* list, struct ChordCheck* custom, bool enharmonic, enum MahlerError* err)
```
Populates the ```results``` member of ```list``` with the potential chords containing every note in ```notes``` . ```noteNum``` is the number of entries in ```notes```. The ```pitch``` of each ```struct ChordResult``` note is 0. Defining ```custom``` will check for chords specified in ```struct ChordList```. Set to ```MAHLER_CHORD_LIST_DEFAULT``` is you would like to use the predefined chord list (see Predefined). ```enharmonic``` determines whether enharmonic equivalents are used (ie, Bb+ triad is also A#+ triad). If there are more possible chords than ```max``` member of ```list```, the ```err``` is set to ```MAHLER_ERROR_OVERFLOW_CHORD_RETURN```. This function tests for chords up to one accidental (ie, flat, natural, and sharp). Duplicate notes in ```notes``` are not allowed.

---

#### invertChord

```C
void invertChord(struct Chord* chord, int inv, enum MahlerError* err)
```
Inverts the ```notes``` member of ```chord``` to the ```inversion```th inversion. ```base``` is left unaltered. An inversion of 0 is considered the root inversion. Any invalid inversions will set the last error to ```MAHLER_ERROR_INVALID_INVERSION```.

</details>

---

<details>
  <summary><b>🎹 Scales 🎹</b></n></summary>

#### Scale
```C
struct Scale {
    int                  size;
    enum MahlerScaleType type;
    struct Note*         notes;
};
```
A scale.

* **size** : number of scale notes
* **type** : type of chord
* **notes** : scale notes

---

#### ScaleBase
```C
struct ScaleBase {
    char const*      name;
    int              size;
    struct Interval* steps;
};
 ```
 Types of scale to be used in scale functions. A number of common types have been pre-defined, but you make make your own if you wish (see Predefined).

* **name** : name of scale base
* **size** : size of chord
* **steps** : intervals between *each note* (ie, ```G -> B -> D``` is a major 3rd, then a minor 3rd)

#### ScaleResult
```C
struct ScaleResult {
    struct Note             key;
    struct ScaleBase const* scale;
};
```
Entry of result from ```returnScale```

* **key** : scale base note
* **scale** : pointer to ScaleBase from list

---

#### ScaleResultList
```C
struct ScaleResultList {
    size_t              max;
    size_t              size;
    struct ScaleResult* results;
};
```
Passed to ```returnScale``` containing results.

* **max** : maximum size of ```results```
* **size** : number of entries in ```results```
* **results** : pointer to ScaleResult array with matching chords

---

#### ScaleCheck
```C
struct ScaleCheck {
    struct ScaleBase const** pos;
    size_t                   size;
    struct Note*             notes;
};
```
Passeed to ```returnScale``` with possible chord list

* **pos** : array of bases to check
* **size** : number of bases inside ```pos```
* **chordBase** : array of ```struct Note``` big enough to hold the largest scale

---

#### getScale

```C
struct Scale getScale(struct Note const start, struct ScaleBase const* type, struct Note notes[], enum MahlerScaleType mode, enum MahlerError* err)
```
Returns a ```type``` scale starting on ```start```. ```notes``` contains the notes of the scale, hence the size must be >= the size member of ```type```. As well, a ```mode``` of ```MAHLER_FULL``` doubles the size requirement (ie, if it was 8, ```MAHLER_FULL``` would be 16). Returns error in ```err``` if the ```type``` contains invalid intervals.

---

#### returnScale

```C
void returnScale(struct Note const notes[], size_t noteNum, struct ScaleResultList* list, struct ScaleCheck* custom, bool enharmonic, enum MahlerError* err)
```
Identical to ```returnChord()```, but for scales.

</details>

---

<details>
  <summary><b>🎼 Key Signature 🎼</b></n></summary>
  
#### KeySig

```C
struct KeySig {
    enum MahlerKeyType type;
    int                alter;
    int                size;
    struct Note        key;
    struct Note        notes[7];
};
```

A key signature.

* **type** : major or minor
* **alter** : sum of the accidentals in the key (ie, G+ is 1 and G- is -2)
* **size** : number of accidentals in the key (ie, G+ is 1 and G- is 2)
* **key** : key note
* **notes** : key signature notes


#### getKeySig

```C
struct KeySig getKeySig(struct Note key, enum MahlerKeyType type)
```
Returns a ```struct KeySig```. All functions support theoritical keys (ie D#+).

---

#### returnKeySig

```C
struct KeySig returnKeySig(int alter, enum MahlerKeyType type)
```
Returns a ```struct KeySig``` based on ```alter``` which contains the number of accidentals in the key. Positive is sharp; negative is flat (ie 3 -> A+).

---

#### getKeyRelative

```C
struct KeySig getKeyRelative(struct KeySig const* key)
```
Returns the relative major/minor of the given key.

---

#### queryAcci

```C
int queryAcci(struct KeySig const* key, enum MahlerNote note)
```
Returns the accidental of the given ```note``` based on ```key```. Note that this is not a ```struct Note``` but the "base" note of type ```enum MahlerNote```.

---

</details>

---

<details>
  <summary><b>🥁 Misc 🥁</b></n></summary>
  
#### printNote

```C
char* printNote(struct Note const note, char buf[], size_t size, enum MahlerError* err)
```
This returns the buffer with ```note``` in text up to 4 accidentals (ie, ````bbbb -> ####````). If ```acci``` exceeds that range or the ```note``` member is invalid, the ```err``` is set to ```MAHLER_ERROR_INVALID_PRINT_NOTE```. If the given buffer is not large enough, the ```err``` is set to ```MAHLER_ERROR_OVERFLOW_PRINT_NOTE```.

---

#### isEnharmonic

```C
bool isEnharmonic(struct Note const noteA, struct Note const noteB)
```
This returns ```true``` if enharmonic, ```false``` if not. Identical notes are considered enharmonic.

</details>

---

<details>
  <summary><b>🎻 Error Handling 🎻</b></n></summary>
  
  <br>
  
If a function encounters one of the defined errors, it will return an zeroed struct if applicable, or an empty string in the case of ```printNote()```. These functions take in an ```err``` variable, which will contain the error. You can pass in ```NULL``` if do not need any error checking. A description of the error can be displayed by passing the error to ```getMahlerError```.

#### getMahlerError

```
char const* getMahlerError(enum MahlerError err)
```
Returns a string containing details of ```err```. Read each function blurb for their specific errors.

</details>

---





