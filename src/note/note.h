#ifndef __MAH_NOTE_H__
#define __MAH_NOTE_H__

// Enums //

typedef enum mah_tone {
    MAH_C, MAH_D, MAH_E, MAH_F, MAH_G, MAH_A, MAH_B
} mah_tone;

typedef enum mah_acci {
    MAH_DBFLAT = -2, MAH_FLAT = -1, MAH_NATURAL = 0, MAH_SHARP = 1, MAH_DBSHARP = 2
} mah_acci;

// Structures //

typedef struct mah_note {
    enum mah_tone tone;
    int           acci;
    int           pitch;
} mah_note;

#endif