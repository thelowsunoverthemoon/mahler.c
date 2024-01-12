#include "mahler.h"
#include <stdio.h>

// Transpose Melody up Perfect 5th //
/*
    G3 Ab3 C#5
*/

#define BUF_SIZE 10

void transpose(struct mah_note mel[], struct mah_note buf[], int size, struct mah_interval inter);

int
main(void)
{
    struct mah_note mel[] = {
        { MAH_C, MAH_NATURAL, 3 },
        { MAH_D, MAH_FLAT, 3 },
        { MAH_F, MAH_SHARP, 4 },
    };

    struct mah_note buf[BUF_SIZE];
    int mel_len = sizeof(mel) / sizeof(*mel);
    transpose(mel, buf, mel_len, (struct mah_interval) { 5, MAH_PERFECT });

    char disp[MAH_DISP_LEN];
    for (int i = 0; i < mel_len; i++)
    {
        printf("%s ", mah_write_note(buf[i], disp, MAH_DISP_LEN, NULL));
    }
}

void
transpose(struct mah_note* mel, struct mah_note* buf, int size, struct mah_interval inter)
{
    for (int i = 0; i < size; i++)
    {
        buf[i] = mah_get_inter(mel[i], inter, NULL);
    }
}