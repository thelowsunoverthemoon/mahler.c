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

See [here](ex) for more!

## Who's Mahler?

Gustav Mahler is one of my favourite composers; if you like the emotions of Wagner, and the ideas of Stravinsky, Mahler is the perfect middle ground! All his works have a flair of modernism that gives his emotional works a unique touch. You should definitely check him out, especially his Symphony No.5 in C# Minor, The Song of the Earth, and Symphony No.6 in A Minor.

## Documentation

See [here](doc/README.md)!

## Unit Tests

See [here](test)!

## Compiling

To compile, you can use ```cmake```. **Be sure to include the ```src``` and ```inc``` folders as folders to search for the source and header.** For example, if I have ```example.c``` and ```mahler``` is located in the same directory

```CMake
cmake_minimum_required(VERSION 3.10)
project(example)
set(MAHLER_PATH "${PROJECT_SOURCE_DIR}/mahler.c")

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.c)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${MAHLER_PATH}/inc"
    "${MAHLER_PATH}/src"
)

add_subdirectory(${MAHLER_PATH})
target_link_libraries(${PROJECT_NAME} PUBLIC mahler)
```

where ```MAHLER_PATH``` is the path to ```mahler```. It is also simple to do it via the commandline, but again make sure to include those folders. For example, using MSVC on the commandline

```Batch
SET "mahler[path]=mahler"

SET "mahler[inc]=%mahler[path]%/inc"
SET "mahler[src]=%mahler[path]%/src"
SET mahler[files]="%mahler[src]%/note/note.c" ^
                "%mahler[src]%/err/err.c" ^
                "%mahler[src]%/misc/misc.c" ^
                "%mahler[src]%/chord/chord.c" ^
                "%mahler[src]%/scale/scale.c" ^
                "%mahler[src]%/key/key.c" ^
                "%mahler[src]%/inter/inter.c"

cl example.c /I "%mahler[inc]%" /I "%mahler[src]%"  %mahler[files]%
```

Just change ```mahler[path]``` to the path to ```mahler``` and ```example.c``` to your source file.




