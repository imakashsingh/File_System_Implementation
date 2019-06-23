#define _CRT_SECURE_NO_WARNINGS
#define metadatasize 1024
#define NO_OF_BLOCKS 600
#define BLOCK_SIZE 16384
#define FILE_START_BLOCK 3
#define MAGIC_NUMBER 0x444E524D
#define HARD_DISK "hardDisk.hdd"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
typedef struct file_data
{
	char file_name[20];
	unsigned int starting_block_address;
	unsigned int no_of_blocks;
	unsigned int length;
}file_data;

typedef struct metadata
{
	int magic_number = MAGIC_NUMBER;
	unsigned int no_of_files;
	unsigned int file_no;
	unsigned int no_of_free_blocks;
	char arr[NO_OF_BLOCKS];
	file_data fd[32];
}metadata;

//Finds the size of file in bytes
int findSize(char*);

//Copies the files from our working directory to the file system
void copy_to_disk(char* source, char* destination);

//utility function to format the disk
void format();

//lists all files present in the file system
void list_all_files();

//function to open a file
FILE* f_open(char *filename, char *mode);

//Copies the files from the file system to our working directory 
void copy_from_disk(char *source_file, char *destination_file);

//reads data from disk(file system) and copies into a buffer
void read_block(int block_no, char* buff);

//writes data to disk(file system) from a buffer
void write_block(int block_no, char* buff);

//utility to delete a file in disk(file system)
void delete_file_from_disk(char *filename);