#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_failure)
    {
        printf("INFO : Encoding Failed Filed did not Opened !!\n");
        return e_failure;
    }

    printf("INFO : ## Encoding Procedure Started ##\n");

    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    printf("INFO : Checking for secret.txt size\n");
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    printf("INFO : Done.Not Empty\n");

    printf("INFO : Checking for SkeletonCode/beautiful.bmp capacity to handel secret.txt\n");
    if (check_capacity(encInfo) == e_failure)
    {
        printf("Encoding failed !!\n");
        return e_failure;
    }
    printf("INFO : Done.Found OK\n");

    printf("INFO : Copying Image Header\n");
    copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image);
    printf("INFO : DONE\n");
    
    printf("INFO : Copying Magic String Signature\n");
    encode_magic_string(encInfo->magic_string,encInfo);
    printf("INFO : Done\n");

    char *extn = strchr(encInfo->secret_fname, '.');

    int extn_len=strlen(extn);

    
    encode_size_to_lsb((char *)&extn_len,sizeof(int),encInfo->fptr_src_image,encInfo->fptr_stego_image);

    printf("INFO : Encoding secret.txt File extension\n");
    encode_secret_file_extn(extn,encInfo);
    printf("INFO : Done\n");

    printf("INFO : Encoding secret.txt File Size\n");
    encode_secret_file_size(encInfo->size_secret_file, encInfo);
    printf("INFO : Done\n");

    printf("INFO : Encoding secret.txt File Data\n");
    encode_secret_file_data(encInfo);
    printf("INFO : Done\n");

    printf("INFO : Copying Left Over Data\n");
    copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);
    printf("INFO : Done\n");


    printf("## Encoding Done Successfully ##\n");

    printf("-------------------------------------------------------\n");
    return e_success;
}

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    printf("INFO : Opening required files\n");

    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    printf("INFO : Opened SkeletonCode/beautiful.bmp\n");

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    printf("INFO : Opened secret.txt\n");

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    printf("INFO : Opened steged_img.bmp\n");

    // No failure return e_success
    printf("INFO : DONE\n");   
    
    
    return e_success;
}


//In this function we will read(store value) and check if it is right or wrong

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    int count=0;
    int check=0;
    //To Check how many arguments are given by user..
    for(int i=0;argv[i];i++)
    {
        count++;
    }

    //To make argv[4] default..
    if(count==4)
    {
        encInfo->src_image_fname=argv[2];
        encInfo->secret_fname=argv[3];
        encInfo->stego_image_fname="stego_img.bmp";

        check=1;
    }

    //If argv[4]already given by user..
    if(count==5)
    {
        encInfo->src_image_fname=argv[2];
        encInfo->secret_fname=argv[3];
        encInfo->stego_image_fname=argv[4];

        check=1;
    }

    //if check remain=1,then all is right..
    if(check==0)
    return e_failure;

    //Check if file is .bmp or not..
    char *domain = strstr(argv[2],".bmp");

    if(domain==NULL)
    {
        check=0;
    }

    if(check)
        return e_success;
    
    else
    return e_failure;
}

//In this function we will try to get size of a file
uint get_file_size(FILE *fptr)
{
    long current_position = ftell(fptr);  // we store current position because when we get last pos we will pointed on last;
    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr); //tell last position that is size 
    fseek(fptr,current_position,SEEK_SET); // again store to original location

    return (uint)file_size;
}

//In this function we will check if secret file can be store in src file or not
Status check_capacity(EncodeInfo *encInfo)
{
    int magic_string_len = strlen(encInfo->magic_string);

    char *extn=strchr(encInfo->secret_fname,'.');
    int extn_len=strlen(extn);

    long check = (magic_string_len *8) + 32 + (extn_len * 8) + 32 + (encInfo->size_secret_file * 8);

    if (check <= encInfo->image_capacity)
    {
        return e_success;
    }

    return e_failure;
}

//In this function we will copy 54 bytes of bmp to secret file

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    unsigned char buffer[54];

    rewind(fptr_src_image);

    fread(buffer, 1, 54, fptr_src_image);
    fwrite(buffer, 1, 54, fptr_dest_image);

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int len=strlen(magic_string);
    
    for(int i=0;i<len;i++)
    {
        char image_buffer[8];
        fread(image_buffer,1,8,encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i],image_buffer);
        fwrite(image_buffer,1,8,encInfo->fptr_stego_image);
    }  

    return e_success;
}
Status encode_byte_to_lsb(char data , char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        int bit = (data >> (7-i)) & 1;
        image_buffer[i] = (image_buffer[i] & ~1) | bit;
    }

    return e_success;
}

Status encode_size_to_lsb(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    for(int i = 0; i < size; i++)
    {
        char image_buffer[8];

        fread(image_buffer, 1, 8, fptr_src_image);

        encode_byte_to_lsb(data[i], image_buffer);

        fwrite(image_buffer, 1, 8, fptr_stego_image);
    }

    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    int len=strlen(file_extn);

    for(int i=0;i<len;i++)
    {
        char extn_buffer[8];

        fread(extn_buffer,1,8,encInfo->fptr_src_image) ;

        encode_byte_to_lsb(file_extn[i],extn_buffer);

        fwrite(extn_buffer,1,8,encInfo->fptr_stego_image);
    }

    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb((char *)&file_size, sizeof(int),encInfo->fptr_src_image,encInfo->fptr_stego_image);

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    int len=encInfo->size_secret_file;

    for(int i=0;i<len;i++)
    {
        char ch;
        char sec_buffer[8];

        fread(&ch,1,1,encInfo->fptr_secret);
        fread(sec_buffer,1,8,encInfo->fptr_src_image);
        encode_byte_to_lsb(ch,sec_buffer);
        fwrite(sec_buffer,1,8,encInfo->fptr_stego_image);
    }

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    int ch;
    while((ch=fgetc(fptr_src))!=EOF)
    {
        fputc(ch,fptr_dest);
    }

    return e_success;
}