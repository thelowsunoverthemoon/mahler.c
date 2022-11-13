<h1 align="center">mahler.c</h1>

<p align="center">Easy-to-use library for Western music theory in C99</p>

## Features

* Small & Easy-to-use
* Interval, Chord, Scale, and Key Signature functions
* No Memory Allocation happening under the hood
* Supports Theoretical Keys (ie Fb+)
* No Accidental Limit (ie G 20th sharp)
* Enharmonically Correct (ie minor 6th of D is Bb, not A#)
* 100% Test Coverage

## Example

Here's an example that creates the C4 Blues Scale, ascending:

```C
struct Note notes[7];
struct Scale scale = getScale(
    (struct Note) {MAHLER_C, MAHLER_NONE, 4}, &MAHLER_BLUES_SCALE, notes, MAHLER_ASCEND, NULL
);
```

And if you want to print it:

```C
char print[MAHLER_DISP_LEN];
for (int i = 0; i < scale.size; i++) {
    puts(printNote(scale.notes[i], print, MAHLER_DISP_LEN, NULL));
}
```

## Who's Mahler?

Gustav Mahler is one of my favourite composers; if you like the emotions of Wagner, and the ideas of Stravinsky, Mahler is the perfect middle ground! All his works have a flair of modernism that gives his emotional works a unique touch. You should definitely check him out, especially his Symphony No.5 in C# Minor, The Song of the Earth, and Symphony No.6 in A Minor.

## Documentation

See [here](doc/README.md)!

## Test

See [here](test/README.md)!
