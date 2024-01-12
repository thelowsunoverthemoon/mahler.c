#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "mahler.h"
#include "macros.h"

// Global Test Variable //

struct Test {
    int pass;
    int total;
};

struct Test TEST = {0};
enum mah_error ERR = MAH_ERROR_NONE;

// Function Prototypes //

void assert(int val, int line, char* file, char* expr);

bool comp_note(struct mah_note note_a, struct mah_note note_b);
bool comp_notes(struct mah_note const* notes_a, struct mah_note const* notes_b, int size_a, int size_b);

bool comp_interval(struct mah_interval inter_a, struct mah_interval inter_b);
bool comp_inters(struct mah_interval const* inters_a, struct mah_interval const* inters_b, int size_a, int size_b);

bool comp_key_sig(struct mah_key_sig key_a, struct mah_key_sig key_b);

bool comp_chord(struct mah_chord chord_a, struct mah_chord chord_b);
bool comp_chord_result_list(struct mah_chord_result_list list_a, struct mah_chord_result_list list_b);
bool comp_chord_result(struct mah_chord_result result_a, struct mah_chord_result result_b);
bool comp_chord_results(struct mah_chord_result* results_a, struct mah_chord_result* results_b, int size_a, int size_b);
bool comp_chord_base(struct mah_chord_base const* base_a, struct mah_chord_base const* base_b);

bool comp_scale(struct mah_scale scale_a, struct mah_scale scale_b);
bool comp_scale_result_list(struct mah_scale_result_list list_a, struct mah_scale_result_list list_b);
bool comp_scale_result(struct mah_scale_result result_a, struct mah_scale_result result_b);
bool comp_scale_results(struct mah_scale_result* results_a, struct  mah_scale_result* results_b, int size_a, int size_b);
bool comp_scale_base(struct mah_scale_base const* base_a, struct mah_scale_base const* base_b);

int
main(void)
{
    
    #include "suites/inter/mah_return_inter.test"
    #include "suites/inter/mah_get_inter.test"
    
    #include "suites/key/mah_get_key_sig.test"
    #include "suites/key/mah_return_key_sig.test"
    #include "suites/key/mah_get_key_relative.test"
    #include "suites/key/mah_query_acci.test"

    #include "suites/misc/mah_is_enharmonic.test"
    #include "suites/misc/mah_write_note.test"
    #include "suites/misc/mah_get_error.test" 
    
    #include "suites/chord/mah_invert_chord.test"
    #include "suites/chord/mah_get_chord.test"
    #include "suites/chord/mah_return_chord.test"
    
    #include "suites/scale/mah_get_scale.test"
    #include "suites/scale/mah_return_scale.test"
    
    printf("%d / %d Tests Passed", TEST.pass, TEST.total);
    if (TEST.pass != TEST.total) {
        return EXIT_FAILURE;
    }

}

// Function Definitions //

void
assert(int val, int line, char* file, char* expr)
{
    TEST.total++;
    if (val) {
        TEST.pass++;
    } else {
        fprintf(stderr, "%s @ LINE %d | %s\n", file, line, expr);
    }
    ERR = MAH_ERROR_NONE;
}

bool
comp_note(struct mah_note note_a, struct mah_note note_b)
{
    return note_a.tone == note_b.tone &&
           note_a.acci == note_b.acci &&
           note_a.pitch == note_b.pitch;
}

bool
comp_notes(struct mah_note const* notes_a, struct mah_note const* notes_b, int size_a, int size_b)
{
    if (size_a != size_b) {
        return false;
    }

    for (int i = 0; i < size_a; i++) {
        if (!comp_note(notes_a[i], notes_b[i])) {
            return false;
        }
    }
    return true;
}

bool
comp_interval(struct mah_interval inter_a, struct mah_interval inter_b)
{
    return inter_a.steps == inter_b.steps &&
           inter_a.qual == inter_b.qual;
}

bool
comp_inters(struct mah_interval const* inters_a, struct mah_interval const* inters_b, int size_a, int size_b)
{
    if (size_a != size_b) {
        return false;
    }

    for (int i = 0; i < size_a; i++) {
        if (!comp_interval(inters_a[i], inters_b[i])) {
            return false;
        }
    }
    return true;
}

bool
comp_key_sig(struct mah_key_sig key_a, struct mah_key_sig key_b)
{
    return key_a.type == key_b.type &&
           key_a.alter == key_b.alter &&
           key_a.size == key_b.size &&
           comp_note(key_a.key, key_b.key) &&
           comp_notes(key_a.notes, key_b.notes, key_a.size, key_b.size);
}

bool
comp_chord(struct mah_chord chord_a, struct mah_chord chord_b)
{
    return chord_a.size == chord_b.size &&
           chord_a.inv == chord_b.inv &&
           comp_notes(chord_a.notes, chord_b.notes, chord_a.size, chord_b.size) &&
           comp_notes(chord_a.base, chord_b.base, chord_a.size, chord_b.size);
           
}

bool
comp_chord_base(struct mah_chord_base const* base_a, struct mah_chord_base const* base_b)
{
    return !strcmp(base_a->name, base_b->name) &&
           base_a->size == base_b->size &&
           comp_inters(base_a->steps, base_b->steps, base_a->size, base_b->size);
}

bool
comp_chord_result(struct mah_chord_result result_a, struct mah_chord_result result_b)
{
    return comp_note(result_a.key, result_b.key) &&
           comp_chord_base(result_a.chord, result_b.chord);
}

bool
comp_chord_result_list(struct mah_chord_result_list list_a, struct mah_chord_result_list list_b)
{
    return list_a.max == list_b.max &&
           list_a.size == list_b.size &&
           comp_chord_results(list_a.results, list_b.results, list_a.size, list_b.size);
}

bool
comp_chord_results(struct mah_chord_result* results_a, struct mah_chord_result* results_b, int size_a, int size_b)
{
    if (size_a != size_b) {
        return false;
    }

    for (int i = 0; i < size_a; i++) {
        if (!comp_chord_result(results_a[i], results_b[i])) {
            return false;
        }
    }
    return true;
}

bool
comp_scale(struct mah_scale scale_a, struct mah_scale scale_b)
{
    return scale_a.size == scale_b.size &&
           scale_a.type == scale_b.type &&
           comp_notes(scale_a.notes, scale_b.notes, scale_a.size, scale_b.size);
}

bool
comp_scale_base(struct mah_scale_base const* base_a, struct mah_scale_base const* base_b)
{
    return !strcmp(base_a->name, base_b->name) &&
           base_a->size == base_b->size &&
           comp_inters(base_a->steps, base_b->steps, base_a->size, base_b->size);
}

bool
comp_scale_result(struct mah_scale_result result_a, struct mah_scale_result result_b)
{
    return comp_note(result_a.key, result_b.key) &&
           comp_scale_base(result_a.scale, result_b.scale);
}

bool
comp_scale_result_list(struct mah_scale_result_list list_a, struct mah_scale_result_list list_b)
{
    return list_a.max == list_b.max &&
           list_a.size == list_b.size &&
           comp_scale_results(list_a.results, list_b.results, list_a.size, list_b.size);
}

bool
comp_scale_results(struct mah_scale_result* results_a, struct  mah_scale_result* results_b, int size_a, int size_b)
{
    if (size_a != size_b) {
        return false;
    }

    for (int i = 0; i < size_a; i++) {
        if (!comp_scale_result(results_a[i], results_b[i])) {
            return false;
        }
    }
    return true;
}