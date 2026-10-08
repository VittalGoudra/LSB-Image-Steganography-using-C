/*Name : Vittal S Goudra 
  Date : 08/10/2026
  Description : Developed an image steganography project in c to securely hide secret data 
                inside BMP image using LSB encoding of different file like txt,c,pdf,mp3,csv,ex.
                
                Used file handling,structures,pointer ,string ,bitwise operations, and coomand-line arguments.
                The decoder extracts the hidden fule extention,size,and data to reconstruct the original file.
                
                Developed during Adcanced C programming Training at Emertxe information tecgnologues */

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if (strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    OperationType Output = e_unsupported;

   if (argc < 3)
    {
        printf("The CLA should be given as :\n");
        printf("Encoding   : ./a.out -e source_file secret_file \n");
        printf("Decoding   : ./a.out -d stego_file \n");
        return 0;
    }

    Output = check_operation_type(argv);
    
    if (Output == e_encode)
    {
        printf("Encode Selected !!\n");

        printf("Enter the Magic String = ");
        scanf("%s",encInfo.magic_string);

        printf("------------------------------------------------------------\n");

        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            do_encoding(&encInfo);
        }
        else
        {
            printf("Unsupported CLA Arguments !!\n");
        }
    }
    else if (Output == e_decode)
    {
        printf("Decode Selected !!\n");
        printf("Enter the Magic String = ");
        scanf("%s",decInfo.magic_string);

        printf("----------------------------------------------------------\n");
        
        if(read_and_validate_decode_args(argv,&decInfo) == e_success)
        {
            do_decoding(&decInfo);
        }
        else
        {
            printf("Unsupported CLA Arguments !!\n");
        }
    }
    else
    {
        printf("Not Encoding Neither Decoding !!\n");
    }

    return 0;
}

