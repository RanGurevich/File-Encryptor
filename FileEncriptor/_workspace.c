#include <stdio.h>
#include <stdlib.h>
#include <time.h>



#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#define SYSTEM_TABLE_FILE_NAME  "C:\\x\\systemTable.bin"

#pragma warning (disable : 4996)

#define TO_LOCK  0
#define TO_FREE  1




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
int readFile(string FileSource, int *length, int sampleSize, unsigned int **sampleArray)
{
	int            i;
	FILE          *fileToRead;
	unsigned char  sampleChar;
	unsigned int  sampleInt;
	int len;
	if ((sampleSize != 1) && (sampleSize != sizeof(unsigned int)))
	{
		return(1);
	}

	fileToRead = fopen(FileSource, "rb");
	if (fileToRead == NULL)
	{
		sampleArray = NULL;

		return(1);
	}

	/* Extract file size */
	fseek(fileToRead, 0, SEEK_END);
	*length = ftell(fileToRead);
	len = *length;
	rewind(fileToRead);

	if (sampleSize == 1)
	{ // Samples of bytes
		*sampleArray = calloc(len + 1, sizeof(char));
		if (sampleArray == NULL)
		{
			return(2);
		}

		for (i = 0; i < len; i++)
		{
			fread(&sampleChar, 1, 1, fileToRead);
			printf("%c", sampleChar);
			*(*(sampleArray)+i) = (unsigned int)sampleChar;
		}
	}
	else
	{ // Samples of unsigned int
		len /= sizeof(unsigned int);

		*sampleArray = calloc(len + 1, sizeof(unsigned int));
		if (sampleArray == NULL)
		{
			return(2);
		}
		for (i = 0; i < len; i++)
		{
			if (fread(&sampleInt, sizeof(unsigned int), 1, fileToRead) != 1)
			{
				return(3);
			}

			*(*(sampleArray)+i) = (unsigned int)sampleInt;
		}
	}

	fclose(fileToRead);

	return(0);
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
int writeFile(string FileSource, int length, int sampleSize, unsigned int *sampleArray)
{
	int            i;
	FILE          *fileToWrite;
	unsigned char  sample;

	if ((sampleSize != sizeof(unsigned char)) && sampleSize != sizeof(int))
	{
		return(1);
	}
	fileToWrite = fopen(FileSource, "wb");
	if (fileToWrite == NULL)
	{
		return(2);
	}

	if (sampleSize == 1)
	{ // Samples of bytes
		for (i = 0; i < length; i++)
		{
			sample = (unsigned char)sampleArray[i];
			printf("%c", sample);
			if (fwrite(&sample, 1, 1, fileToWrite) != 1)
			{
				fcloseall();

				return(3);
			}
		}
	}
	else
	{ // Samples of int
		if (fwrite(sampleArray, sizeof(unsigned int), length, fileToWrite) != length)
		{
			fcloseall();

			return(3);
		}
	}
	fcloseall();
	return(0);
}


int yosefosEncryption(unsigned int *sampleArray, int length)
{
	int i, num;
	linkCircularNode list;
	linkCircularNodePtr head, listPtr;
	linkCircularNode *temp;
	linkCircularNode itemOn;
	initCircularLinkList(&list);
	listPtr = &list;
	char key;
	int count = 0;
	for (i = 0; i < length; i++)
	{
		insertEndCircularLinkList(listPtr, sampleArray[i]);
	}
	key = 1 + length % 9;
	while (listPtr != NULL)
	{
		for (i = 0; i < key; i++)
		{
			listPtr = (listPtr->next);
		}

		num = deleteAfterCircularLinkList(listPtr);
		sampleArray[count] = num;
		count++;
	}
	return(0);


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
Name:
------------------------------------------------------------

Explanation:

------------------------------------------------------------*/
void swap1(int * arr, int p1, int p2)
{
	int  helper = arr[p1];

	arr[p1] = arr[p2];
	arr[p2] = helper;
}



/*---------------------------------------------------------
swap function
-----------------------------------------------------------

General: The function gets two int pointers and switching the value
bewteen the given pointera

Input: The function gets two int pointers

Process:  The funtion switching the values between the pointers

Output: The function doesn't return any value
----------------------------------------------------------*/
void swap(int *bit1, int *bit2)
{
	int  helper = *bit1;

	*bit1 = *bit2;
	*bit2 = helper;
}




/*----------------------------------------------------------
Name:
------------------------------------------------------------

Explanation:

------------------------------------------------------------*/
int SearchFileList(FILE *tableFile, FileUnit *key_ptr)
{
	FileUnit  temp_key;

	/* Verify that the key is unique */
	fseek(tableFile, 0, SEEK_SET);

	while (fread(&temp_key, sizeof(temp_key), 1, tableFile) == 1)
	{
		if (key_ptr->fileId == temp_key.fileId)
		{ // Key is found
			*key_ptr = temp_key;

			return(1); // Key is found
		}
	}

	return(0); // Key is not found
}



/*----------------------------------------------------------
Name:
------------------------------------------------------------

Explanation:

------------------------------------------------------------*/
int WriteFileList(FileUnit *key_ptr)
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
				return(1);
			}

			/* Generate a key */
			temp_key.fileId = (unsigned)rand();
		} while (SearchFileList(tableFile, &temp_key));

		key_ptr->fileId = temp_key.fileId;
	}
	else
	{ // File does not exist
		tableFile = fopen(SYSTEM_TABLE_FILE_NAME, "wb");
		if (tableFile == NULL)
		{
			return(2);
		}

		key_ptr->fileId = (unsigned)rand();
	}

	if (fwrite(key_ptr, sizeof(*key_ptr), 1, tableFile) != 1)
	{
		fclose(tableFile);

		return(3);
	}

	fclose(tableFile);

	return(0);
}



/*----------------------------------------------------------
Name:
------------------------------------------------------------

Explanation:

------------------------------------------------------------*/
int randomEncryption(unsigned int *sampleArray, int length)
{
	FileUnit  key;
	time_t    t;
	int       i;

	/* Set a random seed */
	srand((unsigned)time(&t));

	/* generate 3 random numbers between 3 and 9 */
	key.fileVarables[0] = 3 + rand() % 9; // Multiply by 1..256 (0 is forbidden)
	key.fileVarables[1] = 3 + rand() % 9;      // Shift left by 0..16
	key.fileVarables[2] = 3 + rand() % 9;         // Subtract a random number

	i = WriteFileList(&key);
	if (i != 0)
	{
		return(i);
	}

	for (i = 0; i < length; i++)
	{
		sampleArray[i] *= key.fileVarables[0];
		sampleArray[i] << key.fileVarables[1];
		sampleArray[i] -= key.fileVarables[2];
	}
	sampleArray[length] = key.fileId;
}



/*----------------------------------------------------------
Name:
------------------------------------------------------------

Explanation:

------------------------------------------------------------*/
int randomDecryption(unsigned int *sampleArray, int length)
{
	FILE     *tableFile = fopen(SYSTEM_TABLE_FILE_NAME, "rb");
	FileUnit  key;
	int       i;

	if (tableFile == NULL)
	{
		return(1);
	}

	key.fileId = sampleArray[length];
	if (SearchFileList(tableFile, &key) == 0)
	{
		return(2);
	}

	fclose(tableFile);

	for (i = 0; i < length; i++)
	{
		sampleArray[i] += key.fileVarables[2];
		sampleArray[i] >> key.fileVarables[1];
		sampleArray[i] /= key.fileVarables[0];
	}
}



/*---------------------------------------------------------
Name:    cardEncryptionDecryption
-----------------------------------------------------------

General: Encrypts or decrypts the sample array by "card swapping"

Input:   sample array, length and direction (forward for encryption
or backward for decryption)

Process: Copy the sample samnple array into a circulr linked list,
scramble the linked list by a seed that is derived from the
number of samples (length) and copies the scampled list
back to the sample array.
The scambling can be in two symmetric directions - forward
(for encryption) and backward (for decryption)

Output:  None
----------------------------------------------------------*/

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

/*----------------------------------------------------------
Name:
------------------------------------------------------------

Explanation:

------------------------------------------------------------*/
void main(void)
{
	char          operation; /* 'E' = Encrypt; 'D' = Decrypt */
	string        fileName;
	int           length = 0;
	unsigned int *sampleArray = NULL;
	int           rc; // Return code (for errors)
	int			uniqueKey;

	//do
	//{
	// readFile("C:\\x\\tvs.bin", &sampleArray);
	//  printf("\nWould you like to encrypt or decrypt (E/D)? ");
	//  scanf("%c",&operation);

	//  switch (operation)
	//  {
	//    case 'e':
	//    case 'E':
	//      operation = 'E';
	//    break;

	//    case 'd':
	//    case 'D':
	//      operation = 'D';
	//    break;

	//    default:
	//      printf("\nPlease choose E or D!");
	//    break;
	//  }
	//} while ((operation != 'D') && (operation != 'E'));

	//printf("\nPlease enter file name to %scrypt: ", (operation == 'E') ? "En" : "De");
	//gets(fileName);

	//length = readFile(fileName, &sampleArray);
	//if (length == 0)
	//{
	//  printf("\nERROR: Cannot open '%s'!", fileName);

	//  exit(1);
	//}

	//if (operation == 'E')
	// { // Encrypt
	/* Run encryptions */
	// length = readFile("C:\\x\\tvs.bin", &sampleArray);
	// rc = randomEncryption(sampleArray, length);
	//writeFile("C:\\x\\z.bin", length + 1, sizeof(int), sampleArray);

	//   if (rc)
	//   {
	//     printf("\nERROR (%d): Cannot encrypt '%s'!", rc, fileName);;
	//     exit(1);
	//   }

	//  /* Generate a file name and write the enrypted samples to the file */
	//  strcat(fileName, ".encrypted");

	//  writeFile(fileName, length + 1, sizeof(unsigned int), sampleArray);
	//}
	//else
	//{ // Decrypt
	//  /* Extract unique Key */
	//  uniqueKey = sampleArray[length];
	//  length--;

	//  /* Run decryptions */
	//  rc = randomDecryption(sampleArray, length);

	//  knightTowerXorEncription(sampleArray, length);

	//  /* Generate a file name and write the derypted samples to the file */
	//  strcat(fileName, ".decrypted");
	//  writeFile(fileName, length, 1, sampleArray);
	//}
	//***************************************************************************

	// Encryption Check
	//length = readFile("C:\\x\\tvs.bin", &sampleArray);
	///*rc =*/ randomEncryption(sampleArray, length);

	//writeFile("C:\\x\\z.bin", length+1, sizeof(unsigned int), sampleArray);
	//  ****************************************************************************
	//   Decrepted Check
	// length = readFile("C:\\x\\z.bin", &sampleArray);
	// /* Extract unique Key */
	//   uniqueKey = sampleArray[length];
	//randomDecryption(sampleArray, length);
	//length--;
	//writeFile("C:\\x\\t.bin", length, sizeof(unsigned int), sampleArray);
	readFile("C:\\x\\tvs.bin", &length, sizeof(unsigned char), &sampleArray);
	writeFile("C:\\x\\z.bin", length, sizeof(unsigned char), &sampleArray);
	printf("%d", length);

	//linkCircularNode x;
	//linkCircularNodePtr h;
	//initCircularLinkList(&x);
	//x.info = 1;
	//insertAfterCircularLinkList(&x, 2);
	//insertAfterCircularLinkList(&x, 7);
	//printf("%d\n", x.info);
	//printf("%d\n", x.next->info);
	//printf("%d\n", x.next->next->info);
	//getchar(4);
}
