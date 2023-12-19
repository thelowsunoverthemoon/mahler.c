<h1 align="center">mahler.c</h1>

<div align="center">
    
<a href="">![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/thelowsunoverthemoon/mahler.c/run_unit_tests.yml)</a>
<a href="">![Static Badge](https://img.shields.io/badge/code_coverage-100%25-%23327da8)</a>
<a href="">[![Stack Overflow Badge](https://img.shields.io/badge/docs-purple)](https://github.com/thelowsunoverthemoon/mahler.c/blob/master/doc/README.md)</a>

</div>

<p align="center">Simple library for Western music theory in C99</p>



## Features

* Small & Easy-to-use
* Interval, Chord, Scale, and Key Signature functions
* No Internal Memory Allocation
* Supports Theoretical Keys (eg Fb+)
* No Accidental Limit (eg G 20th sharp)
* Enharmonically Correct (eg minor 6th of D is Bb, not A#)
* 100% Test Coverage

## Example

Here's an example that creates the C4 Blues Scale, ascending:

```C
struct mah_note notes[7];
struct mah_scale scale = mah_get_scale(
    (struct mah_note) {MAH_C, MAH_NATURAL, 4}, &MAH_BLUES_SCALE, notes, MAH_ASCEND, NULL
);
```

And if you want to print it:

```C
char buf[MAH_DISP_LEN];
for (int i = 0; i < scale.size; i++) {
    puts(mah_write_note(scale.notes[i], buf, MAH_DISP_LEN, NULL));
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

To compile, you can use ```cmake```. **Be sure to include the ```src``` and ```inc``` folders as folders to search for the source and header.** For example, given ```example.c``` and ```mahler``` is located in the same directory

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

where ```MAHLER_PATH``` is the path to ```mahler```. It is also simple to compile via commandline.




