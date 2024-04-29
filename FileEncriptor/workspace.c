#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FileList.h"

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#define SYSTEM_TABLE_FILE_NAME  "C:\\x\\systemTable.bin"

#pragma warning (disable : 4996)



/*----------------------------------------------------------
readFile function
------------------------------------------------------------

General: The function inserts the content (bytes) of the file
         into a dynamic array that it allocates and returns
         the length of the file (A length of 0 indicates an error).

Input:   The function gets the file source and a pointer to a
         sample array.

Process: The funtion opens the file and caculates how many bytes
         are in it (by fseek to its end) and then copies its
         content into a dynaic array in memory

Output:  0 - OK;
         1 - illegal sampleSize;
         2 - Allocation error;
         3 - File read error
------------------------------------------------------------*/
int readFile(string FileSource, int *length, int sampleSize, unsigned int *sampleArray[])
{
  int            i;
  FILE          *fileToRead;
  unsigned char  sample;

  if ((sampleSize != 1) && (sampleSize != sizeof(unsigned int)))
  {
    return 0;
  }

  fileToRead = fopen(FileSource, "rb");
  if (fileToRead == NULL)
  {
    *sampleArray = NULL;

    return 1;
  }

  /* Extract file size */
  fseek(fileToRead, 0, SEEK_END);
  *length = ftell(fileToRead);
  rewind(fileToRead);

  if (sampleSize == 1)
  { // Samples of bytes
    *sampleArray = calloc(*length + 1, sizeof(unsigned int));
    if (*sampleArray == NULL)
    {
      return 2;
    }

    for (i = 0; i < *length; i++)
    {
      if (fread(&sample, 1, 1, fileToRead) != 1)
      {
        return 3;
      }

      (*sampleArray)[i] = (int)sample;
    }
  }
  else
  { 
	  // Samples of unsigned int
    *sampleArray = calloc(*length, sizeof(unsigned int));
    if (*sampleArray == NULL)
    {
      return 2;
    }

	*length /= sizeof(unsigned int);

    if (fread(*sampleArray, sizeof(unsigned int), *length, fileToRead) != *length)
    {
      fclose(fileToRead);

      return 3;
    }

	(*length)--;
  }

  fclose(fileToRead);

  return 0;
}



/*----------------------------------------------------------
writeFile function
------------------------------------------------------------

General: The function writes the sample array (from memory)
         into a binary file.

Input:   The name of the output file, the file's length,
         the size (in bytes) of each sample (1 or 4) and the
         sample array

Process: Opens the file and writes each sample from the sample
         array to the output file

Output:  0 - OK;
         1 - illegal sampleSize;
         2 - File open error;
         3 - File write error
------------------------------------------------------------*/
int writeFile(string FileSource, int length, int sampleSize, unsigned int sampleArray[])
{
  int            i;
  FILE          *fileToWrite;
  unsigned char  sample;

  if ((sampleSize != 1) && (sampleSize != sizeof(unsigned int)))
  {
    return 1;
  }

  fileToWrite = fopen(FileSource, "wb");
  if (fileToWrite == NULL)
  {
    return 2;
  }

  if (sampleSize == 1)
  { // Samples of bytes
    for (i = 0; i < length; i++)
    {
      sample = (unsigned char)(sampleArray[i]);
      if (fwrite(&sample, 1, 1, fileToWrite) != 1)
      {
        fclose(fileToWrite);

        return 3;
      }
    }
  }
  else
  { // Samples of unsigned int
    if (fwrite(sampleArray, sizeof(unsigned int), length, fileToWrite) != length)
    {
      fclose(fileToWrite);

      return 3;
    }
  }

  fclose(fileToWrite);

  return 0;
}



/*----------------------------------------------------------
Name:    knightTowerXorEncription
------------------------------------------------------------

General: knight tower encryption/decryption

Input:   sample array and its length

Process: Encrypt each sample by xoring it with a value taken
         from knight moves

Output:  None
------------------------------------------------------------*/
void knightTowerXorEncription(unsigned int *sampleArray, int length)
{
  const int  knightMoves[] =
  {
    15, 30, 1,  50, 27, 36, 63, 38,
    2,  51, 14, 29, 64, 39, 26, 35,
    31, 16, 49, 4 , 33, 28, 37, 62,
    52, 3,  32, 13, 40, 61, 34, 25,
    17, 42, 53, 48, 5,  24, 11, 60,
    54, 45, 20, 41, 12, 57, 8,  23,
    43, 18, 47, 56, 21, 6,  59, 10,
    46, 55, 44, 19, 58, 9,  22, 7
  };

  while (length--)
  {
    sampleArray[length] ^= knightMoves[length % 64];
  }
}



/*----------------------------------------------------------
readFile function
------------------------------------------------------------

General: The function checks if the given unique key is inside the
table file list file

Input: The function gets the file table file source and pointer to
the unique key

Process: The funtion gets the table file list opened and search the unique key
in the table file list file, if found the function put the unique key into the
pointer

Output:  0 - OK;
1 - key not found

------------------------------------------------------------*/
int searchFileList(FILE *tableFile, FileUnit *key_ptr)
{
  FileUnit  temp_key;

  /* Verify that the key is unique */
  fseek(tableFile, 0, SEEK_SET);

  while (fread(&temp_key, sizeof(temp_key), 1, tableFile) == 1)
  {
    if (key_ptr->fileId == temp_key.fileId)
    { // Key is found
      *key_ptr = temp_key;

      return 1; // Key is found
    }
  }

  return 0; // Key is not found
}



/*----------------------------------------------------------
writeFileList function
------------------------------------------------------------

General: The function enter the file unit into the table file list
file and generate unique file key

Input: The function gets file unit pointer

Process: The funtion opens the file list file, creating unique key and write
the file unit

Output:  0 - OK;
1 - Time out error
2 - File not exist
3 - Write file error

------------------------------------------------------------*/
int writeFileList(FileUnit *key_ptr)
{
  FileUnit      temp_key;
  FILE         *tableFile = fopen(SYSTEM_TABLE_FILE_NAME, "rb+");
  unsigned int  max_loops = 1000000;

  if (tableFile)
  { // File opened successfully
    do
    {
      if (max_loops-- == 0)
      {
        return 1;
      }

      /* Generate a key */
      temp_key.fileId = (unsigned)rand();
    } while (searchFileList(tableFile, &temp_key));

    key_ptr->fileId = temp_key.fileId;
  }
  else
  {
	  // File does not exist
    tableFile = fopen(SYSTEM_TABLE_FILE_NAME, "wb");
    if (tableFile == NULL)
    {
      return 2;
    }

    key_ptr->fileId = (unsigned)rand();
  }

  if (fwrite(key_ptr, sizeof(*key_ptr), 1, tableFile) != 1)
  {
    fclose(tableFile);

    return 3;
  }

  fclose(tableFile);
  return 0;
}


/*----------------------------------------------------------
randomEncryption function
------------------------------------------------------------

General: The function doing random encryption

Input: The function gets pointer to sample array and the sample array length

Process: The funtion creating 3 randrom numbers and calling to 
write list function which doing random encryption

Output:  0 - OK;
other: writeFileList function error

------------------------------------------------------------*/
int randomEncryption(unsigned int *sampleArray, int length)
{
  FileUnit  key;
  time_t    t;
  int       i;

  /* Set a random seed */
  srand((unsigned)time(&t));

  /* generate 3 random numbers between 3 and 9 */
  key.fileVarables[0] = 1 + rand()%256; // Multiply by 1..256 (0 is forbidden)
  key.fileVarables[1] = rand()%17;      // Shift left by 0..16
  key.fileVarables[2] = rand();         // Subtract a random number

  i = writeFileList(&key);
  if (i != 0)
  {
    return i;
  }

  for (i = 0; i < length; i++)
  {
    sampleArray[i] *=  key.fileVarables[0];
    sampleArray[i] <<= key.fileVarables[1];
    sampleArray[i] -=  key.fileVarables[2];
  }

  sampleArray[length] = key.fileId;

  return 0;
}



/*----------------------------------------------------------
randomDecryption function
------------------------------------------------------------

General: The function doing random decryption

Input: The function gets pointer to sample array and the sample array length

Process: The funtion doing random descreption and free the file

Output:  0 - OK;
1 - file table list openning error
2 - random encryption key not found in file list table file

------------------------------------------------------------*/
int randomDecryption(unsigned int *sampleArray, int length)
{
  FILE     *tableFile = fopen(SYSTEM_TABLE_FILE_NAME, "rb");
  FileUnit  key;
  int       i;

  if (tableFile == NULL)
  {
    return 1;
  }

  key.fileId = sampleArray[length];
  if (searchFileList(tableFile, &key) == 0)
  {
    return 2;
  }

  fclose(tableFile);

  for (i = 0; i < length; i++)
  {
    sampleArray[i] +=  key.fileVarables[2];
    sampleArray[i] >>= key.fileVarables[1];
    sampleArray[i] /=  key.fileVarables[0];
  }

  return 0;
}



/*----------------------------------------------------------
yosefusEncryption function
------------------------------------------------------------

General: The function doing yosefus encryption

Input: The function gets pointer to sample array and the sample array length

Process: The funtion doing using yosefus algoritim to encrypt the file

Output:  0 - OK;
1 - memory error

------------------------------------------------------------*/

int yosefusEncryption(unsigned int *sampleArray, int length)
{
  unsigned int  *tempArray;
  unsigned char *isDeleted;
  int            tempIndex, sampleIndex, j;
  int            skip = (length % 10) + 1;

  tempArray = malloc(sizeof(unsigned int) * length);
  if (tempArray == NULL)
  {
    return 1;
  }

  memcpy(tempArray, sampleArray, sizeof(unsigned int) * length);

  isDeleted = calloc(sizeof(unsigned int), length);
  if (isDeleted == NULL)
  {
    free(tempArray);

    return 1;
  }
  
  for (tempIndex = sampleIndex = 0; sampleIndex < length; sampleIndex++)
  {
    j = skip;
    do
    {
      tempIndex++;
      if (tempIndex >= length)
      {
        tempIndex = 0;
      }

      if (!isDeleted[tempIndex])
      {
        j--;
      }
    } while (!j);

    sampleArray[sampleIndex] = tempArray[tempIndex];
    isDeleted[tempIndex] = 1;
  }

  free(isDeleted);
  free(tempArray);

  return 0;
}



/*----------------------------------------------------------
yosefusDecryption function
------------------------------------------------------------

General: The function doing yosefus deccryption

Input: The function gets pointer to sample array and the sample array length

Process: The funtion doing using yosefus algoritim to decrypt the file

Output:  0 - OK;
1 - memory error

------------------------------------------------------------*/
int yosefusDecryption(unsigned int *sampleArray, int length)
{
  unsigned int  *tempArray;
  unsigned char *isDeleted;
  int            tempIndex, sampleIndex, j;
  int            skip = (length % 10) + 1;

  tempArray = malloc(sizeof(unsigned int) * length);
  if (tempArray == NULL)
  {
    return 1;
  }

  memcpy(tempArray, sampleArray, sizeof(unsigned int) * length);

  isDeleted = calloc(sizeof(unsigned int), length);
  if (isDeleted == NULL)
  {
    free(tempArray);

    return 1;
  }

  for (tempIndex = sampleIndex = 0; tempIndex < length; tempIndex++)
  {
    j = skip;
    do
    {
      sampleIndex++;
      if (sampleIndex >= length)
      {
        sampleIndex = 0;
      }

      if (!isDeleted[sampleIndex])
      {
        j--;
      }
    } while (!j);

    sampleArray[sampleIndex] = tempArray[tempIndex];
    isDeleted[sampleIndex] = 1;
  }

  free(isDeleted);
  free(tempArray);

  return 0;
}



/*----------------------------------------------------------
main function
------------------------------------------------------------

General: The user interface

Input: The main entry point of the program

Process: The funtion linking between the user and the function

------------------------------------------------------------*/
void main(void)
{
  char          operation; /* 'E' = Encrypt; 'D' = Decrypt */
  string        fileName;
  int           length = 0;
  unsigned int *sampleArray = NULL;
  int           rc = 0; // Return code (for errors)

  do
  {
    printf("\nWould you like to encrypt or decrypt (E/D)? ");
   operation = getch();
    putchar(operation);

    switch (operation)
    {
      case 'e':
      case 'E':
        operation = 'E';
      break;

      case 'd':
      case 'D':
        operation = 'D';
      break;

      default:
        printf("\nPlease choose E or D!");
      break;
    }
  } while ((operation != 'D') && (operation != 'E'));

  printf("\nPlease enter file name to %scrypt: ", (operation == 'E') ? "En" : "De");
  gets(fileName);

  if (operation == 'E')
  {
	  // Encrypt
    rc = readFile(fileName, &length, 1, &sampleArray);
    if (rc || !length)
    {
      printf("\nERROR: Cannot read '%s'!", fileName);
	  printf("\nPress any key to close...");
	  getchar();
      exit(1);
    }

    /* Run encryptions */
    rc = randomEncryption(sampleArray, length); // Must be run first!
    if (rc)
    {
      printf("\nERROR (%d): Cannot encrypt '%s'!", rc, fileName);
	  printf("\nPress any key to close...");
	  getchar();
      exit(1);
    }

    knightTowerXorEncription(sampleArray, length);

    rc = yosefusEncryption(sampleArray, length);
	if (rc)
	{
		printf("\nERROR (%d): Cannot encrypt '%s'!", rc, fileName);
		printf("\nPress any key to close...");
		getchar();
		exit(1);
	}
    /* Generate a file name and write the enrypted samples to the file */
    strcat(fileName, ".enc");
    rc = writeFile(fileName, length + 1, sizeof(unsigned int), sampleArray);
    if (rc)
    {
      printf("\nERROR: Cannot write '%s'!", fileName);
	  printf("\nPress any key to close...");
	  getchar();
      exit(1);
    }

    printf("\nFile encrypted succesfully.");
  }
  else
  { // Decrypt
    rc = readFile(fileName, &length, sizeof(unsigned int), &sampleArray);
    if (rc || !length)
    {

      printf("\nERROR: Cannot read '%s'!", fileName);
	  printf("\nPress any key to close...");
	  getchar();
      exit(1);
    }

    /* Run decryptions */
    rc = yosefusDecryption(sampleArray, length);
    if (rc)
    {
      printf("\nERROR (%d): Cannot decrypt '%s'!", rc, fileName);
	  printf("\nPress any key to close...");
	  getchar();
      exit(1);
    }

    knightTowerXorEncription(sampleArray, length);

    rc = randomDecryption(sampleArray, length); // Must be run last!
    if (rc)
    {
      printf("\nERROR (%d): Cannot decrypt '%s'!", rc, fileName);
	  printf("\nPress any key to close...");
	  getchar();
      exit(1);
    }

    /* Generate a file name and write the derypted samples to the file */
    strcat(fileName, ".dec");
    rc = writeFile(fileName, length, 1, sampleArray);
    if (rc)
    {
      printf("\nERROR: Cannot write '%s'!", fileName);
	  printf("\nPress any key to close...");
	  getchar();
      exit(1);
    }

    printf("\nFile decrypted succesfully.");
  }

  if (sampleArray)
  {
    free(sampleArray);
  }

  printf("\nPress any key to close...");
  getchar();
}