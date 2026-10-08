


  LSB IMAGE STEGANOGRAPHY uSING C







📌 About the Project



This project implements **Image Steganography using the LSB (Least Significant Bit) technique** in C.

It allows a secret file to be hidden inside a **BMP image** and later extracted from the image without changing the visible appearance of the image.

The project can hide files such as .txt, .c, .pdf, .mp3, .jpg, and .csv.



⚙️ How It Works



### Encoding

The secret file is hidden inside the BMP image by storing its information in the "least significant bits" of the image data.

  
Secret File
     ==>
Read File
     ==>
Get Extension & Size
     ==>
Convert Data to Bits
     ==>
Hide Bits in BMP Image
     ==>
Stego Image







### Decoding

The hidden information is extracted from the stego image and used to recreate the original file.


Stego Image
     ==>
Read Hidden Bits
     ==>
Extract Extension & Size
     ==>
Extract File Data
     ==>
Recreate Secret File



🚀 Flow





<img width="1024" height="678" alt="Image" src="https://github.com/user-attachments/assets/21913afa-c4a1-45fd-bd3c-061efbdf75fc" />






🚀 Features



* Hide files inside BMP images
* Extract hidden files from images
* Supports multiple file formats
* Stores file extension and size
* Uses a magic string for validation
* Supports custom output filename
* Command-line based program






## Encode


./a.out  -e  source.bmp  secret_file  output.bmp


Example:


./a.out  -e  beautiful.bmp  secret.txt  stego.bmp


## Decode


./a.out  -d  stego.bmp


The hidden file will be extracted with its original extension.



🛠️ Concepts Used



* C Programming
* File Handling
* Structures
* Pointers
* Strings
* Bitwise Operations
* Command-Line Arguments
* LSB Steganography





▶️ Output


<img width="1284" height="799" alt="Image" src="https://github.com/user-attachments/assets/71a85ca5-b3e2-4af2-9c50-0e6f90914937" />


<img width="967" height="568" alt="Image" src="https://github.com/user-attachments/assets/894cb7b8-b5e4-4e18-a248-e37fccbb16d4" />




🎯 Learning Outcome

This project helped me understand how data can be stored inside an image at the "bit level". It also improved my knowledge of file handling, pointers, structures, bitwise operations, and modular programming in C.


