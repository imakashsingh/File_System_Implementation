#include "Header.h"

FILE* f_open(char *filename, char *mode)
{
	FILE *fp = fopen(filename, mode);

	if (fp == NULL)
	{
		printf("\nError in openeing the file %s", filename);
		system("pause");
		exit(0);
	}
	return fp;
}

int findSize(char* sourcefile) {
	FILE* source = f_open(sourcefile, "rb");
	fseek(source, 0, SEEK_END);
	int ssize = ftell(source);
	fclose(source);
	return ssize;
}

void copy_to_disk(char* source, char* destination)
{
	metadata md;
	char read_buffer[BLOCK_SIZE+1];
	char write_buffer[BLOCK_SIZE+1];
	read_block(0, read_buffer);

	memcpy(&md, read_buffer, sizeof(metadata));

	if (md.magic_number != MAGIC_NUMBER)
	{
		printf("\nDisk is corrupted....Formatting disk");
		printf("\nFormat to use the disk\n");
		return;
	}
	int ssize = findSize(source);

	if (ssize > ((md.no_of_free_blocks)*BLOCK_SIZE))
	{
		printf("No available space. Disk is full\n");
	}
	else
	{
		int free_block_no = 0;
		strcpy(md.fd[md.no_of_files].file_name, destination);
		for (int i = FILE_START_BLOCK ; i < NO_OF_BLOCKS; i++)
		{
			if (md.arr[i] == 0)
			{
				free_block_no = i;
				break;
			}
		}

		md.fd[md.file_no].starting_block_address = (((free_block_no)*BLOCK_SIZE));
		if (ssize < BLOCK_SIZE)
			md.fd[md.file_no].no_of_blocks = 1;
		else
			md.fd[md.file_no].no_of_blocks = ceil((float)ssize / BLOCK_SIZE);
		md.fd[md.file_no].length = ssize;
		md.no_of_free_blocks = md.no_of_free_blocks - md.fd[md.file_no].no_of_blocks;

		for (int i = free_block_no; i <(free_block_no + md.fd[md.file_no].no_of_blocks); i++)
		{
			md.arr[i] = 1;
		}
		md.file_no += 1;
		md.no_of_files += 1;

		memcpy(write_buffer, &md, sizeof(metadata));
		write_block(0, write_buffer);


		FILE *source_pointer = f_open(source, "rb+");

		for (int i = 0; i < (md.fd[md.file_no - 1].no_of_blocks); i++)
		{
				fread(write_buffer,BLOCK_SIZE,1, source_pointer);
				write_block(free_block_no + i, write_buffer);
		}
		fclose(source_pointer);
	}
}

void copy_from_disk(char* source, char* destination)
{
	metadata md;
	char read_buffer[BLOCK_SIZE+1];
	read_block(0, read_buffer);

	memcpy(&md, read_buffer, sizeof(metadata));

	int rem_size = 0;

	FILE *dest_ptr = f_open(destination, "wb+");

	for (int i = 0; i < md.no_of_files; i++)
	{
		if (strcmp(source, md.fd[i].file_name) == 0)
		{
			int block_no = ceil((float)((md.fd[i].starting_block_address) / BLOCK_SIZE));
			rem_size = md.fd[i].length;
			for (int j = 0; j < md.fd[i].no_of_blocks; j++)
			{
				read_block(block_no + j, read_buffer);

				if (rem_size >= BLOCK_SIZE)
				{
					fwrite(read_buffer, BLOCK_SIZE, 1, dest_ptr);
					rem_size = rem_size - BLOCK_SIZE;
				}
				else
				{
					fwrite(read_buffer,rem_size,1,dest_ptr);
				}
			}
			break;
		}
	}
	fclose(dest_ptr);
}

void list_all_files()
{
	metadata md;
	char read_buffer[BLOCK_SIZE+1];
	read_block(0, read_buffer);

	memcpy(&md, read_buffer, sizeof(metadata));

	if (md.magic_number != MAGIC_NUMBER)
	{
		printf("No files in memory\n");
		return;
	}

	for (int i = 0; i < md.no_of_files; i++)
	{
		printf("%s\n", md.fd[i].file_name);
	}
}

void format()
{
	printf("Formatting the disk ...........\n");
	metadata md;
	char write_buffer[BLOCK_SIZE+1];
	md.file_no = 0;
	md.no_of_files = 0;
	md.no_of_free_blocks = NO_OF_BLOCKS;
	md.magic_number = MAGIC_NUMBER;
	for (int i = 0; i < NO_OF_BLOCKS; i++)
	{
		md.arr[i] = 0;
	}
	char arr[20] = { 0 };
	for (int i = 0; i < 32; i++)
	{
		strcpy(md.fd[i].file_name, arr);
	}
	memcpy(write_buffer, &md, sizeof(md));
	write_block(0, write_buffer);
}

void delete_file_from_disk(char *filename)
{
	metadata md;
	char read_buffer[BLOCK_SIZE + 1];
	char write_buffer[BLOCK_SIZE + 1];

	read_block(0, read_buffer);

	if (md.magic_number != MAGIC_NUMBER)
	{
		printf("Disk is corrupted.Format the disk to use it.\n");
		return;
	}
	else if (md.no_of_files == 0)
	{
		printf("Disk is empty.No files to delete \n");
		return;
	}
	else
	{
		for (int i = 0; i < md.no_of_files; i++)
		{
			if (strcmp(md.fd[i].file_name, filename) == 0)
			{
				if (i == md.no_of_files - 1)
				{
					md.file_no = md.file_no - 1;
					int block_no = ceil((float)((md.fd[i].starting_block_address) / BLOCK_SIZE));
					for (int j = block_no; j < ((md.fd[i].no_of_blocks) + block_no); j++)
					{
						//
					}
					memset(md.fd[i].file_name, '\0', sizeof(md.fd[i].file_name));
					md.fd[i].length = 0;
					md.fd[i].starting_block_address = 0;
					md.fd[i].no_of_blocks = 0;
				}
				else
				{
					memcpy(md.fd[md.file_no - 1].file_name, md.fd[i].file_name, sizeof(md.fd[i].file_name));
					md.fd[i].length = md.fd[md.file_no - 1].length;
					md.fd[i].no_of_blocks = md.fd[md.file_no - 1].no_of_blocks;
					md.fd[i].starting_block_address = md.fd[md.file_no - 1].starting_block_address;
					md.no_of_files -= 1;
					md.file_no -= 1;
					//md.
				}
			}
		}
	}

}