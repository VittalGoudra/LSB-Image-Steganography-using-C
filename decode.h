#ifndef DECODE_H
#define DECODE_H

#include "types.h"

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output File Info */
    char *output_fname;
    FILE *fptr_output;

    /* Magic string */
    char magic_string[100];

    /* Secret file */
    char secret_file_extn[10];
    int secret_file_extn_size;

    long secret_file_size;

} DecodeInfo;

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Perform the Decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Decode a byte into LSB of image data array */
Status decode_byte_to_lsb(char *data, char *image_buffer);

/*Decode  Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* DEcode function, which does the real Decoding */
Status decode_size_from_lsb(char *data, int size, FILE *fptr_stego_image);

/* Decode secret file size */
Status decode_secret_file_extn(char *file_extn, int file_extn_size, DecodeInfo *decInfo);

/* Decode secret file  */
Status decode_secret_file(DecodeInfo *decInfo);
#endif