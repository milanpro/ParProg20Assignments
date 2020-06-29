#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

int is_apt_for_exercise(bmp_t *bmp)
{
    int is_argb =
        (bmp->channels == 4) &&
        (bmp->compression == BMP_BI_BITFIELDS) &&
        (bmp->redMask == BMP_ARGB8888_R_MASK) &&
        (bmp->greenMask == BMP_ARGB8888_G_MASK) &&
        (bmp->blueMask == BMP_ARGB8888_B_MASK) &&
        (bmp->alphaMask == BMP_ARGB8888_A_MASK);
    return is_argb;
}

void filter1(uint8_t *in, uint8_t *out, int num_pixels)
{
    pixel_t value;
    int p;

    for (p = 0; p < num_pixels; p++)
    {
        value = ((pixel_t *)in)[p];
        value.r = 255 - value.r;
        value.g = 255 - value.g;
        value.b = 255 - value.b;
        ((pixel_t *)out)[p] = value;
    }
}

void filter2(uint8_t *in, uint8_t *out, int num_pixels)
{
    (void)in;         /* UNUSED */
    (void)out;        /* UNUSED */
    (void)num_pixels; /* UNUSED */
}

void filter3(uint8_t *in, uint8_t *out, int num_pixels)
{
    (void)in;         /* UNUSED */
    (void)out;        /* UNUSED */
    (void)num_pixels; /* UNUSED */
}

void filter4(uint8_t *in, uint8_t *out, int num_pixels)
{
    (void)in;         /* UNUSED */
    (void)out;        /* UNUSED */
    (void)num_pixels; /* UNUSED */
}

/*----------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    bmp_t bmp_in, bmp_out;
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: %s [task = 1] bmp-file\n", argv[0]);
        exit(1);
    }

    char *filename = argv[1];
    char task = '1';
    if (argc == 3)
    {
        filename = argv[2];
        task = argv[1][0];
    }

    bmp_read(&bmp_in, filename);

    if (!is_apt_for_exercise(&bmp_in))
    {
        fprintf(stderr, "For the sake simplicity please provide a ARGB8888 image.\n");
        exit(4);
    }

    bmp_copyHeader(&bmp_out, &bmp_in);

    switch (task)
    {
    case '1':
        filter1(bmp_in.data, bmp_out.data, bmp_in.width * bmp_in.height);
        break;
    case '2':
        filter2(bmp_in.data, bmp_out.data, bmp_in.width * bmp_in.height);
        break;
    case '3':
        filter3(bmp_in.data, bmp_out.data, bmp_in.width * bmp_in.height);
        break;
    case '4':
        filter4(bmp_in.data, bmp_out.data, bmp_in.width * bmp_in.height);
        break;
    default:
        fprintf(stderr, "Invalid filter.\n");
        exit(5);
    }

    bmp_write(&bmp_out, "output.bmp");
    bmp_free(&bmp_in);
    bmp_free(&bmp_out);

    return 0;
}
