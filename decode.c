#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    int count = 0;
    int check = 1;

    for(int i = 0; argv[i]; i++)
    {
        count++;
    }

    if(count == 4)
    {
        decInfo->stego_image_fname = argv[2];
        decInfo->output_fname = argv[3];

        check = 0;
    }

    if(count == 3)
    {
        decInfo->stego_image_fname = argv[2];
        decInfo->output_fname = "output.txt";

        check = 0;
    }

    char *domain = strstr(argv[2], ".bmp");

    if(domain == NULL)
    {
        check = 1;
    }

    if(check)
    return e_failure;

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image=fopen(decInfo->stego_image_fname,"r");

    if(decInfo->fptr_stego_image==NULL)
    {
        printf("Enter in Opening File !!\n");
        return e_failure;
    }

    decInfo->fptr_output=fopen(decInfo->output_fname,"w");

    if(decInfo->fptr_output==NULL)
    {
        printf("Enter in Opening File !!\n");
        return e_failure;
    }


    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO : ## Decoding Procedure Started ##\n");
    if(open_decode_files(decInfo) == e_failure )
    {
        printf("INFO : Problem in Opening file !!\n");
        return e_failure;
    }
    printf("INFO : Opened steged_img.bmp\n");

    fseek(decInfo->fptr_stego_image,54,SEEK_SET);

    printf("INFO : Decoding Magic String Signature\n");
    if(decode_magic_string(decInfo)== e_failure)
    {
        printf("Problem in entered magic string !!\n");
        return e_failure;
    }
    printf("INFO : Done\n");


    int extn_size;
    
    printf("INFO : Decoding Extension Size of Secret File\n");
    decode_size_from_lsb((char*)&extn_size,sizeof(int),decInfo->fptr_stego_image);

    decInfo->secret_file_extn_size=extn_size;
    printf("INFO : Done\n");

    decode_secret_file_extn(decInfo->secret_file_extn,decInfo->secret_file_extn_size,decInfo);

    int file_size;
    printf("INFO : Decoding Extension of Secret File\n");
    decode_size_from_lsb((char *)&file_size, sizeof(int), decInfo->fptr_stego_image);

    decInfo->secret_file_size=file_size;
    printf("INFO : Done\n");

    printf("INFO : Decoding Secret File\n");
    decode_secret_file(decInfo);
    printf("INFO : Done\n");

    printf("## Decoding Done Successfully ##\n");

    printf("---------------------------------------------------------\n");

    return e_success;
}
Status decode_byte_to_lsb(char *data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        *data = ( *data << 1 ) | (image_buffer[i] & 1);
    }

    return e_success;
}
Status decode_magic_string(DecodeInfo *decInfo)
{
    int len=strlen(decInfo->magic_string);
    char decoded_magic_string[100];

    for(int i=0;i<len;i++)
    {
        char buffer[8];
        char ch=0;
        fread(buffer,1,8,decInfo->fptr_stego_image);
        decode_byte_to_lsb(&ch,buffer);
        decoded_magic_string[i]=ch;
    }

    decoded_magic_string[len]='\0';

    if(strcmp(decoded_magic_string,decInfo->magic_string)==0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status decode_size_from_lsb(char *data, int size, FILE *fptr_stego_image)
{
    for(int i=0;i<size;i++)
    {
        char buffer[8];

        fread(buffer,1,8,fptr_stego_image);
        decode_byte_to_lsb(&data[i],buffer);
    }

    return e_success;
}
Status decode_secret_file_extn(char *file_extn, int file_extn_size, DecodeInfo *decInfo)
{
    for(int i=0;i<file_extn_size;i++)
    {
        char buffer[8];
        fread(buffer,1,8,decInfo->fptr_stego_image);
        decode_byte_to_lsb(&file_extn[i],buffer);
    }
    
    file_extn[file_extn_size]='\0';
    return e_success;
}
Status decode_secret_file(DecodeInfo *decInfo)
{
    for (int i=0;i<decInfo->secret_file_size;i++)
    {
        char secret_buffer[8];
        char ch;

        fread(secret_buffer,1,8,decInfo->fptr_stego_image);
        decode_byte_to_lsb(&ch,secret_buffer);
        fwrite(&ch,1,1,decInfo->fptr_output);
    }
    return e_success;
}