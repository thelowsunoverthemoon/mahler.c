add_library(
    mahler
    src/err/err.c
    src/note/note.c
    src/inter/inter.c
    src/chord/chord.c
    src/scale/scale.c
    src/key/key.c
    src/misc/misc.c
)

target_include_directories(
    mahler PRIVATE
    "src"
)