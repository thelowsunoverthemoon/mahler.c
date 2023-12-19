#ifndef __MAH_H__
#define __MAH_H__

// Macros //

#define MAH_DISP_LEN 8              // default print size (note + max acci (4) + max number (99) + 1 (null terminating))
#define MAH_CHORD_LIST_DEFAULT NULL // default chord list for returnChord()
#define MAH_SCALE_LIST_DEFAULT NULL // default scale list for returnChord()

#include "err/err.h"
#include "note/note.h"
#include "inter/inter.h"
#include "scale/scale.h"
#include "chord/chord.h"
#include "key/key.h"
#include "misc/misc.h"

#endif