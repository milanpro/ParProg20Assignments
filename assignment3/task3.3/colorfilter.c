#include "bmp.h"
#include "constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <altivec.h>

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
    vector unsigned char *values = (vector unsigned char *)in;
    vector unsigned char *outputs = (vector unsigned char *)out;

    for (int p = 0; p < num_pixels / 4; p++)
    {
        vector unsigned char value = vec_subs(invertMask, values[p]);

        // We don't want to invert the alpha channel
        value = vec_perm(value, values[p], alphaPattern);

        outputs[p] = value;
    }
}

void filter2(uint8_t *in, uint8_t *out, int num_pixels)
{
    vector unsigned char *values = (vector unsigned char *)in;
    vector unsigned char *outputs = (vector unsigned char *)out;

    for (int i = 0; i < num_pixels / 4; i++)
    {
        //take the 4 r g and b values of each char vector and separate them into their on int vectors
        vector unsigned int rVector = (vector unsigned int)vec_perm(values[i], zeroVector, rPattern);
        vector unsigned int gVector = (vector unsigned int)vec_perm(values[i], zeroVector, gPattern);
        vector unsigned int bVector = (vector unsigned int)vec_perm(values[i], zeroVector, bPattern);

        //convert the int vectors to float
        vector float rFloatVector = vec_ctf(rVector, 0);
        vector float gFloatVector = vec_ctf(gVector, 0);
        vector float bFloatVector = vec_ctf(bVector, 0);

        //apply rgb to grayscale calculation to each r g and b vector
        vector float grayFloatVector;
        grayFloatVector = vec_mul(rFloatVector, rFactor);
        grayFloatVector = vec_madd(gFloatVector, gFactor, grayFloatVector);
        grayFloatVector = vec_madd(bFloatVector, bFactor, grayFloatVector);

        //convert the float vectors to int
        vector unsigned int grayVector = vec_ctu(grayFloatVector, 0);

        //clamp max value at 255
        grayVector = vec_min(grayVector, maxPattern);

        //reconstruct char array from int r g b vectors
        outputs[i] = vec_perm((vector unsigned char)grayVector, alphaMask, grayPattern);
    }
}

void filter3(uint8_t *in, uint8_t *out, int num_pixels)
{
  vector unsigned char *values = (vector unsigned char *)in;
  vector unsigned char *outputs = (vector unsigned char *)out;

  for (int i = 0; i < num_pixels / 4; i++)
  {
    //take the 4 r g and b values of each char vector and separate them into their on int vectors
    vector unsigned int rVector = (vector unsigned int)vec_perm(values[i], zeroVector, rPattern);
    vector unsigned int gVector = (vector unsigned int)vec_perm(values[i], zeroVector, gPattern);
    vector unsigned int bVector = (vector unsigned int)vec_perm(values[i], zeroVector, bPattern);

    // Find out if red is the dominant color
    vector bool char r_larger_g = vec_cmpgt(rVector, gVector);
    vector bool char r_larger_b = vec_cmpgt(rVector, bVector);
    vector bool char r_largest = vec_and(r_larger_g, r_larger_b);
    vector bool char r_mask = vec_perm(r_largest, zeroVector, grayPattern);

    //convert the int vectors to float
    vector float rFloatVector = vec_ctf(rVector, 0);
    vector float gFloatVector = vec_ctf(gVector, 0);
    vector float bFloatVector = vec_ctf(bVector, 0);

    //apply rgb to grayscale calculation to each r g and b vector
    vector float grayFloatVector;
    grayFloatVector = vec_mul(rFloatVector, rFactor);
    grayFloatVector = vec_madd(gFloatVector, gFactor, grayFloatVector);
    grayFloatVector = vec_madd(bFloatVector, bFactor, grayFloatVector);

    //convert the float vectors to int
    vector unsigned int grayVector = vec_ctu(grayFloatVector, 0);

    //clamp max value at 255
    grayVector = vec_min(grayVector, maxPattern);

    //reconstruct char array from int r g b vectors
    vector unsigned char grayOutput = vec_perm((vector unsigned char)grayVector, alphaMask, grayPattern);
    outputs[i] = vec_sel(grayOutput, values[i], r_mask);
  }
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
