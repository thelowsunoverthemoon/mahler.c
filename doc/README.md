## Documentation

Click on each tab to see the respective documentation!

<details>
  <summary>🟥 <b>Enumerators & Macros</b> 🟥</summary>

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
  <summary><b>🟦 Predefined 🟦</b></n></summary>

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
  <summary><b>🟨 Intervals 🟨</b></n></summary>
  
#### Interval

```C
struct Interval {
    int                inter;
    enum MahlerQuality quality;
};
```
An interval.

* **inter** : interval length that must be >= than 1
* **quality** : interval quality

---

#### getInter

```C
struct Note getInter(struct Note note, struct Interval interval, enum MahlerError* err);
```
```getInter``` accepts all intervals (both simple and compound). Returns the destination note of ```interval``` starting from ```note```. If the given interval is an invalid quality (ie, non-perfect intervals with perfect quality, or perfect intervals with major or minor quality), then the ```err``` is set to ```MAHLER_ERROR_INVALID_QUAL```. If the length is not >= 1, it is set to ```MAHLER_ERROR_INVALID_RANGE```.

---

#### returnInter

```C
struct Interval returnInter(struct Note notea, struct Note noteb, enum MahlerError* err);
```
```returnInter``` does the opposite. Given two notes, it will return the interval between them. If the resulting interval has an invalid quality, then the ```err``` is set to ```MAHLER_ERROR_INVALID_INTER```. If it is not >= 1, ```err``` is set to ```MAHLER_ERROR_INVALID_RANGE```.

---

#### Example
```C
struct Note note = {MAHLER_B, MAHLER_DBFLAT, 4};
struct Interval inter = {4, MAHLER_AUGMENTED};

printf("Augmented 4th of B double flat is %s",
    printNote(getInter(note, inter, NULL), (char[MAHLER_DISP_LEN]) {0}, MAHLER_DISP_LEN)
);
```
#### Result
```
Augmented 4th of B double flat is Eb5.
```

</details>

---

<details>
  <summary><b>🟩 Chord Functions 🟩</b></n></summary>
  
</details>
### 🟧 Structures 🟧

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

---



#### Chord

```C
struct Chord {
    int const                size;
    int                      inversion;
    struct Note const* const base;
    struct Note* const       notes;
};
```
A chord.

* **size** : number of chord notes
* **inversion** : current inversion
* **base** : root inversion chord notes
* **notes** : current inversion chord notes specified in ```inversion``

---

#### Scale

```C
struct Scale {
    int const                  size;
    enum MahlerScaleType const type;
    struct Note* const         notes;
};
```
A scale.

* **size** : number of scale notes
* **type** : type of chord
* **notes** : scale notes

---

#### KeySig

```C
struct KeySig {
    enum MahlerKeyType const type;
    int const                alter;
    int const                size;
    struct Note const        key;
    struct Note              notes[7];
};
```

A key signature.

* **type** : major or minor
* **alter** : sum of the accidentals in the key (ie, G+ is 1 and G- is -2)
* **size** : number of accidentals in the key (ie, G+ is 1 and G- is 2)
* **key** : key note
* **notes** : key signature notes

---

#### ChordResult, ScaleResult

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
Results of ```returnChord``` and ```returnScale```, respectively.

* **key** : chord/scale base note
* **chord** : pointer to ChordBase from list
* **scale** : pointer to ScaleBase from list

---

#### ChordBase, ScaleBase

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
Types of chords/scales to be used in chord and scale functions. A number of common types have been pre-defined, but you make make your own if you wish (see [Predefined](#pre)).

* **name** : name of chord/scale base
* **size** : size of chord/scale. For ```struct ScaleBase``` includes the octave (ie, a major scale is size 8)
* **steps** : intervals between *each note* (ie, ```G -> B -> D``` is a major 3rd, then a minor 3rd)

---

#### ChordList, ScaleList

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
Custom lists when checking for chords/scales in ```returnScale()``` and ```returnChord()```.

* **pos** : array of bases to check
* **num** : number of base inside ```pos```
* **chordBase** : array of ```struct Note``` big enough to hold the largest chord. Cannot be the same as ```chordNotes```
* **chordNotes** : array of ```struct Note``` big enough to hold the largest chord. Cannot be the same as ```chordBase```
* **scaleArray** : array of ```struct Note``` big enough to hold the largest scale. Must be big enough to hold the largest scale in ```SCALE_ASCEND``` mode.

---

---

<a name="chord"/>

### 🟩 Chord Functions 🟩

#### getChord

```C
struct Chord getChord(struct Note root, struct ChordBase const* type, struct Note* restrict base, struct Note* restrict notes);
```
Returns a ```struct Chord``` with root ```root``` and type ```type```. You must provide two arrays of ```struct Note``` : ```base``` is for the root inversion chord (ie ```G7 is G B D F```) and ```notes``` is for the current inversion (ie ```B D F G```) specified in ```inversion```.

---

#### returnChord

```C
void returnChord(struct Note const notes[], size_t noteNum, struct ChordResult list[], size_t listMax, struct ChordList const* custom, bool enharmonic);
```
This function populates ```list``` with the potential chords containing each note in ```notes``` . ```noteNum``` is the number of entries in ```notes```, while ```listMax - 1``` is the maximum number of entries to write to. The reason for -1 is because the last ```struct ChordBase``` is set empty as a looping sentinel. The ```pitch``` of each ```struct ChordResult``` note is 0. Defining ```custom``` will check for chords specified in ```struct ChordList```. Set to ```NULL``` is you would like to use the predefined chord list (see [Predefined](#pre)). ```useEnharmonic``` determines whether enharmonic equivalents are used (ie, Bb+ triad is also A#+ triad). If there are more possible chords than ```listMax - 1```, the last error is set to ```MAHLER_ERROR_OVERFLOW_CHORD_RETURN```. This function tests for chords up to one accidental (ie, flat, natural, and sharp).

---

#### invertChord

```C
void invertChord(struct Chord* chord, int inversion);
```
This function inverts the ```notes``` member of ```chord``` to the ```inversion```th inversion. ```base``` is left unaltered. An inversion of 0 is considered the root inversion. Any invalid inversions will set the last error to ```MAHLER_ERROR_INVALID_INVERSION```.

---

#### getKeyChord

```C
struct Chord getKeyChord(struct KeySig const* key, size_t index, size_t size, struct Note* restrict base, struct Note* restrict notes);
```
Returns a ```struct Chord``` starting on ```index```th note of ```key``` (ie, index 3 of D+ would be F#) of size ```size``` (ie, 3 would return a triad). ```base``` and ```notes``` are identical to ```getChord```. See general enums of [Enumerators & Macros](#pre).

---

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

---

### 🟦 Scale Functions 🟦

<a name="scale"/>

#### getScale

```C
struct Scale getScale(struct Note start, const struct ScaleBase* type, struct Note notes[], enum ScaleType mode);
```
Returns a ```type``` scale starting on ```start```. ```notes``` contains the notes of the scale, hence the size must be >= the size member of ```type```. As well, a ```mode``` of ```MAHLER_FULL``` doubles the size requirement (ie, if it was 8, ```MAHLER_FULL``` would be 16).

---

#### returnScale

```C
void returnScale(struct Note const notes[], size_t noteNum, struct ScaleResult list[], size_t listMax, struct ScaleList const* custom, bool enharmonic);
```
Identical to ```returnChord()```, but for scales.

---

#### getKeyScale

```C
struct Scale getKeyScale(struct KeySig const* key, size_t index, enum MahlerScaleType mode, struct ScaleBase const* type, struct Note* notes);
```
Returns a ```struct Scale``` starting on ```index```th note of ```key``` (ie, index 3 of D+ would be F#). In other words, it returns the ```index```th mode of a given key (ie, index 2 of C+ would be C dorian). See general enums of [Enumerators & Macros](#pre).

---

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

---

<a name="key"/>

### 🟧 Key Signature Functions 🟧

#### getKeySig

```C
struct KeySig getKeySig(struct Note key, enum MahlerKeyType type);
```
Returns a ```struct KeySig```. All Key Signature functions support theoritical keys (ie D#+).

---

#### returnKeySig

```C
struct KeySig returnKeySig(char const* str, enum MahlerKeyType type);
```
Returns a ```struct KeySig``` based on ```str```, which contains the accidentals stringified. For example, ```"###"``` with ```MAHLER_MAJOR_KEY``` would return A+. Accepted characters are ```#``` (sharp), ```x``` (double sharp), and ```b``` (flat). Any other characters are ignored. Equivalent expressions are accepted (ie ```"x#"``` -> A+).

---

#### getKeyRelative

```C
struct KeySig getKeyRelative(struct KeySig const* key);
```
Returns the relative major/minor of the given key.

---

#### queryAcci

```C
int queryAcci(struct KeySig const* key, enum MahlerNote note);
```
Returns the accidental of the given ```note``` based on ```key```. Note that this is not a ```struct Note``` but the "base" note of type ```enum MahlerNote```.

---

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

---

<a name="misc"/>

### 🟪 Misc Functions 🟪

#### printNote

```C
char* printNote(struct Note const note, char buf[], size_t size);
```
This returns the buffer with ```note``` in text up to 4 accidentals (ie, ````bbbb -> ####````). If ```acci``` exceeds that range or the ```note``` member is invalid, the last error is set to ```MAHLER_ERROR_INVALID_PRINT_NOTE```. If the given buffer is not large enough, the last error is set to ```MAHLER_ERROR_OVERFLOW_PRINT_NOTE```.

---

#### isEnharmonic

```C
bool isEnharmonic(struct Note notea, struct Note noteb);
```
This returns ```true``` if enharmonic, ```false``` if not. Identical notes are considered enharmonic.

---

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

---

<a name="error"/>

### 🟫 Error Handling 🟫
If a function encounters one of the defined errors, it will return an zeroed struct if applicable, or an empty string in the case of ```printNote()```. You can then query the error through
```
char* getMahlerError(void);
```
which returns a string containing details of the last error. Read each function blurb for their specific errors.

---