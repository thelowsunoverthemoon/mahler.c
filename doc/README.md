## Documentation

```mahler.c``` is designed based on [minimalist libaries](https://nullprogram.com/blog/2018/06/10/). Therefore, only the building blocks of music theory are exposed, as well as no dynamic memory allocation and I/O. All exposed parts of the library are prefixed with ```mah_``` or ```MAH_``` to avoid naming conflicts. As a small note, all structs and enums are typedef'd, so you can do ```struct mah_note``` or ```mah_note``` depending on your style. Click on each tab to see the respective documentation!

<details>
  <summary>🎷 <b>Enumerators & Macros</b> 🎷</summary>
  
#### MAH_CHORD_LIST_DEFAULT
```C
#define MAH_CHORD_LIST_DEFAULT NULL
```
Macro to use default chord list in ```mah_return_chord```

---

#### MAH_SCALE_LIST_DEFAULT
```C
#define MAH_SCALE_LIST_DEFAULT NULL
```
Macro to use default chord list in ```mah_return_scale```

---

#### MAH_DISP_LEN
```C
#define MAH_DISP_LEN 8
```
Macro for default print size you can use for ```mah_write_note()```. The rationale is ```note (1) + max acci (<= 4) + number (<= 99) + null terminating (1)```

---

#### mah_tone

```C
typedef enum mah_tone {
    MAH_C, MAH_D, MAH_E, MAH_F, MAH_G, MAH_A, MAH_B
} mah_tone;
```
```tone``` member ```struct mah_note```. Represents the base tone

---

#### mah_quality

```C
typedef enum mah_quality {
    MAH_MINOR = -1, MAH_MAJOR = 0, MAH_AUGMENTED = 1, MAH_DIMINISHED = -2, MAH_PERFECT = 3
} mah_quality;
```
```quality``` member of ```struct mah_interval```. Represents the interval quality

---

#### mah_acci

```C
typedef enum mah_acci {
    MAH_DBFLAT = -2, MAH_FLAT = -1, MAH_NATURAL = 0, MAH_SHARP = 1, MAH_DBSHARP = 2
} mah_acci;
```
```acci``` member of ```struct mah_note```. Represents the accidental of note

---

#### mah_scale_type

```C
typedef enum mah_scale_type {
    MAH_ASCEND, MAH_DESCEND, MAH_FULL
} mah_scale_type;
```
```mode``` parameter of ```mah_get_scale()```. Determines whether it is ascending, descending, or full (both). Scales include 8th degree, and in ```SCALE_FULL``` it is doubled

---

#### mah_key_type

```C
typedef enum mah_key_type {
    MAHLER_MAJOR_KEY, MAHLER_MINOR_KEY
} mah_key_type;
```
```type``` parameter of ```mah_get_key_sig()``` and ```mah_return_key_sig()```. Type of key signature

---

#### mah_error

```C
typedef enum mah_error {
    MAH_ERROR_NONE,
    MAH_ERROR_INVALID_QUAL, MAH_ERROR_INVALID_INTER, MAH_ERROR_INVALID_INVERSION, MAH_ERROR_INVALID_PRINT_NOTE,
    MAH_ERROR_OVERFLOW_PRINT_NOTE, MAH_ERROR_OVERFLOW_SCALE_RETURN, MAH_ERROR_OVERFLOW_CHORD_RETURN
} mah_error;
```
For reference only. See [Error Handling](#err).

</details>

---

<details>
  <summary><b>🎸 Predefined 🎸</b></n></summary>

#### Predefined Scales

| Name  | Type | 
| ------------- | ------------- | 
| ```MAH_MAJOR_SCALE```|           Major Scale |
| ```MAH_NATURAL_MIN_SCALE```|     Natural Minor Scale |
| ```MAH_HARMONIC_MIN_SCALE```|    Harmonic Minor Scale |
| ```MAH_MELODIC_MIN_SCALE```|     Melodic Minor Scale |
| ```MAH_PENTATONIC_MAJ_SCALE```|  Major Pentatonic Scale |
| ```MAH_PENTATONIC_MIN_SCALE```|  Minor Pentatonic Scale |
| ```MAH_BLUES_SCALE```|          Blues Scale (hexatonic) |
| ```MAH_WHOLE_TONE_SCALE```|      Whole Tone Scale |
| ```MAH_OCTATONIC_HALF_SCALE```|  Octatonic Scale (starting with half tone) |
| ```MAH_OCTATONIC_WHOLE_SCALE```| Octatonic Scale (starting with whole tone) |

---

#### Predefined Chords

| Name  | Type | 
| ------------- | ------------- | 
| ```MAH_MAJOR_TRIAD```|           Major Triad |
| ```MAH_MINOR_TRIAD```|           Minor Triad |
| ```MAH_AUGMENTED_TRIAD```|       Augmented Triad |
| ```MAH_DIMINISHED_TRIAD```|      Diminished Triad |
| ```MAH_DIMINISHED_7```|          Diminished 7th |
| ```MAH_HALF_DIMINISHED_7```|     Half Diminished 7th |
| ```MAH_MINOR_7```|               Minor 7th |
| ```MAH_MAJOR_7```|             Major 7th |
| ```MAH_DOMINANT_7```|           Dominant 7th |


---

#### Predefined Chord List

Contains :
```C

MAH_MAJOR_TRIAD,
MAH_MINOR_TRIAD,
MAH_AUGMENTED_TRIAD,
MAH_DIMINISHED_TRIAD,
MAH_DIMINISHED_7,
MAH_DOMINANT_7

```

---

#### Predefined Scale List

Contains :
```C

MAH_MAJOR_SCALE,
MAH_NATURAL_MIN_SCALE,
MAH_HARMONIC_MIN_SCALE,
MAH_MELODIC_MIN_SCALE

```
</details>

---

<details>
  <summary><b>♩ Notes ♩</b></n></summary>
  
#### mah_note

```C
typedef struct mah_note {
    enum mah_tone tone
    int           acci;
    int           pitch;
} mah_note;
```
A note in scientific pitch notation.

* **tone** : base tone
* **acci** : accidental (eg, G+ is 1 and G- is -2)
* **pitch** : octave the note resides in
</details>

---

<details>
  <summary><b>♫ Intervals ♫</b></n></summary>
  
#### mah_interval

```C
typedef struct mah_interval {
    int              steps;
    enum mah_quality qual;
} mah_interval;
```
An interval.

* **steps** : interval length that must be ```>= than 1```
* **quality** : interval quality

---

#### mah_get_inter()

```C
struct mah_note mah_get_inter(struct mah_note note, struct mah_interval interval, enum mah_error* err)
```
Returns the destination note of ```interval``` starting from ```note```. Accepts both simple and compound intervals. If the given interval is an invalid quality (ie, non-perfect intervals with perfect quality, or perfect intervals with major or minor quality), then the ```err``` is set to ```MAH_ERROR_INVALID_QUAL```. If the length is not ```>= 1```, it is set to ```MAH_ERROR_INVALID_RANGE```.

---

#### mah_return_inter()

```C
struct mah_interval mah_return_inter(struct mah_note note_a, struct mah_note const note_b, enum mah_error* err)
```
Given two notes, returns the interval between them assuming ```note_a``` is the starting point to ```note_b```. If the resulting interval has an invalid quality, then the ```err``` is set to ```MAH_ERROR_INVALID_INTER```. If it is not ```>= 1```, ```err``` is set to ```MAH_ERROR_INVALID_RANGE```.

</details>

---

<details>
  <summary><b>🎶 Chords 🎶</b></n></summary>
  
#### mah_chord
```C
typedef struct mah_chord {
    int                       size;
    int                       inv;
    struct mah_note* restrict base;
    struct mah_note* restrict notes;
} mah_chord;
```
A chord.

* **size** : number of chord notes
* **inversion** : current inversion
* **base** : root inversion chord notes
* **notes** : current inversion chord notes specified in ```inversion```

---

#### mah_chord_base
```C
typedef struct mah_chord_base {
    char const*          name;
    int                  size;
    struct mah_interval* steps;
} mah_chord_base;
```
Types of chords to be used in chord functions. A number of common types have been pre-defined, but you make make your own if you wish (see Predefined).

* **name** : name of chord base
* **size** : size of chord
* **steps** : intervals between *each note* (ie, ```G -> B -> D``` is a major 3rd, then a minor 3rd)

---

#### mah_chord_result
```C
typedef struct mah_chord_result {
    struct mah_note              key;
    struct mah_chord_base const* chord;
} mah_chord_result;
```
Entry of result from ```mah_return_chord()```

* **key** : chord base note
* **chord** : pointer to mah_chord_base from list

---

#### mah_chord_result_list
```C
typedef struct mah_chord_result_list {
    int                      max;
    int                      size;
    struct mah_chord_result* results;
} mah_chord_result_list;
```
Passed to ```mah_return_chord``` containing results.

* **max** : maximum size of ```results```
* **size** : number of entries in ```results```
* **results** : pointer to mah_chord_result array with matching chords

---

#### mah_chord_check
```C
typedef struct mah_chord_check {
    struct mah_chord_base const** pos;
    int                           size;
    struct mah_note* restrict     base;
    struct mah_note* restrict     notes;
    bool                          semi[SIZE_CHROMATIC];
} mah_chord_check;
```
Passed to ```mah_return_chord()``` for possible chord list

* **pos** : array of bases to check
* **size** : number of bases inside ```pos```
* **base** : array of ```struct mah_note``` big enough to hold the largest chord. Cannot be the same pointer as ```notes```
* **notes** : array of ```struct mah_note``` big enough to hold the largest chord. Cannot be the same pointer as ```base```
* **semi** : internal use only

---

#### mah_get_chord()

```C
struct mah_chord mah_get_chord(struct mah_note root, struct mah_chord_base const* type, struct mah_note* restrict base, struct mah_note* restrict notes, enum mah_error* err)
```
Returns a ```struct mah_chord``` with root ```root``` and type ```type```. You must provide two arrays of ```struct mah_note``` : ```base``` is for the root inversion chord (ie ```G7 is G B D F```) and ```notes``` is for the current inversion (ie ```B D F G```) specified in ```inv```. Returns error in ```err``` if the ```type``` contains invalid intervals.

---

#### mah_return_chord()

```C
void mah_return_chord(struct mah_note const notes[], int num, struct mah_chord_result_list* list, struct mah_chord_check* custom, enum mah_error* err)
```
Populates the ```results``` member of ```list``` with the potential chords containing every note in ```notes``` . ```num``` is the number of entries in ```notes```. The ```pitch``` of each ```struct mah_chord_result``` note is 0. Defining ```custom``` will check for chords specified in ```struct chord_list```. Set to ```MAH_CHORD_LIST_DEFAULT``` if you would like to use the predefined chord list (see Predefined). Returned results include enharmonic results (eg, Bb+ triad is also A#+ triad), in the range of non theoretical keys. If there are more possible chords than ```max``` member of ```list```, the ```err``` is set to ```MAH_ERROR_OVERFLOW_CHORD_RETURN```. This function tests for chords up to one accidental (eg, flat, natural, and sharp).

---

#### mah_invert_chord()

```C
void mah_invert_chord(struct mah_chord* chord, int inv, enum mah_error* err)
```
Inverts the ```notes``` member of ```chord``` to the ```inversion```th inversion. ```base``` is left unaltered. An inversion of 0 is considered the root inversion. Any invalid inversions will set the last error to ```MAH_ERROR_INVALID_INVERSION```.

</details>

---

<details>
  <summary><b>🎹 Scales 🎹</b></n></summary>

#### mah_scale
```C
typedef struct mah_scale {
    int                 size;
    enum mah_scale_type type;
    struct mah_note*    notes;
} mah_scale;
```
A scale.

* **size** : number of scale notes
* **type** : type of chord
* **notes** : scale notes

---

#### mah_scale_base
```C
typedef struct mah_scale_base {
    char const*          name;
    int                  size;
    struct mah_interval* steps;
} mah_scale_base;
 ```
 Types of scale to be used in scale functions. A number of common types have been pre-defined, but you make make your own if you wish (see Predefined).

* **name** : name of scale base
* **size** : size of chord
* **steps** : intervals between *each note* (eg, ```G -> B -> D``` is a major 3rd, then a minor 3rd)

---

#### mah_scale_result
```C
typedef struct mah_scale_result {
    struct mah_note              key;
    struct mah_scale_base const* scale;
} mah_scale_result;
```
Entry of result from ```mah_return_scale()```

* **key** : scale base note
* **scale** : pointer to mah_scale_base from list

---

#### mah_scale_result_list
```C
typedef struct mah_scale_result_list {
    int                  max;
    int                  size;
    struct scale_result* results;
} mah_scale_result_list;
```
Passed to ```mah_return_scale()``` containing results.

* **max** : maximum size of ```results```
* **size** : number of entries in ```results```
* **results** : pointer to mah_scale_result array with matching chords

---

#### mah_scale_check
```C
typedef struct mah_scale_check {
    struct mah_scale_base const** pos;
    int                           size;
    struct mah_note*              notes;
    bool                          semi[SIZE_CHROMATIC];
} mah_scale_check;
```
Passeed to ```mah_return_scale()``` with possible chord list

* **pos** : array of bases to check
* **size** : number of bases inside ```pos```
* **notes** : array of ```struct mah_note``` big enough to hold the largest scale
* **semi** : internal use only

---

#### mah_get_scale()

```C
struct mah_scale mah_get_scale(struct mah_note start, struct mah_scale_base const* type, struct mah_note notes[], enum mah_scale_type mode, enum mah_error* err)
```
Returns a ```type``` scale starting on ```start```. ```notes``` contains the notes of the scale, hence the size must be >= the size member of ```type```. As well, a ```mode``` of ```MAH_FULL``` doubles the size requirement (ie, if it was 8, ```MAH_FULL``` would be 16). Returns error in ```err``` if the ```type``` contains invalid intervals.

---

#### mah_return_scale()

```C
void mah_return_scale(struct mah_note const notes[], int num, struct mah_scale_result_list* list, struct mah_scale_check* custom, enum mah_error* err)
```
Identical to ```mah_return_chord()```, but for scales.

</details>

---

<details>
  <summary><b>🎼 Key Signature 🎼</b></n></summary>
  
#### mah_key_sig

```C
typedef struct mah_key_sig {
    enum mah_key_type type;
    int               alter;
    int               size;
    struct mah_note   key;
    struct mah_note   notes[7];
} mah_key_sig;
```

A key signature.

* **type** : major or minor
* **alter** : sum of the accidentals in the key (eg, G+ is 1 and G- is -2)
* **size** : number of accidentals in the key (eg, G+ is 1 and G- is 2)
* **key** : key note
* **notes** : key signature notes

---

#### mah_get_key_sig()

```C
struct mah_key_sig mah_get_key_sig(struct mah_note key, enum mah_key_type type)
```
Returns a ```struct mah_key_sig``` base on ```key``` with type ```type```. All functions support theoritical keys (eg D#+).

---

#### mah_return_key_sig()

```C
struct mah_key_sig mah_return_key_sig(int alter, enum mah_key_type type)
```
Returns a ```struct mah_key_sig``` based on ```type``` and ```alter``` which contains the number of accidentals in the key. Positive is sharp; negative is flat (ie 3 -> A+).

---

#### mah_get_key_relative()

```C
struct mah_key_sig mah_get_key_relative(struct mah_key_sig const* key)
```
Returns the relative major/minor of the given key.

---

#### mah_query_acci()

```C
int mah_query_acci(struct mah_key_sig const* key, enum mah_tone note)
```
Returns the accidental of the given ```note``` based on ```key```. Note that this is not a ```struct mah_note``` but the tone, of type ```enum mah_tone```.

</details>

---

<details>
  <summary><b>🥁 Misc 🥁</b></n></summary>
  
#### mah_write_note()

```C
char* mah_write_note(struct mah_note const note, char buf[], int size, enum mah_error* err)
```
This returns the buffer with ```note``` in text up to 4 accidentals (ie, ````bbbb -> ####````). If ```acci``` exceeds that range or the ```note``` member is invalid, the ```err``` is set to ```MAH_ERROR_INVALID_PRINT_NOTE```. If the given buffer is not large enough, the ```err``` is set to ```MAH_ERROR_OVERFLOW_PRINT_NOTE```.

---

#### mah_is_enharmonic()

```C
bool mah_is_enharmonic(struct mah_note note_a, struct mah_note note_b)
```
This returns ```true``` if enharmonic, ```false``` if not. Identical notes are considered enharmonic.

</details>

---

<details>
  <summary><b>🎻 Error Handling 🎻</b></n></summary>
  
  <br>
  
If a function encounters one of the defined errors, it will return an zeroed struct if applicable, or an empty string in the case of ```mah_write_note()```. These functions take in an ```err``` variable, which will contain the error. You can pass in ```NULL``` if do not need any error checking. A description of the error can be displayed by passing the error to ```mah_get_error```.

#### mah_get_error()

```
char const* mah_get_error(enum mah_error err)
```
Returns a string containing details of ```err```. Read each function blurb for their specific errors.

</details>

---





