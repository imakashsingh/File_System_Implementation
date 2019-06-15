#include "Header.h"

void write_block(int block_no, char* buff)
{
	FILE *hdd = f_open(HARD_DISK, "rb+");
	int block_addr = 0;
	block_addr = ((block_no)*BLOCK_SIZE);
	fseek(hdd, block_addr, SEEK_SET);
	fwrite(buff, BLOCK_SIZE,1, hdd);
	fclose(hdd);
}